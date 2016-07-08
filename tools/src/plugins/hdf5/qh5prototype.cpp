#include <QtScript>
#include <QtCore>
#include "qh5.h"
#include "qh5prototype.h"

using soft::hdf5::QH5;

QH5Prototype :: QH5Prototype (QObject *parent)
   : QObject (parent)
{}

QH5Prototype :: ~QH5Prototype()
{}

QScriptValue QH5Prototype :: read (QString const &key, QScriptValue callback)
{
   auto qh5 = qscriptvalue_cast<QH5*>(thisObject());
   if (!qh5) {
      context()->throwError (QString (tr ("Cannot instanciate QH5")));
   }

   if (!callback.isNull() && callback.isFunction()) {
      auto act = context()->activationObject();
      auto args = QScriptValueList() << "hello";
      callback.call(act, args);
   }
   
   return engine()->toScriptValue(qh5->read(key));
}

QScriptValue QH5Prototype :: write (QString const &key, QVariant const &value, QScriptValue const attr)
{
   auto qh5 = qscriptvalue_cast<QH5*>(thisObject());
   if (!qh5) {
      context()->throwError (QString (tr ("Cannot instanciate QH5")));
   }

   QVariantMap vMap;
   if (attr.isObject()) {
      vMap = attr.toVariant().toMap();
   }
   return qh5->write(key, value, vMap);
}
