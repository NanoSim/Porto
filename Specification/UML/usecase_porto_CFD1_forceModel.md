Use Case Porto -CFD1 Level 
======================
Force Model Development
--------------------------

This use case describes how a model for the fluid-particle interaction force can be developed based on data from Direct Numerical Simulations.

| Section                             | Comment                                                   |
|-------------------------------------|-----------------------------------------------------------|
| Use Case Name                       | Develop a Force Model                  |
| Scope                               | Analyze forces on particles to develop a force model to be used in resolved Euler-Euler or Euler-Lagrange Simulations    |
| Level                               | Sub-Model (for force)                          |
| Primary Actor                       | Researcher that performs fully resolved DNS           |
| Stakeholders and Interests          | Researchers that perform DNS, Engineers & industrial end user (use developed model), researchers that develop models on the next level (use developed model)  |
| Preconditions                       | Data from fully resolved DNS available (DNS running or completed), particle distribution appropriately initialized |
| Success Guarantee                   | Force Model has been benchmarked against reference data, DNS is converged (grid dependency effects have been assesst), DNS code has been verified     |
| Main Success Scenario               | No grid effects affect results, DNS results compare favourable with reference data, DNS results follow expected trend   |
| Extensions                          | Grid dependency detected, large deviation to reference data              |
| Special Requirements                | Porto module need to be able to handle large amounts of data (up to 5GB per case)  |
| Technology and Data Variations List | Binary, compressed, uncompressed data, as well as text and images |
| Frequency of Occurrence             | Peak Usage: analyze 5 DNS cases per day; need ca. 20-100 DNS runs to develop a full model     |
| Miscellaneous  	                  | Interface to filtering tool "C3PO" not yet defined.           |

