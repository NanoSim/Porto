#include <gtest/gtest.h>
#include <iostream>
#include <mongoc.h>

#include "qmongo"

using soft::mongo::Client;
using namespace soft;
using bson::Bson;
using mongo::Uri;

class MongoTest : public ::testing::Test {
protected:
   static void SetUpTestCase() {
      mongo::initialize();
      client = new mongo::Client("mongodb://localhost");
   }

   static void TearDownTestCase() {
   }

   virtual void SetUp() {
   }

   virtual void TearDown() {
   }

  static mongo::Client *client;
};

mongo::Client * MongoTest::client = nullptr;

TEST_F(MongoTest, bson_create) 
{
   Bson b1("{\"foo\":\"bar\"}");
   ASSERT_EQ(b1.countKeys(),1);
   ASSERT_TRUE(b1.asString() == "{ \"foo\" : \"bar\" }");
}

TEST_F(MongoTest, mongoclient_create) 
{
   auto collection = client->collection("test", "capi");
   ASSERT_TRUE(collection != nullptr);
}

TEST_F(MongoTest, client_command) 
{
  auto cmd = new Bson("{\"count\":\"foo\"}");
  auto cursor = client->command("test", cmd, nullptr);
  ASSERT_TRUE (cursor != nullptr);
  while ( (!cursor->error()) && cursor->more()) {
    auto next = cursor->next ();
    ASSERT_TRUE (next != nullptr);
    auto str = next->asString();
    std::cout << qPrintable(str);
  }
}

TEST_F(MongoTest, client_commandSimple) 
{
   FAIL();
}

TEST_F(MongoTest, client_collection) 
{
   auto collection = client->collection("test", "foo");
   ASSERT_TRUE (collection != nullptr);

   Bson ins("{\"foo2\":\"bar2\"}");
   Bson query("{\"foo2\":\"bar2\"}");

   collection->insert(&ins);
   ASSERT_EQ(collection->count(&query), 1);
   collection->drop();
   ASSERT_EQ(collection->count(&query), 0);
}

TEST_F(MongoTest, client_database)
{
   auto * database = client->database("soft");
   ASSERT_TRUE (database != nullptr);
}

TEST_F(MongoTest, client_databaseNames) 
{
   auto names = client->databaseNames();
   ASSERT_TRUE(names.size() > 0 && names[0] == "test");
}

TEST_F(MongoTest, client_gridFS) 
{
   auto gridfs = client->gridFS("test", "");
   ASSERT_TRUE(gridfs != nullptr);
}

TEST_F(MongoTest, client_maxBsonSize) 
{
   FAIL();
}

TEST_F(MongoTest, client_readPrefs) 
{
   FAIL();
}

TEST_F(MongoTest, client_serverStatus) 
{
   FAIL();
}

TEST_F(MongoTest, client_uri) 
{
   auto uri = client->uri();
   ASSERT_TRUE(uri->string() == "mongodb://localhost");
}

TEST_F(MongoTest, client_writeConcern) 
{
   auto writeConcern = client->writeConcern();
   ASSERT_TRUE (writeConcern != nullptr);
}

TEST_F(MongoTest, client_setReadPrefs) 
{   
   FAIL();
}

TEST_F(MongoTest, client_setSslOpts) 
{
   FAIL();
}

TEST_F(MongoTest, client_setWriteConcern) 
{
   mongo::WriteConcern writeConcern;
   client->setWriteConcern (&writeConcern);
   
}

TEST_F(MongoTest, clientpool_pop)
{
   FAIL();
}

TEST_F(MongoTest, clientpool_push)
{
   FAIL();
}

TEST_F(MongoTest, clientpool_tryPop)
{
   FAIL();
}

TEST_F(MongoTest, collection_aggregate)
{
   FAIL();
}

TEST_F(MongoTest, collection_command)
{
   FAIL();
}

TEST_F(MongoTest, collection_commandSimple)
{
   FAIL();
}

TEST_F(MongoTest, collection_count)
{
   Bson query("{\"foo\":\"bar\"}");

   auto database = client->database("test");
   database->createCollection("foo");
   
   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != nullptr);
   
   collection->drop();
   ASSERT_TRUE(collection->count(&query) == 0);
 
   Bson ins("{\"foo\":\"bar\"}");

   collection->insert(&ins);
   collection->insert(&ins);
   collection->insert(&ins);
   ASSERT_TRUE(collection->count(&query) == 3);
   collection->drop();
   ASSERT_TRUE(collection->count(&query) == 0);
}

TEST_F(MongoTest, collection_createBulkOperation)
{
   FAIL();
}

TEST_F(MongoTest, collection_createIndex)
{
   auto database = client->database("test");
   database->createCollection("foo");

   Bson index("{\"foo\":1}");

   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != 0);
   bool ok = collection->createIndex( &index );
   ASSERT_TRUE(ok);
}

TEST_F(MongoTest, collection_drop)
{   
   auto database = client->database("test");
   database->createCollection("foo");
   
   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != nullptr);
   
   bool ok = collection->drop();
   ASSERT_TRUE(ok);
   ok = collection->drop();
   ASSERT_FALSE(ok);
}

TEST_F(MongoTest, collection_dropIndex)
{
   FAIL();
}

#if 0
TEST_F(MongoTest, collection_ensureIndex)
{
   auto database = client->database("test");
   database->createCollection("foo");

   Bson index("{\"foo\":1}");

   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != nullptr);
   bool ok = collection->ensureIndex( &index );
   ASSERT_TRUE(ok);
}
#endif

TEST_F(MongoTest, collection_find)
{
   auto database = client->database("test");
   database->createCollection("foo");

   Bson query("{\"foo\":\"bar\"}");
   Bson fields("{}");


   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != 0);
   collection->drop();
   
   // insert objects that mach the query
   collection->insert(&query);
   collection->insert(&query);

   auto cur = collection->find(&query, &fields, 0);
   ASSERT_TRUE (cur != nullptr);
   ASSERT_TRUE (cur->more());
   
   int count = 0;
   
   while((!cur->error()) && cur->more())
   {
      auto next = cur->next();
      if(next)
         count++;
   }
   ASSERT_EQ(count, 2);
}

TEST_F(MongoTest, collection_findAndModify)
{
   FAIL();
}

TEST_F(MongoTest, collection_lastError)
{
   auto * database = client->database("test");
   database->createCollection("foo");

   Bson query("{\"foo\":\"bar\"}");
   Bson sort;
   Bson update("{\"foo\":\"bay\"}");
   Bson fields;
   Bson* reply;
   bool remove = false;
   bool upsert = true;


   auto * collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != 0);
   collection->drop();
   
   // insert objects that mach the update
   auto ok = collection->findAndModify(&query,
                             &sort,
                             &update,
                             &fields,
                             remove,
                             upsert,
                             &reply);

   ASSERT_TRUE( ok );
   auto cur = collection->find(&update, &fields, 0);
   ASSERT_TRUE (cur != nullptr);
   ASSERT_TRUE (cur->more());
   
   int count = 0;
   
   while((!cur->error()) && cur->more())
   {
      auto next = cur->next();
      if(next)
         count++;
   }
   ASSERT_EQ(count, 1);
   delete reply;
}

TEST_F(MongoTest, collection_name)
{  
   auto database = client->database("test");
   database->createCollection("foo");
   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection->name() == "foo");
}

TEST_F(MongoTest, collection_readPrefs)
{
   FAIL();
}

TEST_F(MongoTest, collection_writeConcern)
{
   FAIL();
}

TEST_F(MongoTest, collection_insert)
{
   auto database = client->database("test");
   database->createCollection("foo");

   Bson ins("{\"foo\":\"bar\"}");

   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != 0);
   bool ok = collection->insert(&ins);
   ASSERT_TRUE (ok);
}

TEST_F(MongoTest, collection_insertBulk)
{
   auto database = client->database("test");
   database->createCollection("foo");


   const int N = 10000;
   QVector<Bson*> bulk(N);
   
   for(int i = 0; i < N; ++i)
      bulk[i] = new Bson(QString("{\"foo\":%1}").arg(i));

   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != 0);
   collection->drop();
   bool ok = collection->insertBulk( bulk );
   ASSERT_TRUE (ok);
   
   // !!! the nested queries do not work, they will be translated to simple {}
   
   Bson query1 = Bson("{\"foo\":{$gte:0}}");
   ASSERT_TRUE(collection->count(&query1) == N);
   
   Bson query2 = Bson(QString("{\"foo\":\\{$gte:%1\\}}").arg(N));
   auto cur = collection->find(&query2, nullptr,  N+1);
   int count = 0;
   while((!cur->error()) && cur->more())
   {
      auto next = cur->next();
      if(next)
         count++;
   }
   for(int i = 0; i < N; ++i)
      delete bulk[i];
   ASSERT_EQ(count, N);
}

