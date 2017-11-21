SOFT5 {#mainpage}
=================

## Source code structure

The main SOFT5 repository is located at https://stash.code.sintef.no/projects/SOFT/repos/soft5/browse

The source code is divided into multiple nested CMake projects.

    /cmake          Common CMake scripts for the project
    /core           SOFT core API
    /doc            Documentation
    /foreign        Foreign language API, such as C, Fortran and Python
    /numerics       SimCoFlow specific
    /porto          Porto/NanoSim specific
    /storage        Storage back-ends (MongoDB, HDF5, etc.)
    /tools          Necessary and supporting utilities

## Installation

Please see the file "INSTALL.md" for detailed instructions on how to
configure and compile the software.


