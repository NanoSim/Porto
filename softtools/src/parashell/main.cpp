#include <QtCore/QCoreApplication>
#include <QtCore/QScopedPointer>
#include <QtCore/QThread>
#include <QtDebug>

#include "prompt.h"
#include "console.h"

int main (int argc, char **argv)
{
  QCoreApplication app(argc, argv);

  QScopedPointer<QThread> readerThread (new QThread);
  QScopedPointer<QThread> consoleThread (new QThread);
  QScopedPointer<Prompt> prompt (new Prompt);
  QScopedPointer<Console> console (new Console);

  console->moveToThread(consoleThread.data());
  prompt->moveToThread(readerThread.data());

  QObject::connect(readerThread.data(), &QThread::started, prompt.data(), &Prompt::start);
  QObject::connect(prompt.data(), &Prompt::ready, console.data(), &Console::write);

  consoleThread->start();
  readerThread->start();

  return app.exec();
}
