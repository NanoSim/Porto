#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class GridFS : public QObject
{
   Q_OBJECT

public:
   explicit GridFS (QObject * parent = nullptr);
   GridFS (mongoc_gridfs_t * gridfs, QObject * parent = nullptr);
   GridFS (GridFS const & other);
   GridFS (GridFS && other);
   virtual ~GridFS();
   
   bool drop();

   friend class GridFSFile;
private:
   std::shared_ptr<mongoc_gridfs_t> gridfs;
};

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE
