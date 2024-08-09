cmake_minimum_required(VERSION 3.10)

#project_name
project(UniversityTimetable)

#Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party/argparse)
include_directories(${CMAKE_SOURCE_DIR}/third_party/nlohmann/include)

#Source files for the library
set(SRC ${CMAKE_SOURCE_DIR}/src/Course.cpp
        ${CMAKE_SOURCE_DIR}/src/Instructor.cpp
        ${CMAKE_SOURCE_DIR}/src/TimeSlot.cpp
        ${CMAKE_SOURCE_DIR}/src/University.cpp)

#Create the dynamic library
add_library(UniversityTimetable SHARED ${SRC})

#Specify where to output the library
set_target_properties(UniversityTimetable PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dynlib)

#Source files for the executable
set(TEST_SRC ${CMAKE_SOURCE_DIR}/src/main.cpp)

#Create the executable
add_executable(main ${TEST_SRC})

#Link the main with the library
target_link_libraries(main UniversityTimetable)

#Specify where to output the executable
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# Install rules
install(TARGETS UniversityTimetable LIBRARY DESTINATION ${CMAKE_SOURCE_DIR})

install(TARGETS main RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})
