#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"
#include "qbson.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

using porto::bson::Bson;
class Cursor : public QObject
{
   Q_OBJECT

public:
   Cursor (QObject * parent = nullptr);
   Cursor (mongoc_cursor_t * crsr, QObject * parent = nullptr);
   Cursor (Cursor const & other);
   Cursor (Cursor && other);
   ~Cursor();

   //void getLastError();
   //void getHost();
   bool more() const;
   bool error() const;
   bool isAlive() const;
   Bson *next();
   Cursor *clone();
   Bson *current();
   void destroy();
   
   
private:
   std::shared_ptr<mongoc_cursor_t> cursor;
}; // class Cursor

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE

