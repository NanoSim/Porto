#include <QtCore>
#include <QtScript>

#include "qgslspecialprototype.h"
#include "qgslspecial.h"

using soft::gsl::QGSLSpecial;

QGSLSpecialPrototype :: QGSLSpecialPrototype (QObject *parent)
   : QObject (parent)
{}

QGSLSpecialPrototype :: ~QGSLSpecialPrototype()
{}

QScriptValue QGSLSpecialPrototype :: airyAi (double x, QScriptValue callback)
{
   auto obj = qscriptvalue_cast<QGSLSpecial*>(thisObject());
   if (!obj) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }

   gsl_sf_result res;
   auto retval = obj->airyAi(x, res);
   Q_UNUSED(retval);
   if (!callback.isNull() && callback.isFunction()) {
      auto act = context()->activationObject();
      auto args = QScriptValueList() << res.val << res.err;
      callback.call(act, args);
   }
   
   return engine()->toScriptValue(res.val);
}

QScriptValue QGSLSpecialPrototype :: airyBi (double x, QScriptValue callback)
{
   auto obj = qscriptvalue_cast<QGSLSpecial*>(thisObject());
   if (!obj) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }

   gsl_sf_result res;
   auto retval = obj->airyBi(x, res);
   Q_UNUSED(retval);
   if (!callback.isNull() && callback.isFunction()) {
      auto act = context()->activationObject();
      auto args = QScriptValueList() << res.val << res.err;
      callback.call(act, args);
   }
   
   return engine()->toScriptValue(res.val);
}

QScriptValue QGSLSpecialPrototype :: airyAiScaled (double x, QScriptValue callback)
{
   auto obj = qscriptvalue_cast<QGSLSpecial*>(thisObject());
   if (!obj) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }

   gsl_sf_result res;
   auto retval = obj->airyAiScaled(x, res);
   Q_UNUSED(retval);
   if (!callback.isNull() && callback.isFunction()) {
      auto act = context()->activationObject();
      auto args = QScriptValueList() << res.val << res.err;
      callback.call(act, args);
   }
   
   return engine()->toScriptValue(res.val);
}

QScriptValue QGSLSpecialPrototype :: airyBiScaled (double x, QScriptValue callback)
{
   auto obj = qscriptvalue_cast<QGSLSpecial*>(thisObject());
   if (!obj) {
      context()->throwError (QString (tr ("Cannot instanciate QGSLLinearRegression")));
   }

   gsl_sf_result res;
   auto retval = obj->airyBiScaled(x, res);
   Q_UNUSED(retval);
   if (!callback.isNull() && callback.isFunction()) {
      auto act = context()->activationObject();
      auto args = QScriptValueList() << res.val << res.err;
      callback.call(act, args);
   }
   
   return engine()->toScriptValue(res.val);
}

