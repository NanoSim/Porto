#include "qmongogridfsfileopt.h"
#include "qmongogridfsfile.h"
#include "qbson.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

using SOFT_NAMESPACE::BSON_NAMESPACE::Bson;

class GridFSFileOpt :: Private
{
  friend class GridFSFileOpt;
  Private(const char* md5,
	  const char* filename,
	  const char* contentType,
	  const Bson& aliases,
	  const Bson& metadata,
	  quint32 chunkSize)
    : md5(md5)
    , filename(filename)
    , contentType(contentType)
    , aliases(aliases)
    , metadata(metadata)
    , chunkSize(chunkSize)
    , opts(new mongoc_gridfs_file_opt_t)
  {}

  const char* md5;
  const char* filename;
  const char* contentType;
  bson::Bson aliases;
  bson::Bson metadata;
  quint32 chunkSize;
  
  mongoc_gridfs_file_opt_t* opts;

};

GridFSFileOpt :: GridFSFileOpt(const char* md5,
                               const char* filename,
			       const char* contentType,
			       const bson::Bson& aliases,
			       const bson::Bson& metadata,
			       quint32 chunkSize)
  : d(new Private(md5, filename, contentType, aliases, metadata, chunkSize))
{
}

GridFSFileOpt :: GridFSFileOpt (GridFSFileOpt const & other)
  : QObject (other.parent())
{}

GridFSFileOpt :: GridFSFileOpt (GridFSFileOpt && other)
  : QObject (std::move (other.parent()))
{}

GridFSFileOpt :: ~GridFSFileOpt ()
{
}

mongoc_gridfs_file_opt_t* GridFSFileOpt :: data()
{
  d->opts->md5          = d->md5;
  d->opts->filename     = d->filename;
  d->opts->content_type = d->contentType;
  d->opts->aliases      = d->aliases.data();
  d->opts->metadata     = d->metadata.data();
  d->opts->chunk_size   = d->chunkSize;
  return d->opts;
}

QString GridFSFileOpt :: md5() const
{
  return QString(d->md5);
}

QString GridFSFileOpt :: filename() const
{
  return QString(d->filename);
}

QString GridFSFileOpt :: contentType() const
{
  return QString(d->contentType);
}


bson::Bson GridFSFileOpt :: aliases() const
{
  return d->aliases;
}

bson::Bson GridFSFileOpt :: metadata() const
{
  return d->metadata;
}

quint32 GridFSFileOpt :: chunkSize() const
{
  return d->chunkSize;
}


MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
