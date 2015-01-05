#ifndef BASE64_H_DEF
#define BASE64_H_DEF

#include <QObject>

class QScriptEngine;
class Base64 : public QObject
{
   Q_OBJECT
public:
   explicit Base64 (QScriptEngine *engine);
   virtual ~Base64();

private:
   Base64 (QObject *parent = nullptr);
};

#endif // BASE64_H_DEF
