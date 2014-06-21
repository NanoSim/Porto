#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE


class GridFS;
class GridFSFileOpt;

class GridFSFile : public QObject
{
   Q_OBJECT

public:
   GridFSFile (QObject * parent = nullptr);
   GridFSFile (mongoc_gridfs_file_t *gridfsfile, QObject * parent = nullptr);
   GridFSFile (GridFSFile const & other);
   GridFSFile (GridFSFile && other);
   
   GridFSFile (GridFS* gridfs, GridFSFileOpt* opts);
   
   virtual ~GridFSFile();
   
   bool save();
   QString md5() const;
   QString filename() const;
   
// N.B.: pass const char* instead of QString since the lifetime
// of the return value of qPrintable() values is somewhat unclear
   bool setMd5(const char* md5);

private:
   std::shared_ptr<mongoc_gridfs_file_t> gridFSFile;
}; // class GridFSFile

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE

