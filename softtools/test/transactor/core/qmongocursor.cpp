#include <mongoc.h>
#include "qmongocursor.h"

NAMESPACE_SOFT_BEGIN

struct CursorDeleter
{
  static inline void cleanup (mongoc_cursor_t *ptr)
  {
    mongoc_cursor_destroy (ptr);
  }
}; // struct CursorDeleter
  
class QMongoCursor :: Private
{
  friend class QMongoCursor;

  Private (mongoc_cursor_t *cursor = nullptr)
    : cursor (cursor)
  {}

  QScopedPointer <mongoc_cursor_t, CursorDeleter> cursor;
}; // class QMongoCursor::Private

QMongoCursor :: QMongoCursor (QObject *parent)
  : QObject (parent)
  , d (q_check_ptr(new QMongoCursor::Private()))
{}

QMongoCursor :: QMongoCursor (_mongoc_cursor_t *cursor, QObject *parent)
  : QObject (parent)
  , d (q_check_ptr(new QMongoCursor::Private(cursor)))
{}

QMongoCursor :: ~QMongoCursor()
{
  delete d;
}

_mongoc_cursor_t *QMongoCursor ::cursor() const
{
  return d->cursor.data();
}

NAMESPACE_SOFT_END
