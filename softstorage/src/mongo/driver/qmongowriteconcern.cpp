#include "qmongowriteconcern.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

static void writeConcernDeleter(mongoc_write_concern_t *ptr)
{
  mongoc_write_concern_destroy (ptr);
}

WriteConcern :: WriteConcern (QObject * parent)
   : QObject (parent)
   , writeConcern (
		   mongoc_write_concern_new (),
		   writeConcernDeleter)
{}

WriteConcern :: WriteConcern (mongoc_write_concern_t const * writeConcern, QObject * parent)
  : QObject (parent)
  , writeConcern (
     mongoc_write_concern_copy (writeConcern), 
     writeConcernDeleter)
{}

WriteConcern :: WriteConcern (WriteConcern const & other)
  : QObject (other.parent())
  , writeConcern (other.writeConcern)
{}

WriteConcern :: WriteConcern (WriteConcern && other)
  : QObject (std::move (other.parent()))
  , writeConcern (std::move (other.writeConcern))
{
}

WriteConcern :: ~WriteConcern()
{}

mongoc_write_concern_t * WriteConcern :: data() const
{
   return writeConcern.get();
}

qint32 WriteConcern :: w () const
{
   return mongoc_write_concern_get_w (writeConcern.get());
}

bool WriteConcern :: setW (qint32 w)
{
   mongoc_write_concern_set_w ( writeConcern.get(), w );
   return true;
}

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
