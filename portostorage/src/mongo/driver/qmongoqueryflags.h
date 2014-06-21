#pragma once

#include <QObject>
#include <mongoc.h>

class QMongoQueryFlags : public QObject
{
  Q_OBJECT
  Q_ENUMS(QueryFlags)
public:
  explicit QMongoQueryFlags (QObject *parent = 0);
  virtual ~QMongoQueryFlags();

  enum QueryFlags {
    None = MONGOC_QUERY_NONE,
    TailableCursor = MONGOC_QUERY_TAILABLE_CURSOR,
    SlaveOk = MONGOC_QUERY_SLAVE_OK,
    OplogReplay = MONGOC_QUERY_OPLOG_REPLAY,
    NoCursorTimeout = MONGOC_QUERY_NO_CURSOR_TIMEOUT,
    AwaitData = MONGOC_QUERY_AWAIT_DATA,
    Exhaust = MONGOC_QUERY_EXHAUST,
    Partial = MONGOC_QUERY_PARTIAL
  };
};
