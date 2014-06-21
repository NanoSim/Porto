#pragma once

#include <QObject>
#include <memory>
#include <mongoc.h>
#include "mongons.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

class Client;
class ClientPool : public QObject
{
   Q_OBJECT

public:
   ClientPool (QObject * parent = nullptr);
   ClientPool (char const *uri, QObject * parent = nullptr);
   ClientPool (ClientPool const & other);
   ClientPool (ClientPool && other);

   Client *pop ();
   void push (Client *client);
   Client *tryPop ();

private:
   std::shared_ptr<mongoc_client_pool_t> clientPool;
}; // class ClientPool

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE
