Use Case COSI - CFD1 Level 
======================
Simulate Drying of a Wet, Porous Particle
--------------------------

Sub-System: PaScal

This use case describes how to use a detailed drying model to predict the temperature, moisture, and nanoparticle distribution within a porous particle 

| Section                             | Comment                                                   |
|-------------------------------------|-----------------------------------------------------------|
| Use Case Name                       | Simulate Drying of a Wet, Porous Particle            |
| Scope                               | Analyze spatial variation of various quanitite within a particle  |
| Level                               | Top-Level model validation                          |
| Primary Actor                       | Researcher uses PaScal (typically, as standalone tool)         |
| Stakeholders and Interests          | Researchers that perform DNS, experimentalists that perform drying experiments, researchers that develop models on the next level (use developed model)  |
| Preconditions                       | Physical and chemical data available, drying experiment specified  |
| Success Guarantee                   | Simulation completes without error, results are insensitive to numerical settings
| Main Success Scenario               | Experimental data of drying process matches simulated data within specified error bound   |
| Extensions                          | Large deviation to reference data              |
| Special Requirements                | none |
| Technology and Data Variations List | Compressed, uncompressed data, final report on key results |
| Frequency of Occurrence             | Peak Usage: run 10-20 simulations per day     |
| Miscellaneous  	                    | < 5 min simulation time in case a single particle is considered  |

