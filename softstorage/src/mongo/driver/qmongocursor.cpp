#include "qmongocursor.h"
#include <iostream>

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

using porto::bson::Bson;

static void cursorDeleter(mongoc_cursor_t *ptr)
{
   mongoc_cursor_destroy(ptr);
}

Cursor :: Cursor (QObject * parent)
   : QObject (parent)
{}

Cursor :: Cursor (mongoc_cursor_t * crsr, QObject * parent)
   : QObject (parent)
   , cursor(crsr, cursorDeleter)
{}

Cursor :: Cursor (Cursor const & other)
   : QObject (other.parent())
   , cursor (other.cursor)
{}

Cursor :: Cursor (Cursor && other)
   : QObject (std::move (other.parent()))
   , cursor (std::move (other.cursor))
{}

Cursor :: ~Cursor()
{}

bool Cursor :: more() const
{
   auto isMore = mongoc_cursor_more (cursor.get());
   return isMore;
}

bool Cursor :: error() const
{
   bson_error_t err;
   auto ret = mongoc_cursor_error (cursor.get(), &err);
   return ret;
}

bool Cursor :: isAlive() const
{
   auto ret = mongoc_cursor_is_alive (cursor.get());
   return ret;
}

Bson *Cursor :: next()
{
   const bson_t *doc;
   if (mongoc_cursor_next(cursor.get(), &doc)) {
      return new Bson(bson_copy(doc), this);
   }

   return nullptr;
}

Cursor *Cursor :: clone()
{
   mongoc_cursor_t * c = mongoc_cursor_clone (cursor.get());
   return new Cursor(c);
}

Bson *Cursor :: current()
{
   bson_t* b = const_cast<bson_t*>(mongoc_cursor_current (cursor.get()));
   if(b)
      return new Bson(b, this);
   else
      return new Bson(this);
}

void Cursor :: destroy()
{
   mongoc_cursor_destroy(cursor.get());
}

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE
