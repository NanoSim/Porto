#include <QString>
#include <QFile>
#include <QTextStream>

#include "logdatareader.h"

LogDataReader :: LogDataReader(QString const &fname)
  : filename(fname)
{}

LogDataReader :: ~LogDataReader()
{}


int LogDataReader :: getProcessor(QString const & buffer) const
{  
  return 127;
}

QString LogDataReader :: getVectorDataName(QString const & buffer) const
{
  return QString("vectorData");
}

QString LogDataReader :: getScalarDataName(QString const & buffer) const
{
  return QString("scalarData");
}


LogData LogDataReader :: readLogData() const
{
  QFile fin(filename);
  if(!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream(stderr) << "Failed to open '" << filename << "': "<< fin.errorString() << endl;
    return LogData();
  }

  QTextStream in(&fin);

  // Read header fields
  QString const procline = (!in.atEnd() ? in.readLine() : QString());
  QString const header   = (!in.atEnd() ? in.readLine() : QString());

  LogData logData;
  logData.processor = getProcessor(procline);
  logData.vectorDataName = getVectorDataName(header);
  logData.scalarDataName = getScalarDataName(header);

  while(!in.atEnd()) {
    QString const line = in.readLine();
    
  }
  return logData;
}
