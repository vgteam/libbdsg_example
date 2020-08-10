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
  
# Add the bdsg project as a subdirectory.
# This lets CMake get an idea of what it is installing, so the libraries from
# libbdsg will come alogn with this project at install time.
# Don't bring along its Python bindings
set(BUILD_PYTHON_BINDINGS OFF CACHE BOOL "Build libbdsg Python bindings")
# Or build its Doxygen docs
set(RUN_DOXYGEN OFF CACHE BOOL "Run libbdsg doxygen")
# This should give us a library target `libbdsg`
add_subdirectory("deps/libbdsg")

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
add_executable(libbdsg_example ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
# Make it link libbdsg and all the libraries it packages and depends on, and
# use its interface includes.
target_link_libraries(libbdsg_example libbdsg)
    
# When installing, install our binary
install(TARGETS libbdsg_example
        DESTINATION "${CMAKE_INSTALL_BINDIR}")
# We will also install libbdsg at install time.

