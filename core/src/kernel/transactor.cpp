#include <memory>
#include <iterator>

#include "soft.h"
#include "ientity.h"
#include "idatamodel.h"
#include "istoragestrategy.h"
#include "transactor.h"

#include <QTextStream>
#include <QMap>

SOFT_BEGIN_NAMESPACE

class TransactorCreator::Private
{
   friend class TransactorCreator;
   friend class Transactor;

   Private ()
   {}
  
   Private (QString const &driver,
	    QString const &uri,
	    QString const &options)
      : driver (driver)
      , uri (uri)
      , options (options)
   {}
  
   QString driver;
   QString uri;
   QString options;
};

class Transactor :: Private
{
   friend class Transactor;

   Private(QString const &driver,
	   QString const &uri,
	   QString const &options)
      : driver (driver)
      , uri (uri)
      , options (options)
      , storageStrategy (soft::create (qPrintable(driver), qPrintable(uri), qPrintable(options)))
   {}

   ~Private()
   {}

   QString const driver;
   QString const uri;
   QString const options;
   std::shared_ptr<IStorageStrategy> const storageStrategy;
   QMap<QString, IEntity*> refmap;

};

Transactor :: Transactor (TransactorCreator const &creator)
   : d (new Transactor::Private(
	   creator.d->driver,
	   creator.d->uri,
	   creator.d->options))
{}

Transactor :: ~Transactor()
{
   delete d;
}

IEntity* Transactor :: instance(const char *uuid)
{
   auto str = QString::fromLocal8Bit(uuid);
   auto ptr = d->refmap.value(str);
   return ptr;
}

IEntity* Transactor :: instance(const std::string &uuid)
{
   auto str = QString::fromStdString (uuid);
   auto ptr = d->refmap.value(str);
   return ptr;
}

void Transactor :: registerInstance (IEntity *entity)
{
   d->refmap.insert(QString::fromStdString(entity->id()), entity);
}


void Transactor :: store()
{
   QTextStream(stdout) << "version 2\n" << endl;
   auto model = d->storageStrategy->dataModel();
   Q_CHECK_PTR (model);

   QMapIterator<QString, IEntity*> i(d->refmap);
   while (i.hasNext())
   {      
      i.next();
      auto v = i.value();
      Q_CHECK_PTR(v);
      if (v != nullptr) {
	 auto child = model->createModel();
	 Q_CHECK_PTR(child);
	 if (child != nullptr) {
	    v->save(child);      
	    model->appendModel(v->id().c_str(), child);	    
	 }
	 delete child;
      }
   }
   d->storageStrategy->store(model);
}

int Transactor :: entityCount() const
{
   return fnmap.size();
}


std::string Transactor :: entity(int index) const
{
   if (index < entityCount() && index >= 0) {
      auto it = fnmap.cbegin();
      std::advance(it, index);
      std::string retval((*it).first);
      return retval;
   }

   return std::string("");
}

TransactorCreator Transactor :: create()
{
   TransactorCreator ret;
   return ret;
}

TransactorCreator :: TransactorCreator ()
   : d (new TransactorCreator::Private())
{
}

TransactorCreator :: TransactorCreator (TransactorCreator const &other)
   : d (new TransactorCreator::Private(other.d->driver,
				       other.d->uri,
				       other.d->options))
{}

TransactorCreator &TransactorCreator :: driver (const char *arg)
{
   d->driver = QString::fromLocal8Bit(arg);
   return *this;
}

TransactorCreator &TransactorCreator :: uri (const char *arg)
{
   d->uri = QString::fromLocal8Bit(arg);
   return *this;
}

TransactorCreator &TransactorCreator :: options (const char *arg)
{
   d->options = QString::fromLocal8Bit(arg);
   return *this;
}


SOFT_END_NAMESPACE
