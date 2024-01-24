# Define the minimum cmake version
cmake_minimum_required(VERSION 3.12)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Project metadata
set(TARGET mega)
project(MEGA++ C CXX)

# Set the version
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

# ================= OPTIONS =================

# Add configuration options
option(WITH_DEBUG_CHECKS "Enable debugging checks. (This will be much slower!)" OFF)
option(DARK_MATTER_ONLY "Ignore all code related to Baryons." OFF)
set(NUM_PART_SPECIES 6 CACHE INT "How many particle species are in the snapshot? (Including types with 0 particles)")

# Add an optional feature
if(WITH_DEBUG_CHECKS)
    add_compile_definitions(WITH_DEBUG_CHECKS)
endif()

if(DARK_MATTER_ONLY)
    add_compile_definitions(DARK_MATTER_ONLY)
endif()

# ================= DEPENDANCIES =================

# HDF5
find_package(HDF5 REQUIRED C CXX)

# pthreads
find_package(Threads REQUIRED)

# ================= METADATA COLLECTION =================

# Execute Git commands to get revision and branch
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND git log -1 --format=%cd --date=short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get compiler information including version
if (CMAKE_CXX_COMPILER_VERSION AND CMAKE_CXX_COMPILER_ID)
  set(COMPILER_INFO "${CMAKE_CXX_COMPILER_ID}@${CMAKE_CXX_COMPILER_VERSION}")
elseif (NOT CMAKE_CXX_COMPILER_VERSION AND CMAKE_CXX_COMPILER_ID)
  set(COMPILER_INFO "${CMAKE_CXX_COMPILER_ID}")
else()
  set(COMPILER_INFO "Unknown Compiler")
endif()

# Get compiler flags (CFLAGS)
string(REPLACE ";" " " CFLAGS_INFO "${CMAKE_CXX_FLAGS}")

# Add the necessary version paths
configure_file(
    "${CMAKE_SOURCE_DIR}/src/config/version.h.in"
    "${CMAKE_SOURCE_DIR}/src/version.h"
)

# ================= SOURCE FILES =================

# Create a list of source files
set(SOURCE_FILES
    mega.cpp
    src/domain.cpp
    src/serial_io.cpp
    src/talking.cpp
    src/threadpool.cpp
    # Add more files here
)

# Add conditional files to the source list
if(ENABLE_FEATURE_X)
    list(APPEND SOURCE_FILES feature_x.cpp)
endif()

# if(NOT DARK_MATTER_ONLY)
#     list(APPEND SOURCE_FILES feature_x.cpp)
# endif()

# ================= COMPILATION =================

# Create the executable using the source files
add_executable(${TARGET} ${SOURCE_FILES})

# ================= COMPILE DEFINITIONS =================

# Define compile-time constants
target_compile_definitions(${TARGET} PRIVATE NUM_PART_SPECIES=${NUM_PART_SPECIES})

# ================= LIBRARY INCLUSION =================

# HDF5
target_link_libraries(${TARGET} PRIVATE ${HDF5_LIBRARIES})
target_include_directories(${TARGET} PRIVATE ${HDF5_INCLUDE_DIRS})

# pthreads
target_link_libraries(${TARGET} PRIVATE Threads::Threads)
