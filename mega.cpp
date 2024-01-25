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

// Definition of the static instance pointer, this is required for the
// singleton pattern.
Logging *Logging::instance = nullptr;

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
  parser.AddOption(std::string("--verbose"), std::string("-v"), 2);
  int nthreads;
  parser.AddOption(std::string("--threads"), std::string("-t"), 8);

  /* Parse command-line arguments and set options. */
  std::string param_file;
  try {
    parser.Parse(argc, argv);
    verbose = parser.GetOption("--verbose");
    param_file = parser.GetParameterFile();

  } catch (std::exception &e) {
    std::cerr << "COMMAND LINE ARG ERROR: " << e.what() << std::endl;
    return 1;
  }

  /* Set up the logging class for report to the user. */
  // NOTE: This is the first and only time the logging class is instantiated.
  // Herafter, only a single instance exists which is accessed via the static
  // getInstance() method.
  Logging *log = Logging::getInstance(static_cast<LogLevel>(verbose));

  // From here the log is setup so we can use the message, v_message,
  // error, report_error, tic, and toc macros.

  /* Parse the parameter file and populate the class. */
  Parameters params;
  try {
    params.parseYAMLFile(param_file);
  } catch (std::exception &e) {
    report_error();
    return 1;
  }
  // params.printAllParameters();

  /* With all that done... */
  say_hello();

  /* Set up the Engine: attach parameters, set up output strings,
   * and instantiate the threadpool. */
  try {
    Engine *engine = new Engine(params, parser, log);
  } catch (std::exception &e) {
    report_error();
    return 1;
  }

  /* Set up the Domain: attach useful parameters, allocate arrays,
   * and load simulation metadata. */
  try {
    Domain *domain = new Domain(params, log);
  } catch (std::exception &e) {
    report_error();
    return 1;
  }

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
