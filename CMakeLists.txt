cmake_minimum_required(VERSION 3.0)
project(NMEA_Parser)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/nlohmann)

# Add executable target
add_executable(nmea_parser
    cpp_parser/nmea_parse.cpp
)

# Copy nmea.txt to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nmea.txt ${CMAKE_CURRENT_BINARY_DIR}/nmea.txt COPYONLY)
