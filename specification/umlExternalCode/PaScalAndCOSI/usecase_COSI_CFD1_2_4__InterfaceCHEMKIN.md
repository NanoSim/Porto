Use Case COSI - CFD1 Level 
======================
Interface with CHEMKIN-II data format, in order to interface with REMARK
--------------------------

Sub-System: PaScal

This use case describes how to PaScal can interface with thermophysical data in OpenFOAM format.

| Section                             | Comment                                                   |
|-------------------------------------|-----------------------------------------------------------|
| Use Case Name                       | Interface with CHEMKIN-II data format            |
| Scope                               | Load thermophysical data and report to screen |
| Level                               | Sub-model                     |
| Primary Actor                       | Researcher uses PaScal (e.g., as standalone tool)         |
| Stakeholders and Interests          | Researchers, programmers developing and testing PaScal   |
| Preconditions                       | Chemical reaction data in CHEMIKIN-II format available  |
| Success Guarantee                   | PaScal reports chemical reaction data for typical conditions |
| Main Success Scenario               | PaScal reads data, and reports them to screen. |
| Extensions                          | -        |
| Special Requirements                | none |
| Technology and Data Variations List | Report of results to screen (by default, switch to disable should be implemented) |
| Frequency of Occurrence             | every PaScal run     |
| Miscellaneous  	                    | data files like these ones: http://www.me.berkeley.edu/gri-mech/version30/text30.html#thefiles   |

