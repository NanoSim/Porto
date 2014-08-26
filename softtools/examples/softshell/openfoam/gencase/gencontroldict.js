
var entity = require('soft.entity').db({
    driver:     'mongodb',
    database:   'soft',
    collection: 'openfoam'
});

ControlDict = entity.using('controlDict', '0.1');

var nozzleControlDict = new ControlDict();
nozzleControlDict.set({
    application       : 'mdFoam',
    startFrom         : 'startTime',
    startTime         : 0,
    stopAt            : 'endTime',
    endTime           : 2e-13,
    deltaT            : 1e-15,
    writeControl      : 'runTime',
    writeInterval     : 5e-14,
    purgeWrite        : 0,    
    writeFormat       : 'ascii',
    writePrecision    : 12,
    writeCompression  : 'off',
    timeFormat        : 'general',
    timePrecision     : '6',
    runTimeModifiable : 'true',    
    adjustTimeStep    : 'no'
});

nozzleControlDict.store();
print (nozzleControlDict.id);
