#ifndef SCRIPTPLUGINS_H_DEF
#define SCRIPTPLUGINS_H_DEF

#include <QObject>
#include <QScopedPointer>

namespace soft { 
   class ScriptEngine;
}
class QScriptEngine;
class ScriptPlugins : public QObject
{
   Q_OBJECT

public:
   ScriptPlugins(soft::ScriptEngine const &engine, QObject * parent = 0);
   virtual ~ScriptPlugins();

protected:
   void load(QScriptEngine *engine);

private:
   class Private;
   QScopedPointer<Private> d;
};

#endif // SCRIPTPLUGINS_H_DEF
