#ifndef BSONDATAMODEL_H_DEF
#define BSONDATAMODEL_H_DEF

#include <idatamodel.h>
#include <softtypes.h>
#include <softns.h>
#include <qbson.h>

SOFT_BEGIN_NAMESPACE

class BsonDataModel : public IDataModel
{
public:
  BsonDataModel();
  virtual ~BsonDataModel();
  virtual IDataModel* createModel() override;
  virtual bool appendDimension     (const char *, StdUInt) override;
  virtual bool appendVariant       (const char *, StdVariant const &) override;
  virtual bool appendString        (const char*,  StdString const &) override;
  virtual bool appendInt8          (const char *, StdInt8) override;
  virtual bool appendUInt8         (const char *, StdUInt8) override;
  virtual bool appendInt16         (const char *, StdInt16) override;
  virtual bool appendUInt16        (const char *, StdUInt16) override;
  virtual bool appendInt32         (const char *, StdInt) override;
  virtual bool appendUInt32        (const char *, StdUInt) override;
  virtual bool appendInt64         (const char *, StdInt64) override;
  virtual bool appendUInt64        (const char *, StdUInt64) override;
  virtual bool appendFloat         (const char *, StdFloat) override;
  virtual bool appendDouble        (const char *, StdDouble) override;
  virtual bool appendBool          (const char *, StdBool) override;
  virtual bool appendInt32Array    (const char *, StdIntArray const &) override;
  virtual bool appendDoubleArray   (const char *, StdDoubleArray const &) override;
  virtual bool appendDoubleArray2D (const char *, StdDoubleArray2D const&) override;
  virtual bool appendDoubleArray3D (const char *, StdDoubleArray3D const&) override;
  virtual bool appendByteArray     (const char *, StdBlob const &) override;
  virtual bool appendStringArray   (const char *, StdStringList const &) override;
  virtual bool appendArray         (const char *, IDataModel const *) override;

  virtual bool getDimension        (const char *, StdUInt &) const override;
  virtual bool getVariant          (const char *, StdVariant &) const override;
  virtual bool getString           (const char *, StdString &str) const override;
  virtual bool getInt8             (const char *, StdInt8 &) const override;
  virtual bool getUInt8            (const char *, StdUInt8 &) const override;
  virtual bool getInt16            (const char *, StdInt16 &) const override;
  virtual bool getUInt16           (const char *, StdUInt16 &) const override;
  virtual bool getInt32            (const char *, StdInt &) const override;
  virtual bool getUInt32           (const char *, StdUInt &) const override;
  virtual bool getInt64            (const char *, StdInt64 &) const override;
  virtual bool getUInt64           (const char *, StdUInt64 &) const override;
  virtual bool getFloat            (const char *, StdFloat &) const override;
  virtual bool getDouble           (const char *, StdDouble &) const override;
  virtual bool getBool             (const char *, StdBool &) const override;
  virtual bool getInt32Array       (const char *, StdIntArray &) const override;
  virtual bool getDoubleArray      (const char *, StdDoubleArray &) const override;
  virtual bool getDoubleArray2D    (const char *, StdDoubleArray2D &) const override;
  virtual bool getDoubleArray3D    (const char *, StdDoubleArray3D &) const override;
  virtual bool getByteArray        (const char *, StdBlob &) const override;
  virtual bool getStringArray      (const char *, StdStringList &) const override;
  virtual bool getArray            (const char *, IDataModel *) const override;
  virtual IDataModel* getModel(const char *) const override;
  virtual bool appendModel(const char *, IDataModel *) override;
  StdStringList getModelLabels() const;
  
  bson::Bson propertyObject;
  bson::Bson dimsObject;

private:
  struct Private;
  std::unique_ptr<Private> d;
};

SOFT_END_NAMESPACE

#endif // BSONDATAMODEL_H_DEF
