#ifndef ENTITY_H_DEF
#define ENTITY_H_DEF

namespace soft {
  class QBson;
}

class Entity
{
public:
  explicit Entity(const char *ref);
  virtual ~Entity() = 0;
  virtual void store (soft::QBson &bson) const = 0;
  virtual void read (soft::QBson const &bson) = 0;

  virtual const char *uuid() const;   
private:
  char const *d_uuid;
};

#endif // ENTITY_H_DEF