TEST_F(MongoTest, collection_keysToIndexString)
{
   FAIL();
}

TEST_F(MongoTest, collection_rename)
{
   auto database = client->database("test");
   auto collection = database->collection("foo");
   auto isOk = collection->rename("test2", "foo2", true);

   ASSERT_TRUE (isOk);
   
}

TEST_F(MongoTest, collection_save)
{
  auto database = client->database("test");
  database->createCollection("foo");
  auto collection = client->collection("test", "foo");

  Bson doc("{\"foo\":\"baz\"}");
  bool ok = collection->save(&doc);
  ASSERT_TRUE( ok );
}

TEST_F(MongoTest, collection_setReadPrefs)
{
   FAIL();
}

TEST_F(MongoTest, collection_setWriteConcern)
{
   FAIL();
}

TEST_F(MongoTest, collection_stats)
{
   FAIL();
}

TEST_F(MongoTest, collection_update)
{
   auto database = client->database("test");
   database->createCollection("foo");

   Bson bson("{\"foo\":\"bar\"}");
   Bson upd("{\"foo\":\"baz\"}");
   Bson fields("{}");


   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != 0);
   collection->drop();
   
   collection->insert(&bson);
   collection->update(&bson, &upd);


   auto cur = collection->find(&upd, &fields, 0);
   ASSERT_TRUE (cur != nullptr);
   ASSERT_TRUE (cur->more());
   
   int count = 0;
   
   while((!cur->error()) && cur->more())
   {
      auto next = cur->next();
      if(next)
         count++;
   }
   ASSERT_EQ(count, 1);
}

TEST_F(MongoTest, collection_validate)
{
   auto database = client->database("test");
   database->createCollection("foo");

   Bson ins("{\"foo\":\"bar\"}");

   auto collection = client->collection("test", "foo");
   ASSERT_TRUE(collection != 0);
   bool ok = collection->insert(&ins);
   ASSERT_TRUE (ok);
   Bson query("{}");
   auto reply = collection->validate(&query, ok);
   ASSERT_TRUE (ok);
}

TEST_F(MongoTest, cursor_clone)
{
  FAIL();
}

TEST_F(MongoTest, cursor_current)
{
  auto database = client->database("test");
  database->createCollection("foo");

  Bson query("{\"foo\":\"bar\"}");
  Bson fields("{}");


  auto collection = client->collection("test", "foo");
  ASSERT_TRUE(collection != 0);
  collection->drop();
  
  // insert objects that mach the query
  const int N = 10;
  for(int i = 0; i < N; ++i)
     collection->insert(&query);

  auto cur = collection->find(&query, &fields, 0);
  ASSERT_TRUE (cur != nullptr);
  ASSERT_TRUE (cur->more());
  
  int count = 0;
  
  while((!cur->error()) && cur->more())
  {
     auto next = cur->next();
     auto curr = cur->current( );
     if(next) {
        count++;
        ASSERT_TRUE(next->asString() == curr->asString());
     }
  }
  ASSERT_EQ(count, N);
}

TEST_F(MongoTest, cursor_error)
{
  FAIL();
}

TEST_F(MongoTest, cursor_getHost)
{
  FAIL();
}

TEST_F(MongoTest, cursor_isAlive)
{
  auto database = client->database("test");
  database->createCollection("foo");

  Bson query("{\"foo\":\"bar\""
    ",\"bla\",\"bvu7tvzufdtugjghgsjtu6265uffjf23jhgj2h3gj3g32jg3jgj32gj2gjg2g\"}");  // some trailing data
  Bson fields("{}");


  auto collection = client->collection("test", "foo");
  ASSERT_TRUE(collection != 0);
  collection->drop();
  
  // insert objects that mach the query
  const int N = 101;
  for(int i = 0; i < N; ++i)
     collection->insert(&query);

  auto cur = collection->find(&query, &fields, 0);
  ASSERT_TRUE (cur != nullptr);
  ASSERT_TRUE(cur->isAlive());
  ASSERT_TRUE (cur->more());
  
  int count = 0;
  
  while((!cur->error()) && cur->more())
  {
     auto next = cur->next();
     if(next)
          count++;
  }
  ASSERT_EQ(count, N);
  ASSERT_TRUE(!cur->isAlive());
}

