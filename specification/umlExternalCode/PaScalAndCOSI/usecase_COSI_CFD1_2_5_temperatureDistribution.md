Use Case COSI - CFD1 Level 
======================
Prediction of the temperature distribution within particles
--------------------------

Sub-System: PaScal

This use case describes how PaScal can be used to compute the temperature distribution within particles, taking heat transfer inside and outside the particles into account

| Section                             | Comment                                                   |
|-------------------------------------|-----------------------------------------------------------|
| Use Case Name                       | Prediction of the temperature distribution within particles      |
| Scope                               | Analyze temperature distribution within particle |
| Level                               | Top-Level model validation             |
| Primary Actor                       | Researcher uses PaScal (e.g., as standalone tool)         |
| Stakeholders and Interests          | Researchers that perform DNS, researchers that develop models on the next level (use developed model)   |
| Preconditions                       | Physical data available, heat transfer experiment specified  |
| Success Guarantee                   | Simulation completes without error, results are insensitive to numerical settings |
| Main Success Scenario               | Simulated data matches analytical results, or experimental data, within specified error bound |
| Extensions                          | Large deviation to reference data       |
| Special Requirements                | none |
| Technology and Data Variations List | Compressed, uncompressed data, final report on key results |
| Frequency of Occurrence             | Peak Usage: run 10-20 simulations per day     |
| Miscellaneous  	                    | Must interact with LIGGGHTS, CFDEM, and C3PO   |

