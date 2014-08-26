#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class WriteConcern : public QObject
{
   Q_OBJECT

public:
   WriteConcern (QObject * parent = nullptr);
   WriteConcern (mongoc_write_concern_t const * writeconcern, QObject * parent = nullptr);
   WriteConcern (WriteConcern const & other);
   WriteConcern (WriteConcern && other);
   virtual ~WriteConcern();
   
   qint32 w() const;
   bool setW(qint32 w);

protected:
   mongoc_write_concern_t * data() const;

private:
   friend class Client;
   friend class Collection;
   std::shared_ptr<mongoc_write_concern_t> writeConcern;
}; // class WriteConcern

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE
