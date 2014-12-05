#ifndef PROCESSPROTOTYPE_H_DEF
#define PROCESSPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>
#include <QScriptValue>
#include <QString>
#include <QStringList>
#include <QProcess>

class ProcessPrototype : public QObject, 
			 public QScriptable
{
   Q_OBJECT
   Q_ENUMS(ProcessChannel)
public:
   explicit ProcessPrototype(QObject *parent = nullptr);
   virtual ~ProcessPrototype();

   enum ProcessChannel {StandardOutput = QProcess::StandardOutput,
			StandardError = QProcess::StandardError};
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
   QScriptValue closeReadChannel(ProcessChannel &channel);
   QScriptValue closeWriteChannel();

signals:
   void finished(int, int);
   void finished(int);
   void error(int);
   void readyReadStandardError();
   void readyReadStandardOutput();
   void started();

}; // class ProcessPrototype

#endif // PROCESSPROTOTYPE_H_DEF
