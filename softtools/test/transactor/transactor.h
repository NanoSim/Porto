#ifndef TRANSACTOR_H_DEF
#define TRANSACTOR_H_DEF

class Entity;

class Transactor
{
public:
  Transactor();
  virtual ~Transactor();

  Transactor &operator<<(Entity &e);
  bool save();
  
private:
  class Private;
  Private *d;
};

#endif // TRANSACTOR_H_DEF
