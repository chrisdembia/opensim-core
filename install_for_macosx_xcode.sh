#!/bin/bash
# Shell script to build OpenSim and its dependencies.
# See README.md for more information.
# This file only writes to the directory containing the opensim-core folder
# (the source code directory).

# Change to the directory containing this script.
cd ${0%/*}
# Move up to the directory containing the source directory.
cd ..

# Dependencies.
# =============
# Create the build directory for dependencies.
mkdir opensim-core-dependencies-build
cd opensim-core-dependencies-build

# Run CMake.
cmake ../opensim-core/dependencies -GXcode

# Build and install the dependencies in debug and relwithdebinfo
# configurations.
cmake --build . --config Debug
cmake --build . --config RelWithDebInfo


# OpenSim.
# ========
cd ../
mkdir opensim-core-build
cd opensim-core-build

# Run CMake.
cmake ../opensim-core -GXcode \
    -DOPENSIM_DEPENDENCIES_DIR=../opensim_dependencies_install

# Build and install OpenSim in debug and relwithdebinfo configurations.
cmake --build . --target install --config Debug
cmake --build . --target install --config RelWithDebInfo

