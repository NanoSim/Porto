Use Case COSI - CFD1 Level 
======================
Interface with OpenFOAM data format for thermophysical data
--------------------------

Sub-System: PaScal

This use case describes how PaScal can interface with thermophysical data in OpenFOAM format.

| Section                             | Comment                                                   |
|-------------------------------------|-----------------------------------------------------------|
| Use Case Name                       | Interface with OpenFOAM data format for thermophysical data              |
| Scope                               | Load thermophysical data and report to screen |
| Level                               | Sub-model                     |
| Primary Actor                       | Researcher uses PaScal (e.g., as standalone tool)         |
| Stakeholders and Interests          | Researchers, programmers developing and testing PaScal   |
| Preconditions                       | Thermophysical data in OpenFOAM format available  |
| Success Guarantee                   | PaScal reports thermophysical data for various temperatures |
| Main Success Scenario               | PaScal reads data, computes thermophysical properties at various temperatures, and reports them. Agreement with OpenFOAM data. |
| Extensions                          | -        |
| Special Requirements                | none |
| Technology and Data Variations List | Report of results to screen (by default, switch to disable should be implemented) |
| Frequency of Occurrence             | every PaScal run     |
| Miscellaneous  	                    | -   |

