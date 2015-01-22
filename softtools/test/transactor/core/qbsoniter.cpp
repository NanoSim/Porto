#include <QtDebug>
#include <mongoc.h>
#include "qbson.h"
#include "qbsoniter.h"
#include "qbsonvalue.h"

NAMESPACE_SOFT_BEGIN

class QBsonIter::Private
{
  friend class QBsonIter;
  Private ()
  {}
  
  Private (QBson const &doc)   
  {
    bson_iter_init(&iter, reinterpret_cast<const bson_t*>(doc.bson()));
  }

  bson_iter_t iter;
};

QBsonIter :: QBsonIter (QObject *parent)
  : QObject (parent)
  , d (new QBsonIter::Private())
{}

QBsonIter :: QBsonIter (QBson const &bson, QObject *parent)
  : QObject (parent)
  , d (new QBsonIter::Private (bson))
{}

QBsonIter :: ~QBsonIter()
{
  delete d;
}

bool QBsonIter :: find (QString const &key)
{
  return bson_iter_find (&d->iter, qPrintable(key));
}

bool QBsonIter :: recurse (QBsonIter &child)
{
  return bson_iter_recurse (&d->iter, &child.d->iter);
}
  
bool QBsonIter :: recurse (std::function<void(QBsonIter&)> fn)
{
  QBsonIter child;
  auto retval = recurse (child);
  fn (child);
  return retval;
}

bool QBsonIter :: next ()
{
  return bson_iter_next (&d->iter);
}

QString QBsonIter :: key () const
{
  return QString::fromUtf8(bson_iter_key (&d->iter));
}


QBsonValue QBsonIter:: value () const
{
  auto bsonValue = bson_iter_value (&d->iter);
  QBsonValue v(reinterpret_cast<const _bson_value_t *>(bsonValue));
  return v;
}

NAMESPACE_SOFT_END
