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

## Building

Presently only the 64bit Linux platform is directly supported in SOFT5.

This guide assumes that the following dependencies has been configured and
installed:

* CMake 3.4 (https://cmake.org/download/)
* Qt 5.6  (http://qt-project.org/downloads)
* MongoDB 3.2.7 (http://www.mongodb.org/downloads)
* MongoDB C-Driver (http://docs.mongodb.org/ecosystem/drivers/c/)

The default SOFT build is configured with multiple options turned off. These options may have additional dependencies. See *Configuration options* below.

Path variables must be set up to include Qt. This can be done by adding the following to .bashrc (or equivalent):

    export QTDIR=/opt/Qt/5.6/gcc_64
    export PATH="$QTDIR:$PATH"

For MongoDB installation, use for example the apt-get installation instructions on the MongoDB site.

For MongoDB C-Driver, see installation instructions here: (https://api.mongodb.com/c/current/installing.html)

Assuming that the source code is located in `~/soft5`, the following commands can be used to configure and build. 

    mkdir ~/build
    cd ~/build
    cmake ~/soft5
    make

## Testing

Assuming that the build has succeeded it is recommended to run the unit tests to verify that SOFT5 works on the system.

For testing purposes, the system requires additional dependencies:

* Google Test 1.7 (https://github.com/google/googletest/). For gtest installation, see for example (http://stackoverflow.com/questions/13513905/how-to-setup-googletest-as-a-shared-library-on-linux) or use the system package installer if available.
* MongoDB must be running locally.

Testing can then be done by:

    cd ~/build
    ctest

Alternatively the tests may be run more verbosely by using:

    make check

## Installation

The installation path can be configured through CMake by running

    cmake -DCMAKE_INSTALL_PREFIX=<path/to/install/to>

This may be `/usr/local/soft5` for Linux systems.

The entire build, test and install step is then carried out by running:

    cd ~/build
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local/soft5 ~/soft5
    make
    make install

It may be that `make install` must be run using `sudo` for administrator privileges:

    sudo make install

## Development dependencies

Additional optional dependencies needed for development:

* Valgrind (http://valgrind.org/)
* Doxygen

## Configuration options

Additional build configurations may be given when configuring with CMake. These can be turned on by passing `-D<configuration_name>=ON` to CMake.

The following configuration options are available with default ON:

    option (WITH_STORAGE_MONGO "MongoDB storage support"      ON)
    option (WITH_STORAGE_JSON "JSON storage support"          ON)
    option (WITH_STORAGE_EXTERNAL "External storage support"  ON)
    option (WITH_PLUGIN_SOFT_KERNEL "Compile kernel plugin"   ON)
    option (WITH_FOREIGN_C "Compile with C API"               ON)
    option (WITH_TESTS "Compile tests"                        ON)

The following configuration options are available with default OFF:

    option (WITH_WEB "Compile with Web support"               OFF)
    option (WITH_HSM "Compile with HSM plugin"                OFF)
    option (WITH_STORAGE_HD5 "HDF5 storage support"           OFF)
    option (WITH_PLUGIN_GSL "Compile GSL plugin"              OFF)
    option (WITH_PLUGIN_NLOPT "Compile NLOpt plugin"          OFF)
    option (WITH_PLUGIN_HD5 "Compile HDF5 plugin"             OFF)
    option (WITH_PLUGIN_MONGO "Compile MongoDB plugin"        OFF)
    option (WITH_FOREIGN_FORTRAN "Compile with Fortran API"   OFF)
    option (WITH_FOREIGN_PYTHON "Compile with Python API"     OFF)
    option (WITH_NUMERICS "Compile with SOFT Numerics"        OFF)
    option (USE_PYTHON3 "Create Python 3 bindings instead of Python 2" OFF)
    option (WITH_PORTO "Compile porto plugins"                OFF)

These may have additional dependencies, as described below.

#### WITH_STORAGE_HD5

This option enables HDF5 as a storage back-end.

Additional dependencies:

* HDF5 1.8.17 (https://www.hdfgroup.org/HDF5/release/obtain5.html)
