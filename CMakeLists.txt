# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)
# We can't use CMake 3.11 until we no longer have to run on Ubuntu 18.04.

# Project's name
project(libbdsg_example)

# We build using c++14
set(CMAKE_CXX_STANDARD 14)

# Make sure we have ExternalProject (ships with CMake)
include(ExternalProject)
# And we know the right install directories for our platform
include(GNUInstallDirs)

# Add libbdsg as an available dependency
#ExternalProject_Add(bdsg
#  GIT_REPOSITORY https://github.com/vgteam/libbdsg.git
#  GIT_TAG 88f46391aafdf9c1e5a3215ba3545a8b6b270ccf)
  
# Add libbdsg as an available dependency.
# Make sure to configure it to install where the external project system wants it.
# ExternalProject is smart enough to auto-detect that it is a CMake project and
# build it, but not smart enough to auto-install it to the right place.
ExternalProject_Add(bdsg
  SOURCE_DIR "/z/home/anovak/workspace/libbdsg"
  CMAKE_ARGS "${CMAKE_ARGS};-Wno-dev;-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>;-DBUILD_PYTHON_BINDINGS=OFF")
# Read out where the headers and libraries are installed to
ExternalProject_Get_property(bdsg INSTALL_DIR)
set(bdsg_INCLUDE "${INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")
set(bdsg_LIB "${INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}")

# Define a program
add_executable(libbdsg_example ${CMAKE_SOURCE_DIR}/src/main.cpp)
# Make it depend on libbdsg
add_dependencies(libbdsg_example bdsg)
# Make it search the libbdsg include directory
target_include_directories(libbdsg_example PUBLIC ${bdsg_INCLUDE})
# Make it link libbdsg out of the lib directory
# TODO: Accoutn for platform library extension
# TODO: Account for transitive dependencies even though the configure step for libbdsg hasn't run yet and we can't use CMAKE inter-package communication or pkg-config
target_link_libraries(libbdsg_example "${bdsg_LIB}/libbdsg.so")