TEST_F(MongoTest, cursor_more)
{
  FAIL();
}

TEST_F(MongoTest, cursor_next)
{
  auto database = client->database("test");
  database->createCollection("foo");

  Bson query("{\"foo\":\"bar\"}");
  Bson fields("{}");


  auto collection = client->collection("test", "foo");
  ASSERT_TRUE(collection != 0);
  collection->drop();
  
  // insert objects that mach the query
  const int N = 101;
  for(int i = 0; i < N; ++i)
     collection->insert(&query);

  auto cur = collection->find(&query, &fields, 0);
  ASSERT_TRUE (cur != nullptr);
  ASSERT_TRUE (cur->more());
  
  int count = 0;
  
  while((!cur->error()) && cur->more())
  {
     auto next = cur->next();
     if(next) {
//	ASSERT_TRUE(next->asString() == "{ \"foo\" : \"bar\" }");
	count++;
     }
  }
  ASSERT_EQ(count, N);
}

TEST_F(MongoTest, database_command)
{
  FAIL();
}

TEST_F(MongoTest, database_simple)
{
  FAIL();
}

TEST_F(MongoTest, database_collection)
{
   auto database = client->database("test");
   auto coll = database->collection("foo");
   ASSERT_TRUE (coll != nullptr);
}

TEST_F(MongoTest, database_collectionNames)
{
   auto database = client->database("test");
   auto names = database->collectionNames();
   ASSERT_TRUE (names.length() > 0);
}

TEST_F(MongoTest, database_name)
{
  auto database = client->database("test");
  ASSERT_TRUE( database->name() == "test" ); 
}

TEST_F(MongoTest, database_readPrefs)
{
  FAIL();
}

TEST_F(MongoTest, database_writeConcern)
{
  FAIL();
}

TEST_F(MongoTest, database_hasCollection)
{
   auto database = client->database("test");
   database->createCollection("baz");
   auto ok = database->hasCollection("baz");
   ASSERT_TRUE(ok);
}

TEST_F(MongoTest, database_setReadPrefs)
{
  FAIL();
}

TEST_F(MongoTest, database_setWriteConcern)
{
  FAIL();
}

TEST_F(MongoTest, database_add_user)
{
   auto database = client->database("test");
   auto ok = database->addUser ("bazinga", "secret");
   ASSERT_TRUE (ok);
}

TEST_F(MongoTest, database_remove_user)
{
   auto database = client->database("test");
   auto ok = database->removeUser ("bazinga");
   ASSERT_TRUE (ok);
}

TEST_F(MongoTest, database_remove_all_users)
{
   auto database = client->database("test");
   ASSERT_TRUE( true == database->addUser ("bazinga", "secret"));
   ASSERT_TRUE( true == database->addUser ("bing", "secret"));
   ASSERT_TRUE( true == database->addUser ("baz", "secret"));
   ASSERT_TRUE( true == database->addUser ("boose", "secret"));
   auto ok = database->removeAllUsers ();
   ASSERT_TRUE (ok);
}

TEST_F(MongoTest, database_drop)
{
  auto database = client->database("a_new_database");
  auto ok = database->drop();
  ASSERT_TRUE (ok);
}

TEST_F(MongoTest, mongoclient_gridfs)
{
  auto gridfs = client->gridFS("soft", "myprefix");
  ASSERT_TRUE (gridfs != nullptr);
}


TEST_F(MongoTest,gridFS_createFile)
{
   auto gridfs = client->gridFS("test", "testprefix");
   ASSERT_TRUE(gridfs != nullptr);
   auto gridfs_file = new mongo::GridFSFile(gridfs);
   ASSERT_TRUE(gridfs_file != nullptr);
}

TEST_F(MongoTest,gridFS_createFileFromStream)
{
  FAIL();
}

TEST_F(MongoTest,gridFS_drop)
{
   auto gridfs = client->gridFS("test", "testprefix");
   ASSERT_TRUE(gridfs != nullptr);
   bool ok = gridfs->drop();
   ASSERT_TRUE( ok );
}

TEST_F(MongoTest,gridFS_find)
{
  FAIL();
}

TEST_F(MongoTest,gridFS_findOne)
{
  FAIL();
}

TEST_F(MongoTest,gridFS_findOneByFilename)
{
  FAIL();
}

TEST_F(MongoTest,gridFS_chunks)
{
  FAIL();
}

