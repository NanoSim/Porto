#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class Stream : public QObject
{
   Q_OBJECT

public:
   Stream (QObject * parent = nullptr);
   Stream (mongoc_stream_t *stream, QObject * parent = nullptr);
   Stream (Stream const & other);
   Stream (Stream && other);
   virtual ~Stream();

private:
   std::shared_ptr<mongoc_stream_t> stream;
}; // class Stream

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE

