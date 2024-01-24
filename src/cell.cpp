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
 * This file contains the fucntionality for populating and manipulating the
 * cells in the cell grid.
 ******************************************************************************/

// Local includes
#include "Cell.h"

// Template method to add particles to the respective vectors.
template <typename ParticleType>
void Cell::addParticle(const ParticleType &particle) {
  // Use std::vector::push_back to add the particle to the appropriate vector.
  particles<ParticleType>().push_back(particle);
}

// Helper function to get the appropriate vector based on particle type.
template <typename ParticleType> std::vector<ParticleType> &Cell::particles() {
  return dark_matter;
}

#ifndef DARK_MATTER_ONLY

// Template specialization for GasParticle
template <> std::vector<GasParticle> &Cell::particles() { return gas; }

// Template specialization for StarParticle
template <> std::vector<StarParticle> &Cell::particles() { return stars; }

// Template specialization for BHParticle
template <> std::vector<BHParticle> &Cell::particles() { return black_holes; }

#endif /* DARK_MATTER_ONLY */
