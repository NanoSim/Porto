#include "qmongogridfsfilelist.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

static void gridFSFileListDeleter (mongoc_gridfs_file_list_t *ptr)
{
   mongoc_gridfs_file_list_destroy (ptr);
}

GridFSFileList :: GridFSFileList (QObject * parent)
   : QObject (parent)
{}

GridFSFileList :: GridFSFileList (mongoc_gridfs_file_list_t *gridfsfilelist, QObject * parent)
   : QObject (parent)
   , gridFSFileList(gridfsfilelist, gridFSFileListDeleter)
{}

GridFSFileList :: GridFSFileList (GridFSFileList const & other)
   : QObject (other.parent())
   , gridFSFileList (other.gridFSFileList)
{}

GridFSFileList :: GridFSFileList (GridFSFileList && other)
   : QObject (std::move (other.parent()))
   , gridFSFileList (std::move (other.gridFSFileList))
{}

GridFSFileList :: ~GridFSFileList ()
{}

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
