#ifndef IENTITY_H_DEF
#define IENTITY_H_DEF

#include "softns.h"
#include <string>
#include <vector>
#include "soft-kernel_export.h"

SOFT_BEGIN_NAMESPACE

class IDataModel;

#define SOFT_META_STRING(x) std::string(#x);
#define SOFT_ENTITY_METADATA(name,ns,ver)	\
  virtual std::string metaType() const override {return name;} \
  virtual std::string metaName() const override {return name;} \
  virtual std::string metaNamespace() const override {return ns;} \
  virtual std::string metaVersion() const override {return ver;}

class SOFT_KERNEL_EXPORT IEntity
{
public:
  IEntity();
  explicit IEntity (std::string const &uuid);
  explicit IEntity (IEntity const *other);
  virtual  ~IEntity() = 0;

  virtual void        save(IDataModel *) const                = 0;
  virtual void        load(IDataModel const *)                = 0;
  static  IEntity*    create (const std::string &uuid);

  virtual std::string id()                              const;
  virtual void setId(std::string const &id);
  virtual std::string metaType()                        const = 0;
  virtual std::string metaName()                        const = 0;
  virtual std::string metaNamespace()                   const = 0;
  virtual std::string metaVersion()                     const = 0;
  virtual std::vector<std::string> dimensions()         const = 0;
  virtual int getDimensionSize(std::string const &)     const = 0;

private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // IENTITY_H_DEF
