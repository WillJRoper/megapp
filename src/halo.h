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
 * This file define the prototypes for a Halo object.
 ******************************************************************************/
#ifndef HALO_H_
#define HALO_H_

/* Includes */
#include <vector>

using namespace std;

/*! @brief The various types of halo.
 *
 * In the simplest terms there are either:
 *    Host (0): Halos at 200 rho_crit derived from FOF groups.
 *    Substructures (1): Halos at higher overdensities nested in Host halos. */
enum halo_types {
  host,
  substructure,
  halo_type_count,
};

/*! @brief The Halo object.
 *
 * This is the basic unit of MEGA. Any collection of particles to be tested as
 * a halo will intialise a Halo object and use the associated properties and
 * methods to derive its reality, children, propertites, progentiors and
 * (evenutally) descendants. */
class Halo {

public:
  /* Below we define all data members of a halo (attributes) */

  /* Internal properties used in the running of MEGA. */

  /*! The type of the halo (host or substructure). */
  enum halo_types type;

  /*! A pointer to the parent halo from which this halo was derived. */
  Halo *parent;

  /*! A pointer to the first substructure child derived from this halo. */
  Halo *child;

  /*! The number of child halos this halo has. */
  int n_child;

  /*! A pointer to the next halo derived from this halo's parent. */
  Halo *sibling;

  /*! The number of siblings this halo has. */
  int n_sibling;

  /*! The depth of the halo in the overdensity heirarchy. */
  int overden_depth;

  /* Physically meaninful properties. */

  /*! The position of the halo (defined by centre of mass). */
  double x[3];

  /*! The bulk velocity of the halo (mass weighted average). */
  double v[3];

  /* Linking properties. */

  /*! A pointer to this halo's progenitors. */
  vector<Halo> *progs;

  /*! The number of progenitors this halo has. */
  int n_progs;

  /*! A pointer to this halo's descendants. */
  Halo *descs;

  /*! The number of descendants this halo has. */
  int n_descs;

  /* Below we define all member functions of a halo. */

  /* Constructor and destructor. */
  Halo();
  ~Halo();

  /* Methods related to finding structures. */

  /* Methods related to linking structures. */
}

#endif // HALO_H_
