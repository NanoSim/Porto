#include "qmongogridfs.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

static void gridfsDeleter(mongoc_gridfs_t *ptr)
{
  mongoc_gridfs_destroy (ptr);
}

GridFS :: GridFS (QObject * parent)
   : QObject (parent)
{}

GridFS :: GridFS (mongoc_gridfs_t * gridfs, QObject * parent)
  : QObject (parent)
  , gridfs (gridfs, gridfsDeleter)
{}

GridFS :: GridFS (GridFS const & other)
  : QObject (other.parent())
  , gridfs (other.gridfs)
{}

GridFS :: GridFS (GridFS && other)
  : QObject (std::move (other.parent()))
  , gridfs (std::move (other.gridfs))
{
}

GridFS :: ~GridFS()
{}

bool GridFS :: drop()
{
  bson_error_t error;
  auto ret = mongoc_gridfs_drop(gridfs.get(), &error);
  return ret;
}

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE
