 #pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"
#include "qbson.h"
#include "qmongogridfsfileopt.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE
class GridFSFile;

class GridFSFileOpt : public QObject
{
   Q_OBJECT

public:
// N.B.: pass const char* instead of QString since the lifetime
// of the return value of qPrintable() values is somewhat unclear
   GridFSFileOpt(const char* md5 = nullptr,
                 const char* filename = nullptr,
                 const char* contentType = nullptr,
                 const PORTO_NAMESPACE::BSON_NAMESPACE::Bson& aliases =  PORTO_NAMESPACE::BSON_NAMESPACE::Bson(),
                 const PORTO_NAMESPACE::BSON_NAMESPACE::Bson& metadata = PORTO_NAMESPACE::BSON_NAMESPACE::Bson(),
                 quint32 chunkSize = 0);
   GridFSFileOpt (GridFSFileOpt const & other);
   GridFSFileOpt (GridFSFileOpt && other);
   virtual ~GridFSFileOpt();
   
   QString md5() const;
   QString filename() const;
   QString contentType() const;
   bson::Bson aliases() const;
   bson::Bson metadata() const;
   quint32 chunkSize() const;

protected:
     mongoc_gridfs_file_opt_t* data();

private:
  friend class GridFSFile;
  class Private;
  QScopedPointer<Private> d;
  
}; // class GridFSFileOpt

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE

