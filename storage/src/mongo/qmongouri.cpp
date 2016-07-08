#include <QStringList>
#include "qmongouri.h"
#include "qmongowriteconcern.h"

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

using soft::bson::Bson;

static void uriDeleter(mongoc_uri_t *ptr)
{
  mongoc_uri_destroy (ptr);
}

Uri :: Uri (QString const & uriStr)
  : uri (mongoc_uri_new(qPrintable(uriStr)), uriDeleter)
{
}

Uri :: Uri (QObject * parent)
   : QObject (parent)
{}

Uri :: Uri (mongoc_uri_t * uri, QObject * parent)
  : QObject (parent)
  , uri (uri, uriDeleter)
{}

Uri :: Uri (Uri const & other)
  : QObject (other.parent())
  , uri (other.uri)
{}

Uri :: Uri (Uri && other)
  : QObject (std::move (other.parent()))
  , uri (std::move (other.uri))
{
}

Uri :: ~Uri()
{}

Uri *Uri :: copy() const
{
   auto uriCopy = mongoc_uri_copy (uri.get());
   auto uri = new Uri(uriCopy, this->parent());
   return uri;
}

bool Uri :: ssl() const
{
   return mongoc_uri_get_ssl (uri.get());
}

QString Uri :: database() const
{
   const char* db = mongoc_uri_get_database(uri.get());
   return QString(db);
}

QString Uri :: string() const
{
   const char* str = mongoc_uri_get_string(uri.get());
   return QString(str);
}


QStringList Uri :: hosts() const
{
   mongoc_host_list_t* list  = const_cast<mongoc_host_list_t*>(mongoc_uri_get_hosts(uri.get()));
   QStringList ret;
   while(list)
   {
      ret.append(QString(list->host));
      list = list->next;
   }
   return ret;
}

Bson Uri :: options() const
{
   Bson options(const_cast<bson_t*>(mongoc_uri_get_options(uri.get())));
   return options;
}

QString Uri :: username() const
{
   const char* str = mongoc_uri_get_username(uri.get());
   return QString::fromLatin1(str);
}

QString Uri :: password() const
{
   const char* str = mongoc_uri_get_password(uri.get());
   return QString::fromLatin1(str);
}

WriteConcern *Uri :: writeConcern() const
{
   auto cls = mongoc_uri_get_write_concern (uri.get());
   auto ret = new WriteConcern (cls);
   return ret;
}

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
