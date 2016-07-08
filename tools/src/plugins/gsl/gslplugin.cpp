/*!
   This is a Soft plugin that wraps the GNU Scientific Library and
   for Qt/C++ utilization and scriptability.

 */

#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>
#include <qgslstatistics.h>
#include "gslplugin.h"
#include "qgslconstants.h"
#include "qgslstatistics.h"
#include "qgslpolynomials.h"
#include "qgslpolynomialsprototype.h"
#include "qgsllinearregression.h"
#include "qgsllinearregressionprototype.h"
#include "qgslvector.h"
#include "qgslmatrix.h"
#include "qgslspecial.h"
#include "qgslspecialprototype.h"
#include "register.hh"

using soft::gsl::QGSLConstants;
using soft::gsl::QGSLStatistics;
using soft::gsl::QGSLLinearRegression;
using soft::gsl::QGSLPolynomials;
using soft::gsl::QGSLVector;
using soft::gsl::QGSLMatrix;
using soft::gsl::QGSLSpecial;

Q_DECLARE_METATYPE(soft::gsl::QGSLConstants*)
Q_DECLARE_METATYPE(soft::gsl::QGSLStatistics*)
Q_DECLARE_METATYPE(soft::gsl::QGSLLinearRegression*)
Q_DECLARE_METATYPE(soft::gsl::QGSLPolynomials*)
Q_DECLARE_METATYPE(soft::gsl::QGSLVector*)
Q_DECLARE_METATYPE(soft::gsl::QGSLMatrix*)
Q_DECLARE_METATYPE(soft::gsl::QGSLSpecial*)

GSLPlugin :: ~GSLPlugin()
{}

template <class T>
inline void registerClass(QScriptValue &parentObj, QString const &name, QObject *that) 
{
   auto module = new T(that);
   auto qobj = parentObj.engine()->newQObject(module, QScriptEngine::ScriptOwnership);
   parentObj.setProperty(name, qobj);
}


static QScriptValue createMatrix (QScriptContext *ctx, QScriptEngine *engine)
{
   QGSLMatrix *ptr = nullptr;
   if (ctx->argumentCount() == 0) {
      ptr = new QGSLMatrix(engine);
   } else if (ctx->argumentCount() == 1 && ctx->argument(0).isArray()) {
      auto arg = ctx->argument(0);
      QVariantList variantList;      
      qScriptValueToSequence(arg, variantList);
      ptr = new QGSLMatrix(variantList);            
   } else if (ctx->argumentCount() == 2
	      && ctx->argument(0).isNumber()
	      && ctx->argument(1).isNumber()) {
      auto sizei = ctx->argument(0).toUInt32();
      auto sizej = ctx->argument(1).toUInt32();
      ptr = new QGSLMatrix(sizei, sizej);
   } else {
      ctx->throwError (QScriptContext::SyntaxError, "Illegal arguments");
      return engine->undefinedValue();
   }
 
   return engine->newQObject (ptr, QScriptEngine::ScriptOwnership);
}

static QScriptValue createVector (QScriptContext *ctx, QScriptEngine *engine)
{
   QGSLVector *ptr = nullptr;
   if (ctx->argumentCount() == 0) {
      ptr = new QGSLVector(engine);
   } else if (ctx->argumentCount() == 1 && ctx->argument(0).isArray()) {
      auto arg = ctx->argument(0);
      QVariantList variantList;      
      qScriptValueToSequence(arg, variantList);
      ptr = new QGSLVector(variantList);            
   } else if (ctx->argumentCount() == 1
	      && ctx->argument(0).isNumber()) {
      auto sizei = ctx->argument(0).toUInt32();
      ptr = new QGSLVector(sizei);
   } else {
      ctx->throwError (QScriptContext::SyntaxError, "Illegal arguments");
      return engine->undefinedValue();
   }
 
   return engine->newQObject (ptr, QScriptEngine::ScriptOwnership);
}

void GSLPlugin :: registerPlugin(QScriptEngine *engine)
{
   Q_CHECK_PTR(engine);
   auto globalObj = engine->globalObject();

   auto gslconst = new QGSLConstants();
   auto gsl = globalObj.engine()->newQObject(gslconst, QScriptEngine::ScriptOwnership);
   globalObj.setProperty("gsl", gsl);

   registerPrototype<QGSLLinearRegressionPrototype, QGSLLinearRegression> (engine);
   registerPrototype<QGSLPolynomialsPrototype, QGSLPolynomials> (engine);
   registerPrototype<QGSLSpecialPrototype, QGSLSpecial> (engine);
   registerClass<QGSLStatistics>(gsl, "stats", gslconst);
   registerClass<QGSLLinearRegression>(gsl, "fit", gslconst);
   registerClass<QGSLPolynomials>(gsl, "poly", gslconst);
   registerClass<QGSLSpecial>(gsl, "sf", gslconst);

   registerConstructor<QGSLVector>(engine, gsl, "Vector", createVector);
   registerConstructor<QGSLMatrix>(engine, gsl, "Matrix", createMatrix);
}

