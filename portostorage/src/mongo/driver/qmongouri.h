#pragma once

#include <QObject>
#include <QStringList>
#include <memory>
#include <mongoc.h>
#include "mongons.h"
#include "qbson.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class WriteConcern;
class Uri : public QObject
{
   Q_OBJECT

public:
   Uri (QString const & uriStr);
   Uri (QObject * parent = nullptr);
   Uri (mongoc_uri_t *uri, QObject * parent = nullptr);
   Uri (Uri const & other);
   Uri (Uri && other);
   virtual ~Uri();

   bool ssl() const;
   Uri *copy () const;
   QString database() const;
   QString username() const;
   QString password() const;
   QString string() const;
   QStringList hosts() const;
   porto::bson::Bson options() const;
   WriteConcern *writeConcern() const;

private:
   std::shared_ptr<mongoc_uri_t> uri;
}; // class Uri

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE
