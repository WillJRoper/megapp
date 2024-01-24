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
 * This header file contains the defintions for all particle types.
 ******************************************************************************/
#ifndef PARTICLE_H
#define PARTICLE_H

#define part_align 128

/**
 * @class Particle
 * @brief A base class for different types of particles.
 *
 * @param id The ID of the particle.
 * @param pos Position of the particle in 3D space.
 * @param physical_pos Physical position of the particle.
 * @param mass Mass of the particle.
 * @param vel Velocity of the particle.
 * @param physical_vel Physical velocity of the particle.
 * @param grav_nrg Gravitational energy.
 * @param kin_nrg Kinetic energy.
 */
class Particle {
public:
  size_t id;
  double pos[3];
  double physical_pos[3];
  double mass;
  double vel[3];
  double grav_nrg;
  double kin_nrg;

  /**
   * @brief Constructor for the Particle class.
   *
   * @param x X-coordinate of the position.
   * @param y Y-coordinate of the position.
   * @param z Z-coordinate of the position.
   * @param mass Mass of the particle.
   * @param vx X-component of velocity.
   * @param vy Y-component of velocity.
   * @param vz Z-component of velocity.
   * @param soft Softening parameter.
   * @param grav_nrg Gravitational energy.
   * @param kin_nrg Kinetic energy.
   */
  Particle(double x, double y, double z, double mass, double vx, double vy,
           double vz, double soft, double grav_nrg, double kin_nrg);

  /**
   * @brief Virtual destructor for the Particle class.
   */
  virtual ~Particle();
};

/**
 * @class DMParticle
 * @brief A class representing a Dark Matter particle.
 */
class DMParticle : public Particle {
public:
  // Add additional members specific to DMParticle
  // Constructors, methods, and other members can be added here.
};
#ifndef DARK_MATTER_ONLY
/**
 * @class StarParticle
 * @brief A class representing a star particle.
 */
class StarParticle : public Particle {
public:
  // Add additional members specific to StarParticle
  // Constructors, methods, and other members can be added here.
};

/**
 * @class GasParticle
 * @brief A class representing a gas particle.
 */
class GasParticle : public Particle {
public:
  // Add additional members specific to GasParticle
  // Constructors, methods, and other members can be added here.
};

/**
 * @class BHParticle
 * @brief A class representing a black hole particle.
 */
class BHParticle : public Particle {
public:
  // Add additional members specific to BHParticle
  // Constructors, methods, and other members can be added here.
};

#endif // DARK_MATTER_ONLY
#endif // PARTICLE_H
