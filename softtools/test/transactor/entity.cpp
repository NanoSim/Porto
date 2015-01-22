#include "entity.h"

Entity :: Entity(const char *ref)
  : d_uuid(ref)
{}

Entity :: ~Entity()
{}

const char *Entity :: uuid() const
{
  return d_uuid;
}
