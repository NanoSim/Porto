#include <QTextStream>
#include <QEventLoop>
#include <QTimer>

#if defined (WIN32) || defined(__MINGW32__)
#define NOSOCKET

#include <QSocketNotifier>
#include <unistd.h>
#endif

#include "prompt.h"

class Prompt :: Private
{
  friend class Prompt;

  Private()
#if !defined(NOSOCKET)
    : notifier(new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read))
#endif
  {}

  ~Private()
  {
#if !defined(NOSOCKET)
    delete notifier;
#endif
  }
  
#if !defined(NOSOCKET)
  QSocketNotifier *notifier;
#endif
  
  QString buffer;
};

Prompt :: Prompt (QObject *parent)
  : QObject (parent)
  , d (new Prompt::Private)
{
#if !defined(NOSOCKET)
  connect(d->notifier, SIGNAL(activated(int)), this, SLOT(read()));
#endif
}

void Prompt :: start()
{
  QTextStream(stdout) << "ParaShell v0.1" << endl;
  QTextStream(stdout) << "\n> ";

#if !defined(NOSOCKET)
  read();
#else
  QEventLoop loop;
  QTimer timer;
  connect(&timer, SIGNAL(timeout()), this, SLOT(read()), Qt::DirectConnection);
  timer.start(100);
  loop.exec();
#endif
}

Prompt :: ~Prompt()
{
  delete d;
}

QString Prompt :: getMessage()
{
  return d->buffer;
}

void Prompt :: read()
{
  QTextStream qin (stdin);
  d->buffer = qin.readLine();
  emit ready(d->buffer);
}
