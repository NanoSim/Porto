#ifndef UTILS_H_DEF
#define UTILS_H_DEF

#include <QObject>
#include <QScopedPointer>

namespace soft {
   class ScriptEngine;
}

inline void initScriptResource() { Q_INIT_RESOURCE(softshell); }

class Utils : public QObject
{
   Q_OBJECT
public:
   Utils(soft::ScriptEngine const &engine, QObject * parent = nullptr);
   virtual ~Utils();

private:
   class Private;
   QScopedPointer<Private> d;
};

#endif // UTILS_H_DEF
