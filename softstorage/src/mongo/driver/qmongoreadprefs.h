#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class ReadPrefs : public QObject
{
   Q_OBJECT

public:
   enum ReadMode {
      Primary           = MONGOC_READ_PRIMARY,
      Seconday          = MONGOC_READ_SECONDARY,
      PrimaryPreferred  = MONGOC_READ_PRIMARY_PREFERRED,
      SecondayPreferred = MONGOC_READ_SECONDARY_PREFERRED,
      Nearest           = MONGOC_READ_NEAREST
   };
   
   ReadPrefs (QObject * parent = nullptr);
   ReadPrefs (mongoc_read_prefs_t const * prefs, QObject * parent = nullptr);
   ReadPrefs (ReadPrefs const & other);
   ReadPrefs (ReadPrefs && other);
   virtual ~ReadPrefs();

   ReadPrefs::ReadMode mode () const;
   void setMode (const ReadMode &readMode);
   
protected:
   mongoc_read_prefs_t *data() const;
private:
   friend class Client;
   friend class Collection;
   std::shared_ptr<mongoc_read_prefs_t> readPrefs;
}; // class ReadPrefs

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
