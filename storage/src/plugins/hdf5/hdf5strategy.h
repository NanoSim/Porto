#ifndef HDF5STRATEGY_H_DEF
#define HDF5STRATEGY_H_DEF

#include "softns.h"
#include "idatamodel.h"
#include "istoragestrategy.h"

SOFT_BEGIN_NAMESPACE

class HDF5Strategy : public IStorageStrategy
{
public:
  HDF5Strategy();
  HDF5Strategy(const char *uri, const char *options);
  HDF5Strategy (HDF5Strategy const &) =delete;
  HDF5Strategy& operator= (HDF5Strategy const &) =delete;  
  virtual ~HDF5Strategy();

  IDataModel* dataModel() const   override;
  void store (IDataModel const *) override;
  void startRetrieve (IDataModel *) const;
  void endRetrieve (IDataModel *) const;

  const char *metaType() const;
  static IStorageStrategy* create(char const *uri, char const *opts);
  static const char *staticMetaType;
private:
   class Private;
   Private *d;
};

SOFT_END_NAMESPACE

#endif // HDF5STRATEGY_H_DEF
