#ifndef QENTITY_PRIVATE_H_DEF
#define QENTITY_PRIVATE_H_DEF
#include "qentity.h"
#include "genericentity.h"

struct QEntity::Private
{
  soft::GenericEntity *ref;
};

#endif
