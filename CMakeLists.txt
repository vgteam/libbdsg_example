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
  
# Add the bdsg project as an available dependency.
# Make sure to configure it to install where the external project system wants it.
# But also configure it to use a final rpath base don where this project will ultimately install
# ExternalProject is smart enough to auto-detect that it is a CMake project and
# build it, but not smart enough to auto-install it to the right place.
ExternalProject_Add(bdsg
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/../libbdsg"
    CMAKE_ARGS "${CMAKE_ARGS};-Wno-dev;-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>;-DBUILD_PYTHON_BINDINGS=OFF;-DRUN_DOXYGEN=OFF")
# Read out where the headers and libraries are installed to
ExternalProject_Get_property(bdsg INSTALL_DIR)
set(bdsg_INCLUDE "${INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")
set(bdsg_LIB "${INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}")

# Create libbdsg as an INTERFACE library which can be installed.
# We would use an IMPORTED library but those can't be installed.
# See https://stackoverflow.com/a/41179630
add_library(libbdsg INTERFACE)
# Track it and all its transitive dependencies.
target_link_libraries(libbdsg INTERFACE
                      "${bdsg_LIB}/libbdsg${CMAKE_SHARED_LIBRARY_SUFFIX}"
                      "${bdsg_LIB}/libhandlegraph${CMAKE_SHARED_LIBRARY_SUFFIX}"
                      "${bdsg_LIB}/libsdsl${CMAKE_SHARED_LIBRARY_SUFFIX}"
                      "${bdsg_LIB}/libdivsufsort64${CMAKE_SHARED_LIBRARY_SUFFIX}"
                      "${bdsg_LIB}/libdivsufsort${CMAKE_SHARED_LIBRARY_SUFFIX}")
# And all the includes to build with.
# target_include_directories(INTERFACE) also doesn't work on imported libraries.
target_include_directories(libbdsg INTERFACE
                          ${bdsg_INCLUDE})
# Make sure we build bdsg before using it
add_dependencies(libbdsg bdsg)

# Make sure when we install our program we set its rpath so it can find its
# dependencies we also install.
# Set this up before program target is created.
# If this is a library and we want to use it as an ExternalProject, this needs
# to stay the default or non-installed consumers of the library won't be able
# to find its dependencies.
# We put this in the cache if it isn't set, and preserve the cached value (from
# "-D" on the command line if specified) if it is set.
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Library search path upon installation")

# Define a program
add_executable(libbdsg_example ${CMAKE_SOURCE_DIR}/src/main.cpp)
# Make it link libbdsg and all the libraries it packages and depends on, and
# use its interface includes.
target_link_libraries(libbdsg_example libbdsg)
    
# When installing, install our binary
install(TARGETS libbdsg_example
        DESTINATION "${CMAKE_INSTALL_BINDIR}")
# Also install all the libraries it depends on.
# TODO: even though INTERFACE libraries can be installed, it doesn't actually
# copy their built library files around because they have none (only an
# interface). So we copy the libs over ourselves.
install(DIRECTORY "${bdsg_LIB}/"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}")
# TODO: Now how do we set library rpaths??? Do we need to or will our executable cover it?

### IF YOUR PROJECT IS ITSELF A LIBRARY ###
# Make sure to install headers in bdsg_INCLUDE alongside your own headers.

