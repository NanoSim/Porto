#ifndef MONGO_STRATEGY_H_DEF
#define MONGO_STRATEGY_H_DEF

#include <memory>
#include <softns.h>
#include <idatamodel.h>
#include <istoragestrategy.h>

SOFT_BEGIN_NAMESPACE

class MongoStrategy : public IStorageStrategy
{
public:
  MongoStrategy();
  MongoStrategy(const char *uri, const char *options);
  MongoStrategy (MongoStrategy const &) =delete;
  MongoStrategy& operator= (MongoStrategy const &) =delete;  
  virtual ~MongoStrategy();

  IDataModel* dataModel() const   override;
  void store (IDataModel const *) override;
  void startRetrieve (IDataModel *model) const;
  void endRetrieve (IDataModel *model) const;

  const char *metaType() const;
  static IStorageStrategy* create(char const *, char const *);
  static const char *staticMetaType;
private:
  class Private;
  std::unique_ptr<Private> d;
};


SOFT_END_NAMESPACE

#endif // MONGO_STRATEGY_H_DEF
