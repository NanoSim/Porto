#ifndef LOGDATAREADER_H_DEF
#define LOGDATAREADER_H_DEF

#include <QString>
#include "logdata.h"

class LogDataReader
{
  QString const filename;
  
public:
  LogDataReader(QString const &filename);
  virtual ~LogDataReader();
  LogData readLogData() const;

private:
  int getProcessor(QString const & buffer) const;
  QString getVectorDataName(QString const & buffer) const;
  QString getScalarDataName(QString const & buffer) const;
}; // class LogDataReader

#endif // LOGDATAREADER_H_DEF
