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
 * This file contains the implementation of methods for translating runtime
 * arguments.
 ******************************************************************************/

/* Include necessary headers */
#include <sstream>
#include <stdexcept>

/* Implementation of the AddOption function */
void CommandLineParser::AddOption(const std::string &name,
                                  const int &defaultValue) {
  options_[name] = defaultValue;
}

/* Implementation of the Parse function */
bool CommandLineParser::Parse(int argc, char *argv[]) {

  /* Handle the --help case. */
  if (std::strcmp(argv[argc - 1], "--help") == 0) {
    std::cout << "HELP INCOMING!" << std::endl;
    return true;
  }

  /* First get the parameter file. */
  parameterFile_ = argv[argc - 1];

  /* Loop over all other command line arguments. */
  for (int i = 1; i < argc - 1; ++i) {
    std::string arg(argv[i]);

    /* Check if the argument is a registered option */
    if (options_.count(arg)) {
      if (i + 1 < argc && isdigit(*argv[i + 1])) {

        /* Argument is followed by a value */
        options_[arg] = Convert(argv[++i]);

      } else {

        /* Argument is a flag (no value) */
        options_[arg] = 1;
      }
    } else {
      /* Unknown argument, print an error message */
      std::cerr << "Unknown argument: " << arg << std::endl;
      return false;
    }
  }

  return true;
}

/* Implementation of the GetOption function */
int CommandLineParser::GetOption(const std::string &name) const {
  if (options_.count(name)) {
    return options_.at(name);
  } else {
    /* Option not found, throw an exception */
    throw std::runtime_error("Option not found: " + name);
  }
}

/* Implementation of the Convert function */
int CommandLineParser::Convert(const std::string &value) {

  /* Try to convert the string to an integer handling excpetions. */
  try {
    return std::stoi(value);
  } catch (const std::invalid_argument &ex) {
    throw std::invalid_argument("Invalid conversion for value: " + value);
  } catch (const std::out_of_range &ex) {
    throw std::out_of_range("Conversion out of range for value: " + value);
  }
}

/* Implementation of the GetParameterFile function */
std::string CommandLineParser::GetParameterFile() const {
  return parameterFile_;
}
