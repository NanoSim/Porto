#ifndef PROCESSPROTOTYPE_H_DEF
#define PROCESSPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>
#include <QString>
#include <QStringList>

class ProcessPrototype : public QObject, 
			 public QScriptable
{
   Q_OBJECT
public:
   explicit ProcessPrototype(QObject *parent = nullptr);
   virtual ~ProcessPrototype();

public slots:
   QScriptValue execute(QString const & program);
   QScriptValue execute(QString const & program, QStringList const & args);
   QScriptValue arguments() const;
   QScriptValue setArguments(QStringList const & arguments);
   QScriptValue program() const;
   QScriptValue setProgram(QString const & program);
   QScriptValue start();
   QScriptValue readAllStandardError();
   QScriptValue readAllStandardOutput();
   QScriptValue waitForStarted(int msecs = 30000);
   QScriptValue waitForFinished(int msecs = 30000);
   QScriptValue kill();
   QScriptValue isRunning();
   QScriptValue isNotRunning();
   QScriptValue isStarting();
   QScriptValue write(QString const & buffer);
   QScriptValue writeln(QString const & buffer);

signals:
   void finished(int);
   void readyReadStandardError();
   void readyReadStandardOutput();
   void started();

}; // class ProcessPrototype

#endif // PROCESSPROTOTYPE_H_DEF
