#ifndef JSONSTRATEGY_H_DEF
#define JSONSTRATEGY_H_DEF

#include "softns.h"
#include "idatamodel.h"
#include "istoragestrategy.h"

SOFT_BEGIN_NAMESPACE

class JSONStrategy : public IStorageStrategy
{
public:
  JSONStrategy();
  JSONStrategy(const char *uri, const char *options);
  JSONStrategy(JSONStrategy const &) =delete;
  JSONStrategy& operator= (JSONStrategy const& ) =delete;
  virtual ~JSONStrategy();

  IDataModel      *dataModel   () const   override;
  void             store       (IDataModel const *) override;
  void             startRetrieve    (IDataModel *) const override;
  void             endRetrieve    (IDataModel *) const override;

  const char *metaType() const override;
  static IStorageStrategy* create(char const *uri, char const *opts);
  static const char *staticMetaType;
private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // JSONSTRATEGY_H_DEF
