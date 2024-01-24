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
 * This file contains the machinery for translating runtime arguments.
 ******************************************************************************/

#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

/* Includes */
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @class CommandLineParser
 * @brief A utility class for parsing and handling command line arguments in a
 *        flexible and extensible manner.
 *
 * The `CommandLineParser` class allows users to define and parse command line
 * options with associated default values. It supports both short and long
 * option names. The class is designed to handle the extraction of option
 * values, retrieval of parameter file names, and provides mechanisms for error
 * handling during argument parsing.
 *
 * The class is specifically tailored for scenarios where command line arguments
 * may include options with or without values, as well as flags (no associated
 * values). It supports the conversion of string values to integers, with
 * appropriate exception handling to deal with conversion errors or out-of-range
 * values.
 *
 * Example usage:
 * @code
 * CommandLineParser parser;
 * parser.AddOption("input", "-i", 0);   // Add an option with primary name
 * "input", alternative name "-i", and default value 0.
 * parser.AddOption("output", "-o", 1);  // Add another option for output with
 * default value 1.
 *
 * if (parser.Parse(argc, argv)) {
 *     int inputValue = parser.GetOption("input");
 *     int outputValue = parser.GetOption("output");
 *     std::string parameterFile = parser.GetParameterFile();
 *     // Process the values and the parameter file as needed.
 * } else {
 *     // Handle parsing errors.
 * }
 * @endcode
 *
 */
class CommandLineParser {
private:
  std::map<std::string, int> options_;
  std::map<std::string, std::string> names_;
  std::string parameterFile_;

public:
  CommandLineParser() = default;

  /**
   * @brief Adds an option with its alternative name and default value.
   *
   * @param name The primary name of the option.
   * @param alt_name The alternative name of the option.
   * @param defaultValue The default value of the option.
   */
  void AddOption(const std::string &name, const std::string &alt_name,
                 const int &defaultValue) {
    options_[name] = defaultValue;
    names_[alt_name] = name;
    names_[name] = name;
  }

  /**
   * @brief Parses the command line arguments.
   *
   * @param argc The number of command line arguments.
   * @param argv The array of command line arguments.
   *
   * @return True if parsing is successful, false otherwise.
   *
   * @throw std::runtime_error if an unknown argument is encountered.
   */
  bool Parse(int argc, char *argv[]) {

    /* Handle the --help case. */
    if (std::strcmp(argv[argc - 1], "--help") == 0) {
      std::cout << "HELP INCOMING!" << std::endl;
      return true;
    }

    /* First get the parameter file. */
    parameterFile_ = argv[argc - 1];

    /* Loop over all other command line
     * arguments. */
    for (int i = 1; i < argc - 1; ++i) {
      std::string arg(names_[argv[i]]);

      /* Check if the argument is a registered
       * option */
      if (options_.count(arg)) {
        if (i + 1 < argc && isdigit(*argv[i + 1])) {

          /* Argument is followed by a value */
          options_[arg] = Convert(argv[++i]);

        } else {

          /* Argument is a flag (no value) */
          options_[arg] = 1;
        }
      } else {
        /* Unknown argument, print an error
         * message */
        throw std::runtime_error("Unknown argument: %s", arg);
        return false;
      }
    }

    return true;
  }

  /**
   * @brief Retrieves the value of a specified option.
   *
   * @param name The name of the option.
   *
   * @return The value of the specified option.
   *
   * @throw std::runtime_error if the option is not found.
   */
  int GetOption(const std::string &name) const {
    if (options_.count(name)) {
      return options_.at(name);
    } else {
      /* Option not found, throw an exception */
      throw std::runtime_error("Option not found: " + name);
    }
  }

  /**
   * @brief Retrieves the parameter file name.
   *
   * @return The parameter file name.
   */
  std::string GetParameterFile() const { return parameterFile_; }

private:
  /**
   * @brief Converts a string to an integer, handling exceptions.
   *
   * @param value The string to be converted.
   *
   * @return The converted integer value.
   *
   * @throw std::invalid_argument if the conversion is invalid.
   *
   * @throw std::out_of_range if the conversion is out of range.
   */
  int Convert(const std::string &value) {

    /* Try to convert the string to an integer
     * handling excpetions. */
    try {
      return std::stoi(value);
    } catch (const std::invalid_argument &ex) {
      throw std::invalid_argument("Invalid conversion for value: " + value);
    } catch (const std::out_of_range &ex) {
      throw std::out_of_range("Conversion out of range for "
                              "value: " +
                              value);
    }
  }
};

#endif // COMMANDLINEPARSER_H
