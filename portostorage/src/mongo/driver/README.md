QMongo/QBson
============

see [Mongo C API](http://api.mongodb.org/c/current/)


To implement

[ ] Client	
    * command
    * commandSimple
	* collection
	* database
	* databaseNames
	* gridFS
	* maxBsonSize
	* maxMessageSize
	* readPrefs
	* serverStatus
	* uri
	* writeConcern
	* setReadPrefs
	* setSslOpts
	* setWriteConcern

[ ] ClientPool
	* pop
	* push
	* tryPop
	 
[ ] Collection
	* aggregate
	* command
	* commandSimple
	* count
	* createBulkOperation
	* createIndex
	* drop
	* dropIndex
	* ensureIndex
	* find
	* findAndModify
	* lastError
	* name
	* readPrefs
	* writeConcern
	* insert
	* insertBulk
	* keysToIndexString
	* rename
	* save
	* setReadPrefs
	* setWriteConcern
	* stats
	* update
	* validate
[ ] Cursor
	* clone
	* current
	* error
	* getHost
	* isAlive
	* more
	* next
[ ] Database
	* add_user
	* command
	* command_simple
	* create_collection
	* drop
	* collection
	* collectionNames
	* name
	* readPrefs
	* writeConcern
	* hasCollection
	* removeAllUsers
	* removeUser
	* setReadPrefs
	* setWriteConcern

[ ] GridFS
	* createFile
	* createFileFromStream
	* drop
	* find
	* findOne
	* findOneByFilename
	* chunks
	* files

[ ] GridFSFile
	* error
	* aliases
	* chunk_size
	* content_type
	* filename
	* length
	* md5
	* metadata
	* uploadDate
	* readv
	* save
	* seek
	* setAliases
	* setContent_type
	* setFilename
	* setMd5
	* setMetadata
	* tell
	* writev

[ ] GridFSFileList
	* error
	* next

[ ] ReadPrefs
	* add_tag
	* copy
  	* destroy
  	* get_mode
  	* get_tags
  	* is_valid
  	* new
  	* setMode
  	* setTags
[ ] Stream
        * bufferedNew
	* close
	* cork
	* destroy
	* flush
	* getBaseStream
	* gridfsNew
	* read
	* readv
	* tlsCheckCert
	* tlsDoHandshake
	* tlsNew
	* uncork
	* writev
[ ] URI
        * copy
	* destroy
	* authMechanism
	* authSource
	* database
	* hosts
	* options
	* password
	* readPrefs
	* replicaSet
	* ssl
	* string
	* username
	* writeConcern
	* new
	* newForHostPort
	* unescape

[ ] WriteConcern
        * copy
	* destroy
	* fsync
	* journal
	* w
	* wmajority
	* wtag
	* wtimeout
	* new
	* setFsync
	* setJournal
	* setW
	* setWmajority
	* setWtag
	* setWtimeout