#include <QtCore>
#include <QtScript>
#include <functional>

#include "qnlopt.h"
#include "qnloptprototype.h"

using soft::nlopt::QNLopt;

QNLoptPrototype :: QNLoptPrototype (QObject *parent)
  : QObject (parent)
{}

QNLoptPrototype :: ~QNLoptPrototype()
{}

double f(QScriptValue callback, QScriptValue activation, unsigned, const double *, double *, void *)
{
  QTextStream (stdout) << "f called!\n";
  auto ret = callback.call(activation, QScriptValueList());
  return ret.toNumber();
}


double testF (QScriptValue callback, QScriptValue act, unsigned n, const double * arr)
{
  QTextStream (stdout) << "testF called: n = " << n << '\n';
  QScriptValueList args;
  for (unsigned i = 0; i < n; ++i) {
    args << arr[i];
  }
  auto ret = callback.call(act, args);
  QTextStream (stdout) << "ok\n";
  return ret.toNumber();
}

QScriptValue QNLoptPrototype :: testCallback(QScriptValue callback)
{
  using namespace std::placeholders;
  auto obj = qscriptvalue_cast<QNLopt*>(thisObject());
  if (!obj) {
    context()->throwError (QString (tr ("Cannot instanciate QNLopt")));
    return QScriptValue();
  }
  auto act = context()->activationObject();
  if (!callback.isNull() && callback.isFunction()) {
    auto f1 = std::bind(testF, callback, act, _1, _2);
    obj->testCallback(f1);
  }
  return QScriptValue();
}

QScriptValue QNLoptPrototype :: setMinObjective (QScriptValue callback, QScriptValue fData)
{
  using namespace std::placeholders;
  auto obj = qscriptvalue_cast<QNLopt*>(thisObject());
  if (!obj) {
    context()->throwError (QString (tr ("Cannot instanciate QNLopt")));
    return QScriptValue();
  }
  auto act = context()->activationObject();
  if (!callback.isNull() && callback.isFunction()) {
    auto f1 = std::bind(f, callback, act, _1, _2, _3, _4);
    obj->setMinObjective(f1, nullptr);
  }
  
  return QScriptValue();
}

QScriptValue QNLoptPrototype :: addInequalityConstraint (QScriptValue callback, QScriptValue constraintData, QScriptValue tolerance)
{
  return QScriptValue();
}
