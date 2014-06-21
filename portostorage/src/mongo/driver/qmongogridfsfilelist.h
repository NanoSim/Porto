 #pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class GridFSFileList : public QObject
{
   Q_OBJECT

public:
   GridFSFileList (QObject * parent = nullptr);
   GridFSFileList (mongoc_gridfs_file_list_t *gridfsfile, QObject * parent = nullptr);
   GridFSFileList (GridFSFileList const & other);
   GridFSFileList (GridFSFileList && other);
   virtual ~GridFSFileList();

private:
   std::shared_ptr<mongoc_gridfs_file_list_t> gridFSFileList;
}; // class GridFSFileList

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE

Q_DECLARE_METATYPE(PORTO_NAMESPACE::MONGO_NAMESPACE::GridFSFileList*)
Q_DECLARE_METATYPE(PORTO_NAMESPACE::MONGO_NAMESPACE::GridFSFileList)
