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
 * This is the main function used to run MEGA.
 ******************************************************************************/

/* Includes */
#include <iostream>
#include <sstream>
#include <stdexcept>

/* Local includes */
#include "src/cmd_parser.h"
#include "src/domain.h"
#include "src/engine.h"
#include "src/logging.h"
#include "src/params.h"
#include "src/talking.h"

int main(int argc, char *argv[]) {

  /* Check command line arguments. */
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0]
              << " [Optional Arguments] <parameter_file>" << std::endl;
    return 1;
  }

  CommandLineParser parser;

  /* Add runtime options with default values. */
  int verbose;
  parser.AddOption("-verbose", 2);
  int nthreads;
  parser.AddOption("-threads", 8);

  /* Parse command-line arguments and set options. */
  std::string param_file;
  if (parser.Parse(argc, argv)) {
    verbose = parser.GetOption("-verbose");
    nthreads = parser.GetOption("-threads");
    param_file = parser.GetParameterFile();

  } else {
    throw std::runtime_error("Failed to parse the command line arguments!");
    return 1;
  }

  /* Set up the logging class for report to the user. */
  Logging log(static_cast<LogLevel>(verbose));

  /* Parse the parameter file and populate the class. */
  Parameters params(log);
  if (params.parseYAMLFile(param_file))
    return 1;
  // params.printAllParameters();

  /* With all that done... */
  say_hello();

  /* Set up the Engine: attach parameters, set up output strings,
   * and instantiate the threadpool. */
  Engine *engine = new Engine(params, parser, log);

  /* Set up the Domain: attach useful parameters, allocate arrays,
   * and load simulation metadata. */
  Domain *domain = new Domain(params, log);

  // engine->threadpool->map(function1, data2, 1000,
  //                         engine->threadpool->threadpool_auto_chunk_size,
  //                         extraData);

  /* The main loop of MEGA: loop over snapshots. */
  for (int i = 0; i < 10; i++) {

    /* Read the snapshot data. */

    /* Construct the adaptive cell grid. */

    /* Construct the tasks. */

    /* Heigh-ho, heigh-ho, it's off to work we go... */

    /* Clean up */
  }
}
