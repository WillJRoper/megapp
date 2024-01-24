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
 * This header file contains the defintions for the container of domain related
 * information and any functions used to load and manipulate the domain.
 ******************************************************************************/
#ifndef DOMAIN_H_
#define DOMAIN_H_

/* Local includes. */
#include "cell.h"
#include "logging.h"
#include "params.h"
#include "particles.h"

#define num_part_species NUM_PART_SPECIES

/**
 * @class Domain
 * @brief A Singleton containing all the infromation about the domain.
 *
 * The "Domain" constitutes every parameter related to the simulated volume
 * and the particles it contains. All particles and cells dividing the volume
 * are stored within the Domain along with the properties defining their
 * construction.
 *
 * In addition to the properties this class contains all of the functions to
 * construct and update the domain, including cell construction, particle
 * population, I/O from the snapshots/fof catalog, etc.
 *
 * Note: This does not include parameters related to the running of MEGA, these
 * are housed in the #engine.
 *
 * @param periodic Is the volume periodic?
 * @param boxsize The size of the simulation volume along each axis.
 * @param npart_type The number of particles of each type (0-6).
 * @param npart_tot The total number of particles of any type.
 * @param cdim The number of cells along an axis.
 * @param ncells The total number of cells.
 * @param ntop_cells The total number of top level cells.
 * @param width The width of a top level cell.
 * @param iwidth The inverse of the top level cell width.
 * @param top_cells Pointers to the top level cells.
 * @param sub_cells Pointers to cells used for the cell trees.
 */
class Domain {
public:
  /* Flag for whether the volume is periodic. */
  int periodic;

  /* Is it a zoom simulation? */
  int is_zoom;

  /* The boxsize. */
  double boxsize[3];

  /* Number of particles of each type. */
  int npart_type[num_part_species];

  /* Total number of particles. */
  size_t npart_tot;

  /* Which particle types are we working on? */
  int part_flags[num_part_species];

  /* The number of cells along each axis. */
  int cdim[3];

  /* The total number of cells. */
  int ncells;

  /* The total number of top level cells. */
  int ntop_cells;

  /* The width of the top level cells. */
  double width[3];

  /* The inverse width of the top level cells. */
  double iwidth[3];

  /* The top level cells. */
  Cell *top_cells;

  /* The pool of cells for the cell tree. */
  Cell *sub_cells;

  /* The dark matter particles array. */
  DMParticle *dark_matter;

#ifndef DARK_MATTER_ONLY

  /* The gas particles array. */
  GasParticle *gas;

  /* The star particles array. */
  StarParticle *stars;

  /* The black hole particles array. */
  BHParticle *black_holes;

#endif /* DARK_MATTER_ONLY */
  Domain(Parameters params, Logging log);
  ~Domain();

private:
};
#endif // DOMAIN_H_
