#ifndef TRANSACTOR_H_DEF
#define TRANSACTOR_H_DEF

class Storage;
class IEntity;
class Transactor
{
   Transactor(){}
   Transactor(Storage const &storage);
   
public:
   ~Transactor();
   static Transactor create(Storage const &storage);
   void add(IEntity *entity);
   void store();
   void retrieve();

private:
   class Private;
   Private *d;
};

#endif // TRANSACTOR_H_DEF
