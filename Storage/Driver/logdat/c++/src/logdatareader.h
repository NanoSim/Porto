#ifndef LOGDATAREADER_H_DEF
#define LOGDATAREADER_H_DEF

#include <QString>
#include <QPair>
#include "logdata.h"

class LogDataReader
{
  QString const filename;
  
public:
  LogDataReader(QString const &filename);
  virtual ~LogDataReader();
  LogData readLogData() const;

private:
  QPair<QString,QString> keywordValuePair(QString const & buffer) const;
  QString getValue(QString const & buffer, QString const &keyword) const;
  int getProcessor(QString const & buffer) const;
  QString getVectorDataName(QString const & buffer) const;
  QString getScalarDataName(QString const & buffer) const;
}; // class LogDataReader

#endif // LOGDATAREADER_H_DEF
