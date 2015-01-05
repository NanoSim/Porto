#ifndef LOGDATA_H_DEF
#define LOGDATA_H_DEF

#include <QString>
#include <QVector>

struct LogData {
  unsigned int               processor;
  QString                    vectorDataName;
  QString                    scalarDataName;
  QVector< unsigned int>     index;
  QVector< double>           time;  
  QVector< QVector<double> > vectorData;
  QVector< double>           scalarData;
  QVector< QVector<double> > position;
};


#endif // LOGDATA_H_DEF
