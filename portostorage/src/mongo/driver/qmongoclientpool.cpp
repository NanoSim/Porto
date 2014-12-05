#include "qmongoclientpool.h"
#include "qmongoclient.h"

PORTO_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

static void uriDeleter (mongoc_uri_t * ptr)
{
   mongoc_uri_destroy (ptr);
}

static void clientPoolDeleter (mongoc_client_pool_t * ptr)
{
   mongoc_client_pool_destroy (ptr);
}
  
ClientPool :: ClientPool (QObject * parent)
   : QObject (parent)
{}

ClientPool :: ClientPool (const char *uri, QObject * parent)
   : QObject (parent)
     
{
   std::shared_ptr<mongoc_uri_t> auto_uri (
      mongoc_uri_new(uri), 
      uriDeleter);
   
   auto *pool = mongoc_client_pool_new ( auto_uri.get() );
   std::shared_ptr <mongoc_client_pool_t> newPool (pool, clientPoolDeleter);
   clientPool.swap (newPool);
}

ClientPool :: ClientPool (ClientPool const & other)
   : QObject (other.parent())
   , clientPool (other.clientPool)
{}

ClientPool :: ClientPool (ClientPool && other)
   : QObject (std::move (other.parent()))
   , clientPool (std::move (other.clientPool))
{}

Client *ClientPool :: pop ()
{
   mongoc_client_t * c = mongoc_client_pool_pop (clientPool.get());
   auto client = new Client(c, this);
   return client;
}

void ClientPool :: push (Client *client)
{
   mongoc_client_pool_push (clientPool.get(), 
                            client->data());
}

Client *ClientPool :: tryPop ()
{
   auto c = mongoc_client_pool_try_pop (clientPool.get());
   auto client = new Client(c);
   return client;
}

MONGO_END_NAMESPACE
PORTO_END_NAMESPACE
