#ifndef QBSON_H_DEF
#define QBSON_H_DEF

#include <QObject>
#include <QString>
#include <memory>
#include <mongoc.h>
#include <bson.h>
#include <softtypes.h>
#include "mongons.h"

SOFT_BEGIN_NAMESPACE

MONGO_BEGIN_NAMESPACE
class Client;
class Collection;
class GridFSFileOpt;
MONGO_END_NAMESPACE

BSON_BEGIN_NAMESPACE

class Bson : public QObject
{
  Q_OBJECT

public:
  explicit Bson (QObject * parent = nullptr);
  Bson (QString const& json, QObject * parent = nullptr);
  Bson (bson_t *bsn, QObject * parent = nullptr);
  Bson (Bson const & other);
  Bson (Bson && other);
  virtual ~Bson();
   
  bool appendBson (char const *key, Bson const &value);
  bool appendString (char const * key, char const * value);
  bool appendStringList (char const * key, QStringList const &value);
  bool appendInt32 (char const * key, qint32 value);
  bool appendInt64 (char const * key, qint64 value);
  bool appendDouble(char const * key, double const &value);
  bool appendFloat(char const * key, float const &value);
  bool appendBool (char const * key, bool value);
  bool appendBinary (char const *key, QByteArray const &value);
  bool appendIntArray(char const *key, soft::StdIntArray const &value);
  bool appendDoubleArray(char const *key, soft::StdDoubleArray const &value);
  bool appendDoubleArray2D(char const *key, soft::StdDoubleArray2D const &value);
  bool appendDoubleArray3D(char const *key, soft::StdDoubleArray3D const &value);

  bool append(char const *key, Bson const &value);
  bool append(char const *key, soft::StdInt const &value);
  bool append(char const *key, soft::StdDouble const &value);
  bool append(char const *key, soft::StdString const &value);
  bool append(char const *key, soft::StdStringList const &value);
  bool append(char const *key, soft::StdIntArray const &value);
  bool append(char const *key, soft::StdDoubleArray const &value);
  bool append(char const *key, soft::StdDoubleArray2D const &value);
  bool append(char const *key, soft::StdDoubleArray3D const &value);
  bool get(char const *key, soft::StdInt &value) const;
  bool get(char const *key, soft::StdDouble &value) const;
  bool get(char const *key, soft::StdString &value) const;
  bool get(char const *key, soft::StdStringList &value) const;
  bool get(char const *key, soft::StdIntArray &value) const;
  bool get(char const *key, soft::StdDoubleArray &value) const;
  bool get(char const *key, soft::StdDoubleArray2D &value) const;
  bool get(char const *key, soft::StdDoubleArray3D &value) const;

  bool getInt32(const char *key, qint32 &value) const;
  bool getInt64(const char *key, qint64 &value) const;
  bool getString(const char *key, QString &value) const;
  bool getDouble(const char *key, double &value) const;
  bool getFloat(const char *key, float &value) const;
  bool getBool (const char *key, bool &value) const;
  bool getBinary(const char *key, QByteArray &value) const;
  bool getStringList (char const * key, QStringList &value) const;
  bool getIntArray(char const *key, soft::StdIntArray &value) const;
  bool getDoubleArray(char const *key, soft::StdDoubleArray &value) const;
  bool getDoubleArray2D(char const *key, soft::StdDoubleArray2D &value) const;
  bool getDoubleArray3D(char const *key, soft::StdDoubleArray3D &value) const;

  QStringList keys() const;
  qint32 countKeys() const;
  bool hasField(char const * key) const;
  QString asString() const;
  Bson getBson(const char *key) const;

  Bson& operator=(Bson const &other);
  std::shared_ptr<struct _bson_t> bsonPtr();
  
  protected:
  struct _bson_t* data() const;

private:
  std::shared_ptr<struct _bson_t> bson;
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::GridFSFileOpt;
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::Client;
  friend class SOFT_NAMESPACE::MONGO_NAMESPACE::Collection;  
};

BSON_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QBSON_H_DEF
