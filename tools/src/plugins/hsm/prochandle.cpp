#include "prochandle.h"
#include <QTextStream>

ProcHandle :: ProcHandle(QProcess *proc, QObject *parent)
  : QObject (parent)
{
  connect(proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleProcessError(QProcess::ProcessError)));
  connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(handleExit(int, QProcess::ExitStatus)));
}

ProcHandle :: ~ProcHandle()
{}

void ProcHandle :: handleExit(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit && exitCode == 0) emit normalExit();
  else emit exitFailure();
}

void ProcHandle :: handleProcessError(QProcess::ProcessError error)
{
  QTextStream(stdout) << "ProcHandle :: handleProcessError" << error << endl;
  switch (error) {
  case QProcess::FailedToStart:
    emit failedToStart();
    break;
  case QProcess::Crashed:
    emit crashed();
    break;
  case QProcess::Timedout:
    emit timedOut();
    break;
  case QProcess::WriteError:
    emit writeError();
    break;
  case QProcess::ReadError:
    emit readError();
  case QProcess::UnknownError:
  default:
    emit unknownError();
  }
}
