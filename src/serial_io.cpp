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
 * This file contains the functionality for reading from and writing to
 * a HDF5 file in serial.
 ******************************************************************************/

/* Local includes. */
#include "serial_io.h"

/** @brief The constructor for the #HDF5Helper class.
 *
 * This will create an instance of the HDF5Helper class with the HDF5 file
 * opened in the specified mode.
 *
 * Note: HDF5 file is closed when the deconstructor is called.
 *
 * @param filename The filepath to the HDF5 file to read or write.
 * @param accessMode The mode with which to open the HDF5 file. Can be
 *                   H5F_ACC_RDONLY, H5F_ACC_RDWR, H5F_ACC_TRUNC, H5F_ACC_EXCL,
 *                   or H5F_ACC_CREAT. Defaults to H5F_ACC_RDONLY.
 */
HDF5Helper::HDF5Helper(const std::string &filename, unsigned int accessMode)
    : file(filename, accessMode) {}

HDF5Helper::~HDF5Helper() { this->file.close(); }

bool HDF5Helper::createGroup(const std::string &groupName) {
  try {
    H5::Group group = file.createGroup(groupName);
    return true;
  } catch (H5::Exception &e) {
    return false;
  }
}

bool HDF5Helper::writeAttribute(const std::string &objName,
                                const std::string &attributeName,
                                const std::string &attributeValue) {
  try {
    H5::Group group(file.openGroup(objName));
    H5::DataSpace dataspace(H5S_SCALAR);
    H5::StrType strType(H5::PredType::C_S1, attributeValue.length());
    H5::Attribute attr =
        group.createAttribute(attributeName, strType, dataspace);
    attr.write(strType, attributeValue);
    return true;
  } catch (H5::Exception &e) {
    return false;
  }
}

template <typename T>
bool HDF5Helper::writeDataset(const std::string &datasetName,
                              const std::vector<T> &data) {
  try {
    H5::DataSpace dataspace(1, data.size());
    H5::DataSet dataset =
        file.createDataSet(datasetName, H5::PredType::NATIVE_DOUBLE, dataspace);
    dataset.write(data.data(), H5::PredType::NATIVE_DOUBLE);
    return true;
  } catch (H5::Exception &e) {
    return false;
  }
}

template <typename T>
bool HDF5Helper::readDataset(const std::string &datasetName,
                             std::vector<T> &data) {
  try {
    H5::DataSet dataset = file.openDataSet(datasetName);
    H5::DataSpace dataspace = dataset.getSpace();
    std::vector<T> buffer(dataspace.getSelectNpoints());
    dataset.read(buffer.data(), H5::PredType::NATIVE_DOUBLE);
    data = buffer;
    return true;
  } catch (H5::Exception &e) {
    return false;
  }
}

// template <typename T>
// bool HDF5Helper::readAttribute(const std::string &objName,
//                                const std::string &attributeName,
//                                T &attributeValue) {
//   try {
//     H5::Group group(file.openGroup(objName));
//     H5::Attribute attr = group.openAttribute(attributeName);
//     H5::DataType attrType = attr.getDataType();
//     attr.read(attrType, &attributeValue);
//     return true;
//   } catch (H5::Exception &e) {
//     return false;
//   }
// }

// template bool
// HDF5Helper::readAttribute<double *>(const std::string &objName,
//                                     const std::string &attributeName,
//                                     double *&attributeValue);
