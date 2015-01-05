qmake .prf configuration files
==============================

Included in this directory are qmake project configuration
files. Please make sure you have defined the QMAKEFEATURES environment
variable to point to the current directory, and the prf-files are
referring to the correct paths.

about qmake and prf-files
-------------------------

qmake lets you create your own features that can be included in
project files by adding their names to the list of values specified by
the CONFIG variable. Features are collections of custom functions and
definitions in .prf files that can reside in one of many standard
directories. The locations of these directories are defined in a
number of places, and qmake checks each of them in the following order
when it looks for .prf files: In a directory listed in the
QMAKEFEATURES environment variable; this contains a colon-separated
list of directories.  In a directory listed in the QMAKEFEATURES
property variable; this contains a colon-spearated list of
directories.  In a features directory residing within a mkspecs
directory. mkspecs directories can be located beneath any of the
directories listed in the QMAKEPATH environment variable (a
colon-separated list of directories). ($QMAKEPATH/mkspecs/<features>)
In a features directory residing beneath the directory provided by the
QMAKESPEC environment variable. ($QMAKESPEC/<features>) In a features
directory residing in the data_install/mkspecs
directory. (data_install/mkspecs/<features>) In a features directory
that exists as a sibling of the directory specified by the QMAKESPEC
environment variable. ($QMAKESPEC/../<features>)
