/*******************************************************************************
 * This file is part of MEGA++.
 * Copyright (c) 2023 Will Roper (w.roper@sussex.ac.uk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This file contains the functionality for the container of domain related
 * information and any functions used to load and manipulate the domain.
 ******************************************************************************/

/* Includes. */
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

/* Local includes. */
#include "cell.h"
#include "domain.h"
#include "params.h"
#include "particles.h"
#include "serial_io.h"

/** @brief The constructor for the Domain.
 *
 * This will set all the parameters passed from the parameter file and attach
 * them to the Domain object. It will also allocate the arrays for Cells and
 * Particles which will live until MEGA is finished.
 *
 * @param params An instance of the Parameters class containing the parameter
 *               file contents.
 */
Domain::Domain(Parameters params, Logging *log) {

  tic();

  // First lets attach all the parameter file defined members.

  /* Is the volume periodic? */
  periodic = params.getParameter("Simulation/periodic", 1);
  message("Running with periodic: %d", periodic);

  /* Are we analysising a zoom simulation? */
  is_zoom = params.getParameter("Simulation/is_zoom", 0);
  if (is_zoom)
    message("This is a zoom simulation: Only the high resolution region "
            "will be considered.");

  /* Which particles are we working on? */
  for (int i = 0; i < num_part_species; i++) {

    /* A variable for the key. */
    std::ostringstream param;

    /* Get the parameter key. */
    char buffer[24]; // Adjust the buffer size as needed.
    snprintf(buffer, sizeof(buffer), "Particles/part_type_%d", i);
    param << buffer;

    /* Store flag. */
    part_flags[i] = params.getParameter(param.str(), 0);
  }

  /* How many cells are on an axis? */
  cdim[0] = params.getParameter("Tasking/cell_grid_dim", 16);
  cdim[1] = cdim[0];
  cdim[2] = cdim[0];
  message("The cell grid has dimensions: [%d, %d, %d]", cdim[0], cdim[1],
          cdim[2]);

  /* How many cells in total? */
  ncells = cdim[0] * cdim[1] * cdim[2];
  ntop_cells = cdim[0] * cdim[1] * cdim[2];
  message("There are %d top level cells in total", ntop_cells);

  /* Open the first snapshot and get some metadata. */
  HDF5Helper *snap = new HDF5Helper(
      "/Users/willroper/Documents/3D Printing/Python/ani_hydro_1379.hdf5");

  /* Read the boxsize. */
  if (!(snap->readAttribute("/Header", "BoxSize", boxsize))) {
    error("Failed to read the BoxSize from the snapshot!");
  }
  message("Read box dimensions from the first snapshot: [%.2f, %.2f, %.2f]",
          boxsize[0], boxsize[1], boxsize[2]);

  /* Read the number of particles in total. */
  if (!(snap->readAttribute("/Header", "NumPart_Total", npart_type))) {
    error("Failed to read the NumPart_Total from the snapshot!");
  }

  /* Zero any particle counts not flagged for use in part_flags. */
  for (int i = 0; i < num_part_species; i++) {
    if (!(part_flags[i])) {
      npart_type[i] = 0;
    }
  }

  /* Create the print output for the number of particles. Necessary because
   * the number of particle species is set by a configuation flag. */
  std::string s_arr = "[ ";
  for (int i = 0; i < num_part_species; i++) {
    s_arr += " " + std::to_string(npart_type[i]);
  }
  s_arr += "]";
  message("Read the number of particles from the first snapshot: %s",
          s_arr.c_str());
  delete snap;

  // Set the total number of particles.
  npart_tot = 0;
  for (int i = 0; i < num_part_species; i++) {
    npart_tot += npart_type[i];
  }
  if (npart_tot == 0) {
    error(
        "The total number of particles is 0! Check your Particles:part_type_* "
        "parameters and ensure a non-zero particle species is flagged.");
  }
  message("Total number of particles: %ld", npart_tot);

  // Calculate the width of the top level cells.
  for (int ijk = 0; ijk < 3; ijk++) {
    width[ijk] = boxsize[ijk] / cdim[ijk];
    iwidth[ijk] = 1 / width[ijk];
  }

  // Allocate the array for dark matter particles.
  dark_matter = (DMParticle *)std::aligned_alloc(
      part_align, npart_type[1] * sizeof(DMParticle));

#ifndef DARK_MATTER_ONLY
  // Allocate the array for baryonic particles.
#endif

  // Allocate the array of top level cells
  top_cells = (Cell *)std::aligned_alloc(cell_align, ntop_cells * sizeof(Cell));

  // Allocate the array of subcells (starts with the entire first layer of the
  // tree as a guess)
  sub_cells =
      (Cell *)std::aligned_alloc(cell_align, 8 * ntop_cells * sizeof(Cell));

  toc("Initialising the Domain");
}
