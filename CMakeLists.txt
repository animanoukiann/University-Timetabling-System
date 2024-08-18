cmake_minimum_required(VERSION 3.10)

# Project name
project(UniversityTimetable)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/argparse)
include_directories(${CMAKE_SOURCE_DIR}/third_party/nlohmann/include)

# Source files for the library
set(SRC ${CMAKE_SOURCE_DIR}/src/Course.cpp
        ${CMAKE_SOURCE_DIR}/src/Instructor.cpp
        ${CMAKE_SOURCE_DIR}/src/TimeSlot.cpp
        ${CMAKE_SOURCE_DIR}/src/University.cpp)

# Create the dynamic library
add_library(UniversityTimetable SHARED ${SRC})

# Specify where to output the library
set_target_properties(UniversityTimetable PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dynlib)

# Install rules
install(TARGETS UniversityTimetable LIBRARY DESTINATION ${CMAKE_SOURCE_DIR})

find_package(pybind11 REQUIRED)

# Add Pybind11 bindings file
set(PYBIND_SOURCES ${CMAKE_SOURCE_DIR}/src/UniBindings.cpp)

# Create the Python module
pybind11_add_module(pyuniversity ${PYBIND_SOURCES})

# Link the module with the library
target_link_libraries(pyuniversity PRIVATE UniversityTimetable)

set_target_properties(pyuniversity PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/UI/backend
)