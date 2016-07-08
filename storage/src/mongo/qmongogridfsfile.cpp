#include "qmongogridfsfile.h"
#include "qmongogridfs.h"
#include "qmongogridfsfileopt.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE


// static mongoc_gridfs_file_opt_t stdopt = {NULL, NULL, NULL, NULL, NULL, 0};

static void gridFSFileDeleter (mongoc_gridfs_file_t *ptr)
{
   mongoc_gridfs_file_destroy (ptr);
}

GridFSFile :: GridFSFile (QObject * parent)
   : QObject (parent)
{}

GridFSFile :: GridFSFile (mongoc_gridfs_file_t *gridfsfile, QObject * parent)
   : QObject (parent)
   , gridFSFile(gridfsfile, gridFSFileDeleter)
{}

GridFSFile :: GridFSFile (GridFSFile const & other)
   : QObject (other.parent())
   , gridFSFile (other.gridFSFile)
{}

GridFSFile :: GridFSFile (GridFSFile && other)
   : QObject (std::move (other.parent()))
   , gridFSFile (std::move (other.gridFSFile))
{}

GridFSFile :: ~GridFSFile ()
{}

GridFSFile :: GridFSFile(GridFS* gridfs, GridFSFileOpt* opts)
  : gridFSFile(mongoc_gridfs_create_file (gridfs->gridfs.get(),
                                          opts->data()),
               gridFSFileDeleter)
{
}

bool GridFSFile :: save()
{
   return mongoc_gridfs_file_save( gridFSFile.get() );
}

QString GridFSFile :: filename() const
{
   return QString(mongoc_gridfs_file_get_filename (gridFSFile.get()));
}

QString GridFSFile :: md5() const
{
   return QString(mongoc_gridfs_file_get_md5 (gridFSFile.get()));
}

bool GridFSFile :: setMd5(const char* md5)
{
   mongoc_gridfs_file_set_md5 (gridFSFile.get(), md5);
   return true;
}


MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
