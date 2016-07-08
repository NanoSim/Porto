#ifndef TRANSACTOR_H_DEF
#define TRANSACTOR_H_DEF

#include <string>
#include <map>
#include <iostream>
#include "softns.h"

SOFT_BEGIN_NAMESPACE

class IEntity;
class TransactorCreator;
class Transactor
{
public:
  Transactor()                                     =delete;
  Transactor& operator=(Transactor const &)        =delete;
  Transactor (TransactorCreator const &creator);
  virtual ~Transactor();

  void        store();
  void        read();
  IEntity*    instance(const char *uuid);
  IEntity*    instance(const std::string &uuid);
  void        registerInstance (IEntity *entity);
  int         entityCount() const;
  std::string entity(int) const;
  template <class T> void registerEntity ()  {
     fnmap[T::staticMetaType] = T::create;
  }
  
  static  TransactorCreator create();

protected:
   std::map<const char*, IEntity*(*)(const char*)> fnmap;

private:
  class Private;
  Private *d;
};

class TransactorCreator
{
public:
  TransactorCreator &operator=(TransactorCreator const &) =delete;
  TransactorCreator (TransactorCreator const &other);
  TransactorCreator &driver (const char *arg);
  TransactorCreator &uri (const char *arg);
  TransactorCreator &options (const char *arg);
  
private:
  friend class Transactor;
  TransactorCreator();      

  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // TRANSACTOR_H_DEF
