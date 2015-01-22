#ifndef QBSON_H_DEF
#define QBSON_H_DEF

#include <QObject>
#include <QDateTime>
#include <functional>
#include <initializer_list>
#include "softns.h"

struct _bson_t;

class QJsonDocument;

NAMESPACE_SOFT_BEGIN

class QBson : public QObject
{
  Q_OBJECT
  Q_ENUMS (Subtype);
public:
  explicit QBson (QObject * parent = nullptr);
  QBson (QBson const &other);
  QBson (char const *str, QObject * parent = nullptr);
  QBson (_bson_t * bson, QObject * parent = nullptr);
  virtual ~QBson();
  
  static QBson fromJson (QString const &json);
  enum Subtype {BINARY, FUNCTION, UUID, MD5, USER};
  
protected:
  friend class QMongoCollection;
  friend class QBsonIter;
  _bson_t *bson() const;
		       
public slots:
  QString asJson() const;
  QBson &appendInt64 (QString const &key, qint64 value);
  QBson &appendInt32 (QString const &key, qint32 value);
  QBson &appendDouble (QString const &key, double value);
  QBson &appendBool (QString const &key, bool value);
  QBson &appendArrayBegin (QString const &key, QBson &child);
  QBson &appendArrayEnd (QBson &child);
  QBson &appendArray (QString const &key, const QBson &array);
  QBson &appendBinary (QString const &key, Subtype subtype, QByteArray const &binary);
  QBson &appendCode (QString const &key, QString const &javaScript);
  QBson &appendCodeWithScope (QString const &key, QString const &javaScript, QBson const &scope);
  QBson &appendDateTime (QString const &key, QDateTime const &dateTime = QDateTime::currentDateTime());
  QBson &appendDocument (QString const &key, QBson const &value);
  QBson &appendDocumentBegin (QString const &key, QBson &child);  
  QBson &appendDocumentEnd (QBson &child);
  QBson &appendNull (QString const &key);
  QBson &appendRegex (QString const &key, QString const &regex, QString const &options = QString());
  QBson &appendSymbol (QString const &key, QString const &value);
  QBson &appendUndefined (QString const &key);
  QBson &appendUtf8 (QString const &key, QString const &value);
  int compare (QBson const &other) const;
  quint32 countKeys() const;
  QByteArray data() const;
  QBson &append (QString const &key, qint32 value);
  QBson &append (QString const &key, double value);
  QBson &append (QString const &key, bool value);
  QBson &append (QString const &key, QString const &value);
  QBson &append (QString const &key, Subtype subtype, QByteArray const &binary);
  QBson &append (QString const &key, QDateTime const &dateTime);
  QBson &append (QString const &key);
  
  static bool concat (QBson &dst, QBson const &src);

public:
  QBson &appendDocument (QString const &key, std::function<void(QBson&)> fn);
  QBson &appendDocument (std::function<void(QBson&)> fn);
  QBson &appendArray (QString const &key, std::function<void(QBson&)> fn);
  QBson &appendArray (std::function<void(QBson&)> fn);

  template <typename T>
  QBson &append (QString const &key, std::initializer_list<T> l) {
    appendArray(key, [&l](QBson &lst){
	for (T t: l) {
	  lst.append("_id", t);
	}
      });
    return *this;
  }

  QJsonDocument toJsonDocument() const;
  static QBson fromJsonDocument(QJsonDocument const &jsonDoc);
  
  QBson &operator=(QBson const &other);
  bool operator==(QBson const &other) const;
  
private:
  class Private;
  Private *d;
  
};

NAMESPACE_SOFT_END

#endif // QBSON_H_DEF
