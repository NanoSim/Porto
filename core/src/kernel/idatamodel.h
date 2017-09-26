#ifndef IDATAMODEL_H_DEF
#define IDATAMODEL_H_DEF

#include <string>
#include <vector>
#include <cstdint>
#include "softns.h"
#include "softtypes.h"
#include "soft-kernel_export.h"

SOFT_BEGIN_NAMESPACE

class SOFT_KERNEL_EXPORT IDataModel
{
public:
  IDataModel();
  virtual ~IDataModel() = 0;

  virtual IDataModel* createModel() = 0;
  virtual bool appendDimension     (const char *, StdUInt) = 0;
  virtual bool appendVariant       (const char *, StdVariant const &) = 0;
  virtual bool appendString        (const char*, const StdString &) = 0;
  virtual bool appendInt8          (const char *, StdInt8) = 0;
  virtual bool appendUInt8         (const char *, StdUInt8) = 0;
  virtual bool appendInt16         (const char *, StdInt16) = 0;
  virtual bool appendUInt16        (const char *, StdUInt16) = 0;
  virtual bool appendInt32         (const char *, StdInt) = 0;
  virtual bool appendUInt32        (const char *, StdUInt) = 0;
  virtual bool appendInt64         (const char *, StdInt64) = 0;
  virtual bool appendUInt64        (const char *, StdUInt64) = 0;
  virtual bool appendFloat         (const char *, StdFloat) = 0;
  virtual bool appendDouble        (const char *, StdDouble) = 0;
  virtual bool appendBool          (const char *, StdBool) = 0;
  virtual bool appendInt32Array    (const char *, StdIntArray const &) = 0;
  virtual bool appendDoubleArray   (const char *, StdDoubleArray const &) = 0;
  virtual bool appendDoubleArray2D (const char *, StdDoubleArray2D const&) = 0;
  virtual bool appendDoubleArray3D (const char *, StdDoubleArray3D const&) = 0;
  virtual bool appendByteArray     (const char *, StdBlob const &) = 0;
  virtual bool appendStringArray   (const char *, StdStringList const &) = 0;
  virtual bool appendArray         (const char *, IDataModel const *) = 0;

  virtual bool getDimension        (const char *, StdUInt &) const = 0;
  virtual bool getVariant          (const char *, StdVariant &) const = 0;
  virtual bool getString           (const char *, StdString &str) const = 0;
  virtual bool getInt8             (const char *, StdInt8 &) const = 0;
  virtual bool getUInt8            (const char *, StdUInt8 &) const = 0;
  virtual bool getInt16            (const char *, StdInt16 &) const = 0;
  virtual bool getUInt16           (const char *, StdUInt16 &) const = 0;
  virtual bool getInt32            (const char *, StdInt &) const = 0;
  virtual bool getUInt32           (const char *, StdUInt &) const = 0;
  virtual bool getInt64            (const char *, StdInt64 &) const = 0;
  virtual bool getUInt64           (const char *, StdUInt64 &) const = 0;
  virtual bool getFloat            (const char *, StdFloat &) const = 0;
  virtual bool getDouble           (const char *, StdDouble &) const = 0;
  virtual bool getBool             (const char *, StdBool &) const = 0;
  virtual bool getInt32Array       (const char *, StdIntArray &) const = 0;
  virtual bool getDoubleArray      (const char *, StdDoubleArray &) const = 0;
  virtual bool getDoubleArray2D    (const char *, StdDoubleArray2D &) const = 0;
  virtual bool getDoubleArray3D    (const char *, StdDoubleArray3D &) const = 0;
  virtual bool getByteArray        (const char *, StdBlob &) const = 0;
  virtual bool getStringArray      (const char *, StdStringList &) const = 0;
  virtual bool getArray            (const char *, IDataModel *) const = 0;

  // TODO: This is actually different key than above. Make this clear.
  virtual IDataModel* getModel(const char *) const = 0;
  virtual bool appendModel(const char *, IDataModel *) = 0;

  virtual void setId               (const StdString &id) {_id = id;}
  virtual void setMetaName         (const StdString &metaName){_metaName = metaName;}
  virtual void setMetaVersion      (const StdString &metaVersion) {_metaVersion = metaVersion;}
  virtual void setMetaNamespace    (const StdString &metaNamespace) {_metaNamespace = metaNamespace;}

  virtual StdString id() const {return _id;}
  virtual StdString metaName() const {return _metaName;}
  virtual StdString metaVersion() const {return _metaVersion;}
  virtual StdString metaNamespace() const {return _metaNamespace;}
private:
  StdString _id;
  StdString _metaName;
  StdString _metaVersion;
  StdString _metaNamespace;
};

SOFT_END_NAMESPACE

#endif // IDATAMODEL_H_DEF
