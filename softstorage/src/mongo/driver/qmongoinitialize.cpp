#include "qmongo.h"
#include <mongoc.h>

SOFT_BEGIN_NAMESPACE
MONGO_BEGIN_NAMESPACE

void initialize()
{
   mongoc_init();
}

void cleanup()
{
  mongoc_cleanup();
}

MONGO_END_NAMESPACE
SOFT_END_NAMESPACE
