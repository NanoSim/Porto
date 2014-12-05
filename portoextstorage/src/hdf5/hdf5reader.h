#ifndef HDF5READER_H_DEF
#define HDF5READER_H_DEF

#include <QString>
#include <QPair>
#include "hdf5.h"

class HDF5Reader
{
  QString const filename;
  
public:
  HDF5Reader(QString const &filename);
  virtual ~HDF5Reader();
  HDF5 readHDF5() const;

private:
  QPair<QString,QString> keywordValuePair(QString const & buffer) const;
  QString getValue(QString const & buffer, QString const &keyword) const;
  int getProcessor(QString const & buffer) const;
  QString getVectorDataName(QString const & buffer) const;
  QString getScalarDataName(QString const & buffer) const;
}; // class HDF5Reader

#endif // HDF5READER_H_DEF
