# Workflow example REMARC -> ANSYS Fluent UDF

In this Porto tutorial we want to use the scripting shell to set up a
REMARC simulation from a script and push the data to a (central)
database.

## Build a collection 
In order to run REMARC we need the following components:

    1. A thermo.dat file
    2. A path to existing DFT data
    3. A REMARC configuration file

In order to demonstrate the flexibility of the Porto platform we
handle all these components differently. The thermo.dat file we will
store in the data warehouse. The DFT data is potentially huge, and we
want just to make available the reference information. The REMARC
configuration we would like to set up from a script.

This demonstrates three aspects of Porto

    1. The ability to store a whole file in the database
    2. The handling of references to static files or directories
    3. Make use of proprietary file format backends and make the data
       scriptable



