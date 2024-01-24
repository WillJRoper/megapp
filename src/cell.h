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
 * This header file contains the defintions for a cell. The basic unit on which
 * all calculations are done.
 ******************************************************************************/

//  Includes
#include <vector>

// Local includes.
#include "particles.h"

#ifndef CELL_H_
#define CELL_H_

#define cell_align 128

class Cell {
public:
  // Location of the cell.
  double loc[3];

  // The width of the cell.
  double width[3];

private:
  // Pointers to the dark matter particles in this cell.
  std::vector<DMParticle> *dark_matter;

#ifndef DARK_MATTER_ONLY

  // The gas particles in this cell.
  std::vector<GasParticle> *gas;

  // The star particles in this cell.
  std::vector<StarParticle> *stars;

  // The black hole particles in this cell.
  std::vector<BHParticle> *black_holes;

#endif /* DARK_MATTER_ONLY */

public:
  // Template method to add particles to the respective vectors.
  template <typename ParticleType>
  void addParticle(const ParticleType &particle) {
    // Use std::vector::push_back to add the particle to the appropriate vector.
    particles<ParticleType>().push_back(particle);
  }

private:
  // Helper function to get the appropriate vector based on particle type.
  template <typename ParticleType> std::vector<ParticleType> &particles();
};

#endif // CELL_H_
