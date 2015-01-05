#include "qmongoreadprefs.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

static void readPrefsDeleter(mongoc_read_prefs_t *ptr)
{
  mongoc_read_prefs_destroy (ptr);
}

ReadPrefs :: ReadPrefs (QObject * parent)
   : QObject (parent)
{}

ReadPrefs :: ReadPrefs (mongoc_read_prefs_t const * readPrefs, QObject * parent)
  : QObject (parent)
  , readPrefs (mongoc_read_prefs_copy (readPrefs), 
	       readPrefsDeleter)
{}

ReadPrefs :: ReadPrefs (ReadPrefs const & other)
  : QObject (other.parent())
  , readPrefs (other.readPrefs)
{}

ReadPrefs :: ReadPrefs (ReadPrefs && other)
  : QObject (std::move (other.parent()))
  , readPrefs (std::move (other.readPrefs))
{
}

ReadPrefs :: ~ReadPrefs()
{}

mongoc_read_prefs_t * ReadPrefs :: data() const
{
   return readPrefs.get();
}

ReadPrefs::ReadMode ReadPrefs :: mode () const
{
   return (ReadMode)mongoc_read_prefs_get_mode
      (static_cast <const mongoc_read_prefs_t*> 
       (readPrefs.get()));       
}

void ReadPrefs :: setMode (const ReadMode &readMode)
{
  mongoc_read_prefs_set_mode
    (readPrefs.get(),
     (mongoc_read_mode_t)readMode);
}


MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