TEST_F(MongoTest,gridFS_files)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_error)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_aliases)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_chunk_size)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_content_type)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_filename)
{
   auto gridfs = client->gridFS("test", "testprefix");
   ASSERT_TRUE(gridfs != nullptr);
   
   mongo::GridFSFileOpt gridfsopt(/*md5:*/"1234567890", /*filename:*/"testfile");
   auto gridfs_file = new mongo::GridFSFile(gridfs, &gridfsopt);
   ASSERT_TRUE(gridfs_file != nullptr);
   auto filename = gridfs_file->filename();
   ASSERT_TRUE( filename == "testfile" );
   delete gridfs_file;
}

TEST_F(MongoTest, GridFSFile_length)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_md5)
{
   auto gridfs = client->gridFS("test", "testprefix");
   ASSERT_TRUE(gridfs != nullptr);
   
   mongo::GridFSFileOpt gridfsopt(/*md5:*/"1234567890", /*filename:*/"testfile");
   auto gridfs_file = new mongo::GridFSFile(gridfs, &gridfsopt);
   ASSERT_TRUE(gridfs_file != nullptr);
   auto md5 = gridfs_file->md5();
   ASSERT_TRUE( md5 == "1234567890" );
   delete gridfs_file;
}

TEST_F(MongoTest, GridFSFile_metadata)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_uploadDate)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_readv)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_save)
{
   auto gridfs = client->gridFS("test", "testprefix");
   ASSERT_TRUE(gridfs != nullptr);
   
   mongo::GridFSFileOpt gridfsopt;
   auto gridfs_file = new mongo::GridFSFile(gridfs, &gridfsopt);
   ASSERT_TRUE(gridfs_file != nullptr);
   auto ok = gridfs_file->save();
   ASSERT_TRUE( ok );
   delete gridfs_file;
}

TEST_F(MongoTest, GridFSFile_seek)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_setAliases)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_setContent_type)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_setFilename)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_setMd5)
{
   auto gridfs = client->gridFS("test", "testprefix");
   ASSERT_TRUE(gridfs != nullptr);
   
   mongo::GridFSFileOpt gridfsopt;
   auto gridfs_file = new mongo::GridFSFile(gridfs, &gridfsopt);
   ASSERT_TRUE(gridfs_file != nullptr);
   gridfs_file->setMd5( "12345678" );
   auto md5 = gridfs_file->md5();
   ASSERT_TRUE( md5 == "12345678" );
   delete gridfs_file;
}

TEST_F(MongoTest, GridFSFile_setMetadata)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_tell)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFile_writev)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFileList_error)
{
  FAIL();
}

TEST_F(MongoTest, GridFSFileList_next)
{
  FAIL();
}

TEST_F(MongoTest, ReadPrefs_add_tag)
{
  FAIL();
}

TEST_F(MongoTest, ReadPrefs_copy)
{
  FAIL();
}

TEST_F(MongoTest, ReadPrefs_destroy)
{
  FAIL();
}

TEST_F(MongoTest, ReadPrefs_get_mode)
{
  FAIL();
}

TEST_F(MongoTest, ReadPrefs_get_tags)
{
  FAIL();
}

TEST_F(MongoTest, ReadPrefs_is_valid)
{
  FAIL();
}

TEST_F(MongoTest, ReadPrefs_setMode)
{
  FAIL();
}

TEST_F(MongoTest, ReadPrefs_setTags)
{
  FAIL();
}

TEST_F(MongoTest, Stream_bufferedNew)
{
  FAIL();
}

TEST_F(MongoTest, Stream_close)
{
  FAIL();
}

TEST_F(MongoTest, Stream_cork)
{
  FAIL();
}

TEST_F(MongoTest, Stream_destroy)
{
  FAIL();
}

TEST_F(MongoTest, Stream_flush)
{
  FAIL();
}

TEST_F(MongoTest, Stream_getBaseStream)
{
  FAIL();
}

TEST_F(MongoTest, Stream_gridfsNew)
{
  FAIL();
}

TEST_F(MongoTest, Stream_read)
{
  FAIL();
}

TEST_F(MongoTest, Stream_readv)
{
  FAIL();
}

TEST_F(MongoTest, Stream_tlsCheckCert)
{
  FAIL();
}

TEST_F(MongoTest, Stream_tlsDoHandshake)
{
  FAIL();
}

TEST_F(MongoTest, Stream_tlsNew)
{
  FAIL();
}

TEST_F(MongoTest, Stream_uncork)
{
  FAIL();
}

