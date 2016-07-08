#ifndef MONGOSTRATEGY_H_DEF
#define MONGOSTRATEGY_H_DEF

#include "softns.h"
#include "idatamodel.h"
#include "istoragestrategy.h"

SOFT_BEGIN_NAMESPACE

class MongoStrategy : public IStorageStrategy
{
public:
  MongoStrategy();
  MongoStrategy(const char *uri, const char *options);
  MongoStrategy (MongoStrategy const &) =delete;
  MongoStrategy& operator= (MongoStrategy const &) =delete;  
  virtual ~MongoStrategy();

  IDataModel* dataModel() const override;
  void store (IDataModel const *) override;
  void startRetrieve (IDataModel *) const override;
  void endRetrieve (IDataModel*) const override;


  const char *metaType() const;
  static IStorageStrategy* create(char const *uri, char const *opts);
  static const char *staticMetaType;
private:
   class Private;
   Private *d;
};

SOFT_END_NAMESPACE

#endif // MONGOSTRATEGY_H_DEF
