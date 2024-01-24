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
 * This file contains all functionality related to "talking" to the user.
 ******************************************************************************/

/* Includes */
#include <iostream>
using namespace std;

/* Local includes. */
#include "talking.h"
#include "version.h"

std::string padString(const std::string &input, std::size_t length) {

  /* Set up the result. */
  std::string result = input;

  /* Loop until the desired length is reached. */
  while (result.length() < length) {
    result += " ";
  }
  return result;
}

/**
 * @brief Prints a greeting message to the standard output containing code
 * version and revision number
 *
 * This was constructed using the 'figlet' tool and the 'slant' font. The
 * lower-bar of the f is then lengthened.
 *
 * @param fof Is this for the FOF greeting?
 */
void say_hello() {

  string string1 =
      R"(          )"
      R"(__/\\\\____________/\\\\___/\\\\\\\\\\\\\\\______/\\\\\\\\\\\\______/)"
      R"(\\\\\\\\\--O-O-O-O-O-O-O         O-O         0          )";
  string string2 =
      R"(           )"
      R"(_\/\\\\\\________/\\\\\\__\/\\\///////////_____/\\\//////////_____/)"
      R"(\\\\\\\\\\\\\__      \ / \       /           /           )";
  string string3 =
      R"(            )"
      R"(_\/\\\//\\\____/\\\//\\\__\/\\\_______________/\\\_______________/)"
      R"(\\\/////////\\\_      O   O-O-O-O     O     O            )";
  string string4 =
      R"(             )"
      R"(_\/\\\\///\\\/\\\/_\/\\\__\/\\\\\\\\\\\______\/\\\____/)"
      R"(\\\\\\\__\/\\\_______\/\\\_          \     \   / \   /             )";
  string string5 =
      R"(              )"
      R"(_\/\\\__\///\\\/___\/\\\__\/\\\///////_______\/\\\___\/////)"
      R"(\\\__\/\\\\\\\\\\\\\\\-O-O-O-O   O-O-O-O-O-O-O-O-O-O-O-O-O-O-O)";
  string string6 =
      R"(               )"
      R"(_\/\\\____\///_____\/\\\__\/\\\______________\/\\\_______\/\\\__\/\\\///)"
      R"(//////\\\_      \ /   \       /     \           )";
  string string7 =
      R"(                )"
      R"(_\/\\\_____________\/\\\__\/\\\______________\/\\\_______\/\\\__)"
      R"(\/\\\_______\/\\\_      O     O     O       O          )";
  string string8 =
      R"(                 )"
      R"(_\/\\\_____________\/\\\__\/\\\\\\\\\\\\\\\__\//\\\\\\\\\\\\/)"
      R"(___\/\\\_______\/\\\_    / \     \   /                  )";
  string string9 =
      R"(                  )"
      R"(_\///______________\///___\///////////////____\////////////_____\///)"
      R"(________\///--O-O   O-O-O-O-O                    )";

  cout << endl;
  cout << string1 << endl;
  cout << string2 << endl;
  cout << string3 << endl;
  cout << string4 << endl;
  cout << string5 << endl;
  cout << string6 << endl;
  cout << string7 << endl;
  cout << string8 << endl;
  cout << string9 << endl;
  cout << endl;

  /* Report some information about the MEGA version being run. */
  int nPad = 30;
  cout << padString(string(" Version : "), nPad) << PROJECT_VERSION_MAJOR << "."
       << PROJECT_VERSION_MINOR << "." << PROJECT_VERSION_PATCH << endl;

  cout << endl;

  cout << " Git:" << endl
       << padString(string(" On branch: "), nPad) << GIT_BRANCH << endl
       << padString(string(" Using revision: "), nPad) << GIT_REVISION << endl
       << padString(string(" Last updated: "), nPad) << GIT_DATE << endl;
  // printf(" Webpage : %s\n\n", PACKAGE_URL);
  // printf(" Config. options: %s\n\n", configuration_options());

  cout << endl;

  cout << padString(string(" Compiler: "), nPad) << COMPILER_INFO << endl;
  cout << padString(string(" CFLAGS: "), nPad) << CFLAGS_INFO << endl;

  cout << endl;

  cout << padString(string(" HDF5 library version: "), nPad) << HDF5_VERSION
       << endl;
  // #ifdef HAVE_FFTW
  //     printf(" FFTW library version     : %s\n", fftw3_version());
  // #endif
  // #ifdef HAVE_LIBGSL
  //     printf(" GSL library version      : %s\n", libgsl_version());
  // #endif
  // #ifdef WITH_MPI
  //     printf(" MPI library version      : %s\n", mpi_version());
  // #ifdef HAVE_METIS
  //     printf(" METIS library version    : %s\n", metis_version());
  // #endif
  // #ifdef HAVE_PARMETIS
  //     printf(" ParMETIS library version : %s\n", parmetis_version());
  // #endif
  // #endif
  printf("\n");
}
