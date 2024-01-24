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
 * This header file contains the prototypes for reading from and writing to
 * a HDF5 file in serial.
 ******************************************************************************/
#ifndef SERIAL_IO_H_
#define SERIAL_IO_H_

#include <H5Cpp.h>
#include <string>
#include <vector>

class HDF5Helper {
public:
  H5::H5File file;

  HDF5Helper(const std::string &filename,
             unsigned int accessMode = H5F_ACC_RDONLY);
  ~HDF5Helper();

  bool createGroup(const std::string &groupName);
  bool writeAttribute(const std::string &objName,
                      const std::string &attributeName,
                      const std::string &attributeValue);

  template <typename T>
  bool writeDataset(const std::string &datasetName, const std::vector<T> &data);

  template <typename T>
  bool readDataset(const std::string &datasetName, std::vector<T> &data);
  // template <typename T>
  // bool readAttribute(const std::string &objName,
  //                    const std::string &attributeName, T &attributeValue);
  template <typename T>
  bool readAttribute(const std::string &objName,
                     const std::string &attributeName, T &attributeValue) {
    try {
      H5::Group group(file.openGroup(objName));
      H5::Attribute attr = group.openAttribute(attributeName);
      H5::DataType attrType = attr.getDataType();
      attr.read(attrType, &attributeValue);
      return true;
    } catch (H5::Exception &e) {
      return false;
    }
  }

private:
};

#endif // SERIAL_IO_H_
