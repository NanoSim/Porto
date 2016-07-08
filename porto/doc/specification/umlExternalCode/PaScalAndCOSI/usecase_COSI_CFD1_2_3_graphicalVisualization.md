Use Case COSI - CFD1 Level 
======================
Graphic Visualization of Particle Properties
--------------------------

Sub-System: PaScal

This use case describes how key metrics of intra-particle properties (e.g., min/mean/max temperature) can be visualized.

| Section                             | Comment                                                   |
|-------------------------------------|-----------------------------------------------------------|
| Use Case Name                       | Graphic Visualization of Particle Properties              |
| Scope                               | Vizualize temperature and concentration distribution, report integral quantities |
| Level                               | Visualization                      |
| Primary Actor                       | PaScal Users (all except standalone users)  |
| Stakeholders and Interests          | Programmers (verification and debugging), Researchers using PaScal  |
| Preconditions                       | PaScal simulation (with LIGGGHTS or CFDEM) has been set up, or has been run  |
| Success Guarantee                   | Paraview can visualize various quantities for a large number of particles |
| Main Success Scenario               | PaScal simulation has been run, data is dumped to vtk or hdf5 format, Paraview can read and visualize data  |
| Extensions                          | - |
| Special Requirements                | Data should be dumped during the simulation in order to check results; global quantities (e.g., the maximum max. temperature wihtin a particle, or the total heat transfer rate should be logged to a file  |
| Technology and Data Variations List | Binary, compressed, or uncompressed data; report on key global quantities to be logged during the simulation run |
| Frequency of Occurrence             | During every PaScal run |
| Miscellaneous  	                    | - |

