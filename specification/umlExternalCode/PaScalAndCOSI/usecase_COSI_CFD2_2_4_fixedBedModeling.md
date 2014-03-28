Use Case COSI - CFD2 Level 
======================
Perform a CFDEM simulation of a Fixed Bed
--------------------------

Sub-System: CFDEM, PaScal, C3PO

This use case describes how to run and analyze fixed Bed simulations in order to develop closures

| Section                             | Comment                                                   |
|-------------------------------------|-----------------------------------------------------------|
| Use Case Name                       | Perform a CFDEM simulation of a Fixed Bed     |
| Scope                               | Setup and run a CFDEM simulation  for a dense, static particle bed |
| Level                               | End Usage                          |
| Primary Actor                       | Researchers that derive closures for fixed simulations    |
| Stakeholders and Interests          | Engineers and researchers that fixed bed simulations and need to rely on closures |
| Preconditions                       | Physical and chemical data, particle properties, as well as boundary conditions are defined.  |
| Success Guarantee                   | Derived closures agree with available data, effect of numerical parameters, e.g., grid resolution, is small |
| Main Success Scenario               | Simulation have completed with no errors, data for closures is analyzed by C3PO, and model is handed over to Porto for further processing  |
| Extensions                          | -  |
| Special Requirements                | none |
| Technology and Data Variations List | Compressed, uncompressed data, final report on key results |
| Frequency of Occurrence             | Peak Usage: setup and start 1-5 simulations per day     |
| Miscellaneous  	                    | < 1 month computing time for each case  |
