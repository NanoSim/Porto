#ifndef PROCHANDLE_H_DEF
#define PROCHANDLE_H_DEF

#include <QObject>
#include <QProcess>

class ProcHandle : public QObject
{
  Q_OBJECT
public:
  explicit ProcHandle(QProcess *proc, QObject *parent = NULL);
  virtual ~ProcHandle();

protected slots:
  void handleExit(int exitCode, QProcess::ExitStatus);
  void handleProcessError(QProcess::ProcessError);

signals:
  void crashed();
  void failedToStart();
  void timedOut();
  void writeError();
  void readError();
  void unknownError();
  void normalExit();
  void exitFailure();
};

#endif // PROCHANDLE_H_DEF
