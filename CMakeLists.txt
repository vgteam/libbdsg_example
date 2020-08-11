# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)
# We can't use CMake 3.11 until we no longer have to run on Ubuntu 18.04.

# Project's name
project(libbdsg_example)

# We build using c++14
set(CMAKE_CXX_STANDARD 14)

# Make sure we know the right install directories for our platform
include(GNUInstallDirs)

# Add the bdsg project as a subdirectory.
# This lets CMake get an idea of what it is installing, so the libraries from
# libbdsg will come alogn with this project at install time.
# Don't bring along its Python bindings
set(BUILD_PYTHON_BINDINGS OFF CACHE BOOL "Build libbdsg Python bindings")
# Or build its Doxygen docs
set(RUN_DOXYGEN OFF CACHE BOOL "Run libbdsg doxygen")
# This should give us a library target `libbdsg`
add_subdirectory("deps/libbdsg")

# Define a program
add_executable(libbdsg_example ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
# Make it link libbdsg and all the libraries it packages and depends on, and
# use its interface includes.
target_link_libraries(libbdsg_example libbdsg)
    
# When installing, install our binary
install(TARGETS libbdsg_example
        DESTINATION "${CMAKE_INSTALL_BINDIR}")
# We will also install libbdsg at install time.

