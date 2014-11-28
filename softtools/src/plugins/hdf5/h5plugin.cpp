#include <QtCore>
#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>
#include <h5ns.h>
#include "register.hh"

#include "qh5.h"
#include "qh5dataset.h"    
#include "qh5datatype.h" 
#include "qh5group.h"  
#include "qh5property.h"
#include "qh5attribute.h" 
#include "qh5dataspace.h"  
#include "qh5file.h"
#include "h5plugin.h"


using soft::hdf5::QH5;
using soft::hdf5::QH5Dataset;
using soft::hdf5::QH5Datatype;
using soft::hdf5::QH5Group;
using soft::hdf5::QH5Property;
using soft::hdf5::QH5Attribute;
using soft::hdf5::QH5Dataspace;
using soft::hdf5::QH5File;


Q_DECLARE_METATYPE (QH5*)
Q_DECLARE_METATYPE (QH5Dataset*)
Q_DECLARE_METATYPE (QH5Datatype*)
Q_DECLARE_METATYPE (QH5Group*)
Q_DECLARE_METATYPE (QH5Property*)
Q_DECLARE_METATYPE (QH5Attribute*)
Q_DECLARE_METATYPE (QH5Dataspace*)
Q_DECLARE_METATYPE (QH5File*)

H5Plugin :: ~H5Plugin()
{}

void H5Plugin :: registerPlugin (QScriptEngine *engine)
{
//   registerPrototype<QH5Prototype, QH5> (engine);
   registerConstructor<QH5>          (engine, "H5",          creator<QH5>);
   registerConstructor<QH5Dataset>   (engine, "H5Dataset",   creator<QH5Dataset>);
   registerConstructor<QH5Datatype>  (engine, "H5Datatype",  creator<QH5Datatype>);
   registerConstructor<QH5Group>     (engine, "H5Group",     creator<QH5Group>);
   registerConstructor<QH5Attribute> (engine, "H5Attribute", creator<QH5Attribute>);
   registerConstructor<QH5Dataspace> (engine, "H5Dataspace", creator<QH5Dataspace>);
   registerConstructor<QH5File>      (engine, "H5File",      creator<QH5File>);  
}

