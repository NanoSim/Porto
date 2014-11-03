#include <QScriptEngine>
#include <QScriptValue>
#include <QTimer>
#include <QtDebug>
#include "async.h"

class Async::Private
{
  friend class Async;
  Private(QScriptEngine *e)
    : engine(e)
  {}

  QList<QScriptValue> functionList;
  QScriptEngine *engine;
  QTimer timer;
};

Async :: Async (QScriptEngine *engine)
  : QObject (engine)
  , d (new Async::Private (engine))
{
  connect(this, SIGNAL(asyncRequest(QScriptValue)), this, SLOT(asyncEval(QScriptValue)), Qt::QueuedConnection);
  engine->setProcessEventsInterval(10);
  connect(&d->timer, SIGNAL(timeout()), this, SLOT(processQueue()));
  d->timer.start(10);
}

Async :: Async (QObject *parent)
  : QObject (parent)
{}

Async :: ~Async()
{}

void Async :: processQueue()
{
  while( !d->functionList.isEmpty() ) {
    auto fn = d->functionList.takeFirst();
    auto res = fn.call(fn, QScriptValueList());
    emit (result (res));
  }
}

void Async :: asyncEval (QScriptValue fn)
{
  if (!fn.isFunction()) {
    d->engine->currentContext()->throwError(QScriptContext::TypeError, "Arguments to Async.eval must be of type 'function'");
    return;
  }
  d->functionList.append(fn);
}

void Async :: eval (QScriptValue fn)
{
  emit (asyncRequest(fn));
}
