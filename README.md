# Porto

The Porto platform, based on SOFT5 (SINTEF Open Framework and Tools)
will provide a lightweight and flexible system for data and workflow
management. The prevalent way of working with multi-scale model
development is not data-agnostic. This complicates the adoption of new
software and various workflows, as data has to be fitted to every
transitional phase. Existing systems that do embrace unified access to
the data are often relying on predefined schemas. Porto will overcome
these limitations by adopting technology from exemplary projects,
tailored to fit the needs of the NanoSim project. The key benefit of
the unified data access is the ability for offline-analysis of big
amounts of data by different research groups all over the world. Data
from one simulation can be used by various researchers to generate new
model formulations, effectively removing the need for redundant
simulations of the same problem. In addition, the seamless integration
of statistical analysis tools will significantly speed up the model
development process, which is currently a tedious and cumbersome task.

## Installation

This guide assumes that the following software has been configured and
installed:

* Qt 5.x  [Qt Downloads](http://qt-project.org/downloads)
* MongoDB [MongoDB Downloads](http://www.mongodb.org/downloads)
* MongoDB C-Driver [C MongoDB Driver](http://docs.mongodb.org/ecosystem/drivers/c/)
* gtest 1.7 [Google Test](https://code.google.com/p/googletest/) (optional)
* valgrind [Valgrind Home](http://valgrind.org/) (optional)
* Google Closure Compiler [Google Developers](https://developers.google.com/closure/compiler/) (optional)

Be sure you have included the bin and lib directories of the above software tools in the PATH and LD_LIBRARY_PATH of your operating system!

Define the following environmental variables in your .bashrc or .cshrc

	PORTOBASE          = <path>/Porto/portobase
	LD_LIBRARY_PATH    = $PORTOBASE/lib:$PORTOBASE/bin/plugins:$LD_LIBRARY_PATH
	PATH               = $PORTOBASE/bin:$PATH
	QMAKEFEATURES      = $PORTOBASE/prf:$QMAKEFEATURES
    GTEST_LIBRARY_PATH = <path/to/gtest-lib>
    BSON_LIBRARY_PATH  = <path/to/libbson>
    MONGO_LIBRARY_PATH = <path/to/libmongo>

and be sure they are exported.

Build and install the application like this:

    cd <path>/Porto
	qmake
	make install
	
You should now have the following files:

    $PORTOBASE/bin/portoshell
    $PORTOBASE/bin/plugins/libmongoplugin.so
    $PORTOBASE/include/Porto
    $PORTOBASE/include/porto.h
    $PORTOBASE/include/portons.h
    $PORTOBASE/include/scriptengine.h

## Where to go from here

To get started with Porto-development, please find the introductory [Porto Tutorial](doc/tutorial.org)
