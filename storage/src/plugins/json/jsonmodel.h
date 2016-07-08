#ifndef JSONMODEL_H_DEF
#define JSONMODEL_H_DEF

#include <softtypes.h>
#include "softns.h"
#include "idatamodel.h"

class QJsonObject;
SOFT_BEGIN_NAMESPACE

class JSONModel : public IDataModel
{
  friend class JSONStrategy;
public:
  JSONModel();
  virtual ~JSONModel();
  IDataModel *createModel() override;

  bool appendDimension     (const char *, StdUInt) override;
  bool appendVariant       (const char *, StdVariant const &) override;
  bool appendString        (const char *, StdString const &) override;
  bool appendInt8          (const char *, StdInt8) override;
  bool appendUInt8         (const char *, StdUInt8) override;
  bool appendInt16         (const char *, StdInt16) override;
  bool appendUInt16        (const char *, StdUInt16) override;
  bool appendInt32         (const char *, StdInt) override;
  bool appendUInt32        (const char *, StdUInt) override;
  bool appendInt64         (const char *, StdInt64) override;
  bool appendUInt64        (const char *, StdUInt64) override;
  bool appendFloat         (const char *, StdFloat) override;
  bool appendDouble        (const char *, StdDouble) override;
  bool appendBool          (const char *, bool) override;
  bool appendInt32Array    (const char *, StdIntArray const &) override;
  bool appendDoubleArray   (const char *, StdDoubleArray const &) override;
  bool appendDoubleArray2D (const char *, StdDoubleArray2D const&) override;
  bool appendDoubleArray3D (const char *, StdDoubleArray3D const&) override;
  bool appendByteArray     (const char *, StdBlob const &) override;
  bool appendStringArray   (const char *, StdStringList const &) override;
  bool appendArray         (const char *, IDataModel const *) override;

  bool getDimension        (const char *, StdUInt &) const override;
  bool getVariant          (const char *, StdVariant &) const override;
  bool getString           (const char *, StdString &str) const override;
  bool getInt8             (const char *, StdInt8 &) const override;
  bool getUInt8            (const char *, StdUInt8 &) const override;
  bool getInt16            (const char *, StdInt16 &) const override;
  bool getUInt16           (const char *, StdUInt16 &) const override;
  bool getInt32            (const char *, StdInt &) const override;
  bool getUInt32           (const char *, StdUInt &) const override;
  bool getInt64            (const char *, StdInt64 &) const override;
  bool getUInt64           (const char *, StdUInt64 &) const override;
  bool getFloat            (const char *, StdFloat &) const override;
  bool getDouble           (const char *, StdDouble &) const override;
  bool getBool             (const char *, StdBool &) const override;
  bool getInt32Array       (const char *, StdIntArray &) const override;
  bool getDoubleArray      (const char *, StdDoubleArray &) const override;
  bool getDoubleArray2D    (const char *, StdDoubleArray2D &) const override;
  bool getDoubleArray3D    (const char *, StdDoubleArray3D &) const override;
  bool getByteArray        (const char *, StdBlob &) const override;
  bool getStringArray      (const char *, StdStringList &) const override;
  bool getArray            (const char *, IDataModel *) const override;

  bool appendModel(const char *, IDataModel *) override;
  IDataModel* getModel(const char *) const override;

  void setJson(QJsonObject const &);
  const QJsonObject *json() const;

private:
  class Private;
  Private *d;
};

SOFT_END_NAMESPACE

#endif // JSONMODEL_H_DEF
