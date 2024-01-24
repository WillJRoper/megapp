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

class CommandLineParser {
public:
  CommandLineParser() = default;

  void AddOption(const std::string &name, const int &defaultValue);
  bool Parse(int argc, char *argv[]);
  int GetOption(const std::string &name) const;
  std::string GetParameterFile() const;

private:
  std::map<std::string, int> options_;
  std::string parameterFile_;
  int Convert(const std::string &value);
};

#include "cmd_parser.inl" // Include the implementation file.

#endif // COMMANDLINEPARSER_H
