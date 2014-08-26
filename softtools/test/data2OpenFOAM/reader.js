importData = require('soft.storage.importer').importData;

/* Define the external data source with a URL and a schema
 */

var externalSourceInfo = {
    url: "file:///path/to/data",
    driver: "logdata",
    schema: ["particleDataExt", "1.0"]
};

/*!
  Define the destination schema
 */
var dest = {
    schema: ["myfirst", "1.0"]
};


/* Call upon the schema-translator that will pass the data from the
 * external source to the destination entity. Uses the data-driver
 * that matches the external source schema.
 */

var myObj = importData(externalSourceInfo, destinationInfo);

/* Perform a simulation/calculation on myObj to build the structure
 * 'myObj'
 */
var result = runCalculation(myObj)


/*  Use the result-object as input to the generator
 *  'generateOpenFOAM'
 */
var src_OpenFOAM = generateOpenFOAM(result);


