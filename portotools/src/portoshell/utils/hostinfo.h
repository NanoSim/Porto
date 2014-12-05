#ifndef HOSTINFO_H_DEF
#define HOSTINFO_H_DEF

#include <QObject>

class QScriptEngine;
class HostInfo : public QObject
{
   Q_OBJECT
public:
   explicit HostInfo (QScriptEngine *engine);
   virtual ~HostInfo();

private:
   HostInfo (QObject *parent = nullptr);
};

#endif // HOSTINFO_H_DEF
