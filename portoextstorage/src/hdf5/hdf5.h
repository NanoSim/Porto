#ifndef HDF5_H_DEF
#define HDF5_H_DEF

#include <QString>
#include <QVector>

struct HDF5 {
  unsigned int               processor;
  QString                    vectorDataName;
  QString                    scalarDataName;
  QVector< unsigned int>     index;
  QVector< double>           time;  
  QVector< QVector<double> > vectorData;
  QVector< double>           scalarData;
  QVector< QVector<double> > position;
};


#endif // HDF5_H_DEF
