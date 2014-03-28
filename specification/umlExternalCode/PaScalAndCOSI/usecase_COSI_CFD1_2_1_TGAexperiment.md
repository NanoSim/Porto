Use Case COSI - CFD1 Level 
======================
Simulate Thermogravimetric Analysis (TGA) Experiment
--------------------------

Sub-System: PaScal (as standalone application)

This use case describes how to use the shrinking core model to simulate a TGA experiment (incl. Temperature and concentration distribution, as well as sintering effects), including sensitivity studies

| Section                             | Comment                                                   |
|-------------------------------------|-----------------------------------------------------------|
| Use Case Name                       | Simulate TGA Experiment               |
| Scope                               | Analyze temperature and concentration distribution within particle, as well as sintering effects|
| Level                               | Top-Level model validation                          |
| Primary Actor                       | Researcher uses PaScal as standalone tool         |
| Stakeholders and Interests          | Researchers that perform DNS, experimentalists that perform the TGA, researchers that develop models on the next level (use developed model)  |
| Preconditions                       | Physical and chemical data available, TGA experiment specified  |
| Success Guarantee                   | Simulation completes without error, results are insensitive to numerical settings
| Main Success Scenario               | TGA experimental data matches simulated data within specified error bound   |
| Extensions                          | Large deviation to reference data              |
| Special Requirements                | none |
| Technology and Data Variations List | Compressed, uncompressed data, final report on key results |
| Frequency of Occurrence             | Peak Usage: run 10-20 simulations per day;     |
| Miscellaneous  	                    | < 30 min simulation time           |