TEST_F(MongoTest, Stream_writev)
{
  FAIL();
}

TEST_F(MongoTest, URI_copy)
{
  auto uri = new Uri("mongodb://localhost/bazinga"); 
  ASSERT_TRUE(uri != nullptr);

  auto copy = uri->copy();
  ASSERT_TRUE(copy->database() == "bazinga");  
}

TEST_F(MongoTest, URI_authMechanism)
{
  FAIL();
}

TEST_F(MongoTest, URI_authSource)
{
  FAIL();
}

TEST_F(MongoTest, URI_database)
{
  auto uri = new Uri("mongodb://localhost/bla"); 
  ASSERT_TRUE(uri != nullptr);
  ASSERT_TRUE(uri->database() == "bla");
  delete uri;
}

TEST_F(MongoTest, URI_hosts)
{
  auto uri = new Uri("mongodb://compu1,compu2/bla"); 
  ASSERT_TRUE(uri != nullptr);
  auto hosts = uri->hosts();
  ASSERT_TRUE(hosts.size() == 2 && hosts[1] == "compu2");
  delete uri;
}

TEST_F(MongoTest, URI_options)
{
  auto uri = new Uri("mongodb://compu1,compu2/bla?myopts=baz"); 
  ASSERT_TRUE(uri != nullptr);
  auto options = uri->options();
  ASSERT_TRUE(options.asString() == "{ \"myopts\" : \"baz\" }");
  delete uri;
}

TEST_F(MongoTest, URI_password)
{
  auto uri = new Uri("mongodb://user:secret@compu1/bla?myopts=baz"); 
  ASSERT_TRUE(uri != nullptr);
  auto password = uri->password();
  ASSERT_TRUE(uri->password() == "secret");
  delete uri;
}

TEST_F(MongoTest, URI_readPrefs)
{
  FAIL();
}

TEST_F(MongoTest, URI_replicaSet)
{
  FAIL();
}

TEST_F(MongoTest, URI_ssl)
{
   auto uri = new Uri("mongodb://compu1,compu2/bla?myopts=baz"); 
   auto hasSsl = uri->ssl();

   ASSERT_TRUE(uri != nullptr);
   ASSERT_EQ(hasSsl, false);
   
   delete uri;
}

TEST_F(MongoTest, URI_string)
{
  auto uri = new Uri("mongodb://user:secret@compu1/bla?myopts=baz"); 
  ASSERT_TRUE(uri != nullptr);
  ASSERT_TRUE(uri->string() == "mongodb://user:secret@compu1/bla?myopts=baz");
  delete uri;
}

TEST_F(MongoTest, URI_username)
{
  auto uri = new Uri("mongodb://user:secret@compu1/bla?myopts=baz"); 
  ASSERT_TRUE(uri != nullptr);
  auto username = uri->username();
  ASSERT_TRUE(username == "user");
  delete uri;
}

TEST_F(MongoTest, URI_writeConcern)
{
  FAIL();
}

TEST_F(MongoTest, URI_new)
{
  auto uri = new Uri("mongodb://localhost/test"); 
  ASSERT_TRUE(uri != nullptr);
  delete uri;
}

TEST_F(MongoTest, URI_newForHostPort)
{
  FAIL();
}

TEST_F(MongoTest, URI_unescape)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_copy)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_destroy)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_fsync)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_journal)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_w)
{
  auto wconc = new mongo::WriteConcern;
  ASSERT_TRUE(wconc != nullptr);
  wconc->setW(-123456);
  qint32 w = wconc->w();
  ASSERT_EQ(w, -123456);
  delete wconc;
}

TEST_F(MongoTest, WriteConcern_wmajority)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_wtag)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_wtimeout)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_new)
{
  auto wconc = new mongo::WriteConcern;
  ASSERT_TRUE(wconc != nullptr);
  delete wconc;
}

TEST_F(MongoTest, WriteConcern_setFsync)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_setJournal)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_setW)
{
  auto wconc = new mongo::WriteConcern;
  ASSERT_TRUE(wconc != nullptr);
  wconc->setW(123456);
  qint32 w = wconc->w();
  ASSERT_EQ(w, 123456);
  delete wconc;
}

TEST_F(MongoTest, WriteConcern_setWmajority)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_setWtag)
{
  FAIL();
}

TEST_F(MongoTest, WriteConcern_setWtimeout)
{
  FAIL();
}

