SOFT5 {#installation}
=================

# Installation

This chapter describes how to install the SOFT5/Porto system. The
software is available on github: https://github.com/nanosim/porto. The
software is currently only distributed as source code, and requires
some preparation, compilation and installation steps. 


Presently only the 64bit Linux platform is directly supported in SOFT5.
Here we will show the necessary steps for installing the software on Ubuntu 16.04
LTS. Compilation on OpenSuse Leap 42.2 is similar.

*NOTE: Windows is presently only supported with the options -DWITH_FOREIGN_C=Off -DWITH_TESTS=Off -DWITH_STORAGE_MONGO=Off -DWITH_STORAGE_EXTERNAL=Off passed to CMake.*

## Preparation of basic software

### The very basics

Your installation of Soft5/port assumes that the following dependencies has been configured and
installed:

* CMake 3.4 (https://cmake.org/download/)
* Qt 5.6  (http://qt-project.org/downloads)
* MongoDB 3.2.7 (http://www.mongodb.org/downloads)
* MongoDB C-Driver (http://docs.mongodb.org/ecosystem/drivers/c/) (This is NOT necessary in case 'BUILD_MONGOC' is specified during configuration, see *Configuration* below!)

The default SOFT build is configured with multiple options turned off. These options may have additional dependencies. See *Configuration* below.

The software is available as a Git repository, and a local
installation of git is required. Either become root to perform the
following operations, or use the sudo command (assumes the current
user have sudo access; instead of "apt-get", the user may want to use "zypper", or use "YAST2" on OpenSUSE)

	$ sudo apt-get update
	$ sudo apt-get install -y git

Before the code can be compiled, a set of dependencies needs to be
installed. First of all the development tools (compilers++) is
needed. We will also need CMake, HDF5 (development), and boost (development)

	$sudo apt-get install -y build-essential cmake libhdf5-dev libboost-all-dev

### GTEST
The user CAN install also the GTEST library 'libgtest-dev'. Before we can use (the optional manual) GTest, it needs to be build as a shared library

	$ cd /usr/src/gtest
	$ sudo cmake -DEBUILD_SHARED_LIBS=On . && sudo make
	$ ln *.so /usr/lib
	
However, the GTEST library can also be installed during the configuration process using '-DBUILD_GTEST=yes' (See section 
'Configuration' below). This is much more comfortable, and hence suggested.


### Qt5
SOFT5 is built on Qt5. Qt5 comes with its own online installer which
can be downloaded and run for the official website (alternatively, one can install it via
a package manager). 
Here we use Qt 5.7 (any Qt 5.x should be ok)

	$ wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run
	$ chmod +x qt-opensource-linux-x64-5.7.0.run
	$ ./qt-opensource-linux-x64-5.7.0.run
	
Now that all external dependencies have been resolved, we will also
need to set up our local command prompt environment. If you are using
bash, add the following to your .bashrc (this is not necessary in case of a system installation)

	$ export QTDIR=<path-to-installed-Qt>/5.7/<platform>
	$ export PATH=$QTDIR/bin:$PATH
	$ export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH

here <platform> will typically be gcc_64 on GNU/Linux.

### MongoDB and the MongoDB-C driver

In order to use MongoDB, it is recommended to installit via the package manager (e.g., YAST2 in OpenSUSE Leap 42.2). Alternatively, and for UBUNTU, one can add the official repository.

	$ sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 \
	  --recv 0C49F3730359A14518585931BC711F9BA15703C6
	$ echo "deb [ arch=amd64,arm64 ] http://repo.mongodb.org/apt/ubuntu xenial/mongodb-org/3.4 \
	  multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-3.4.list
	$ sudo apt-get update
	$ sudo apt-get install -y mongodb-org libmongoc-1.0.0


For the (optional manual) MongoDB C-Driver installation, see installation instructions here: (https://api.mongodb.com/c/current/installing.html)

Note, 'libmongoc-1.0.0' can also installed automatically using 'BUILD_MONGOC' during the ccmake configuration, see below. This is much more comfortable, and hence suggested.

## Download and preparation of the build
First, download the source code to an appropriate folder using git.
	
	$ git clone git@github.com:NanoSim/Porto.git soft5
	
Second, one needs to set up the compilation directory (this will be an out-of-source build to keep the original source code directories clean!). Assuming that the source code is located in `~/soft5`, the following commands can be used : 

    mkdir ~/build-soft5
    cd ~/build-soft5

## Configuration 

We now assume that the user has opened a terminal in the ~/build-soft5 directory.

The installation path can be configured through CMake by running

    cmake -DCMAKE_INSTALL_PREFIX=<path/to/install/to>  -D<other options> ~/soft5

"path/to/install/to":
This may be `/usr/local/soft5` for Linux systems, and in case the user wishes to make soft5/porto 
available for all users.

"Other options" may include (these are also the three recommended ones!):
- WITH_PORTO=yes
- BUILD_MONGOC=yes
- BUILD_GTEST=yes

For example, one may use

    cd ~/build
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local/soft5 -DWITH_PORTO=yes -DBUILD_MONGOC=yes -DBUILD_GTEST=yes ../soft5

Note that your local installation may need help to find different
software components. It is therefore recommended to use ccmake (CMake
with curses interface) when configuring the build process. Hence, one may want to use "ccmake ." (after the "cmake ..." command) to further configure the installation (see the next paragraph).

Ccmake offers the following configuration options (note, some of these options do not work on OpenSUSE, e.g., the GSL plugin):

 BUILD_GTEST                      OFF
 BUILD_HD5                        OFF
 BUILD_MONGOC                     OFF
 CMAKE_BUILD_TYPE                 Debug
 CMAKE_INSTALL_PREFIX             /usr/local
 Qt5Concurrent_DIR                /usr/lib64/cmake/Qt5Concurrent
 Qt5Core_DIR                      /usr/lib64/cmake/Qt5Core
 Qt5Network_DIR                   /usr/lib64/cmake/Qt5Network
 Qt5Script_DIR                    /usr/lib64/cmake/Qt5Script
 USE_PYTHON3                      OFF
 WITH_FOREIGN_C                   OFF
 WITH_FOREIGN_FORTRAN             OFF
 WITH_FOREIGN_PYTHON              OFF
 WITH_HSM                         OFF
 WITH_NUMERICS                    OFF
 WITH_PLUGIN_GSL                  OFF
 WITH_PLUGIN_HD5                  OFF
 WITH_PLUGIN_MONGO                OFF
 WITH_PLUGIN_NLOPT                OFF
 WITH_PLUGIN_SOFT_KERNEL          OFF
 WITH_PORTO                       OFF
 WITH_STORAGE_EXTERNAL            OFF
 WITH_STORAGE_HD5                 OFF
 WITH_STORAGE_JSON                OFF
 WITH_STORAGE_MONGO               OFF
 WITH_TESTS                       OFF
 WITH_WEB                         OFF 

Note, that the "BUILD_*" options lead to the download and compilation of additional pieces of software (e.g., the mongoc library). These additional pieces of software will be installed in the build-soft5/usr directory.

The "WITH_*" options may have additional dependencies, as described below.

### WITH_STORAGE_HD5

This option enables HDF5 as a storage back-end.

Additional dependencies:

* HDF5 1.8.17 (https://www.hdfgroup.org/HDF5/release/obtain5.html)

## Build and install
The user simply types

    make
    make install

Note, one may want to use 

    make -j 4

to compile in parallel (with 4 processes in this case) to speed up the compilation process.
Note, it may be that `make install` must be run using `sudo` for administrator privileges:

    sudo make install

## Update the .bashrc file

Let us assume we have used as "CMAKE_INSTALL_PREFIX" the directory '/usr/local/soft5'. Then, 
your .bashrc file should look as follows:

export NANOSIM=/usr/local/soft5
export SOFTBASE=$NANOSIM/lib64/soft5
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOSIM/lib64:$SOFTBASE/storage:$SOFTBASE/plugins:$HOME/NanoSim/build-soft5/usr/lib64/
export PATH=$PATH:$NANOSIM/bin

## Testing

Assuming that the build has succeeded it is recommended to run the unit tests to verify that SOFT5 works on the system.

For testing purposes, the system requires additional dependencies:

* Google Test 1.7 (https://github.com/google/googletest/). For gtest installation, see for example (http://stackoverflow.com/questions/13513905/how-to-setup-googletest-as-a-shared-library-on-linux) or use the system package installer if available.
* MongoDB must be running locally.

Testing can then be done by:

    cd ~/build-soft5
    ctest

Alternatively, instead of ctest one can use

	make test

The tests may be run more verbosely by using:
    make check


## Development dependencies

Additional optional dependencies needed for development are:

* Valgrind (http://valgrind.org/)
* Doxygen

