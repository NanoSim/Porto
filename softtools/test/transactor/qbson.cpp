#include <QSharedPointer>
#include <QJsonDocument>
#include <QDebug>
#include <mongoc.h>
#include "qbson.h"

NAMESPACE_SOFT_BEGIN

class QBson :: Private
{  
  friend class QBson;

  Private ()
    : bson (q_check_ptr(bson_new()),
	    [](bson_t *ptr)
	    {
	      bson_destroy (ptr);
	    })
  {}
  
  Private (bson_t *doc)
    : bson (q_check_ptr(doc),
	    [](bson_t *ptr)
	    {
	      bson_destroy (ptr);
	    })
  {}
  
  Private (QSharedPointer <bson_t> const & ptr)
    : bson (ptr)
  {}
  
  QSharedPointer <bson_t> bson;
};

static void handleError (bson_error_t const &error)
{
  if (error.code != 0) {
    qWarning() << error.domain << ". " << error.code << ": " << QString::fromUtf8(error.message);
  }
}

QBson :: QBson (QObject * parent)
  : QObject (parent)
  , d (new QBson::Private(bson_new()))
{}

QBson :: QBson (_bson_t * bson, QObject * parent)
  : QObject (parent)
  , d (q_check_ptr(new QBson::Private(reinterpret_cast<bson_t*>(bson))))
{}

static bson_t *newFromJSON(QString const &json)
{
  bson_error_t err;
  auto bson = q_check_ptr(bson_new_from_json (reinterpret_cast<const uint8_t*>(q_check_ptr(json.toUtf8().constData())),
					      json.length(),
					      &err));
  handleError (err);
  return bson;
}

QBson :: QBson (char const *str, QObject * parent)
  : QObject (parent)
  , d (q_check_ptr(new QBson::Private(newFromJSON(str))))
{}

QBson :: ~QBson()
{
  delete d;
}

QBson :: QBson (QBson const &other)
  : QObject (nullptr)
  , d (q_check_ptr(new QBson::Private(other.d->bson)))
{
}

QBson QBson :: fromJson (QString const &json)
{
#if 0
  bson_error_t err;
  auto bson = q_check_ptr(bson_new_from_json (reinterpret_cast<const uint8_t*>(q_check_ptr(json.toUtf8().constData())),
					      json.length(),
					      &err));
  handleError (err;)
  QBson ret(reinterpret_cast<_bson_t *>(bson));
#endif
  
  QBson ret(reinterpret_cast<_bson_t *>(newFromJSON(json)));
  return ret;
}

QString QBson :: asJson() const
{
  size_t length;
  auto chptr = bson_as_json (d->bson.data(), &length);

  QString ret = QString::fromUtf8(chptr, length);
  return ret;
}

_bson_t *QBson :: bson() const
{
  return reinterpret_cast<_bson_t*>(d->bson.data());
}

QBson &QBson :: appendInt64 (QString const &key, qint64 value)
{
  auto retval = bson_append_int64(d->bson.data(),
				  qPrintable(key),
				  key.length(),
				  value);
  if (retval == false) {
    qWarning() << "Error appending int64";
  }

  return *this;
}

QBson &QBson :: appendInt32 (QString const &key, qint32 value)
{
  auto retval = bson_append_int32 (d->bson.data(),
				   qPrintable(key),
				   key.length(),
				   value);
  return *this;
}

QBson &QBson :: appendDouble (QString const &key, double value)
{
  auto retval = bson_append_double (d->bson.data(),
				    qPrintable(key),
				    key.length(),
				    value);
  return *this;
}

QBson &QBson :: appendBool (QString const &key, bool value)
{
  auto retval = bson_append_bool (d->bson.data(),
				  qPrintable(key),
				  key.length(),
				  value);
  return *this; 
}

QBson &QBson :: appendArrayBegin (QString const &key, QBson &child)
{
  auto retval = bson_append_array_begin (d->bson.data(),
					 qPrintable(key),
					 key.length(),
					 child.d->bson.data());
  return *this;
}

QBson &QBson :: appendArrayEnd (QBson &child)
{
  auto retval = bson_append_array_end (d->bson.data(),
				       child.d->bson.data());
  return *this;
}

QBson &QBson :: appendArray (QString const &key, const QBson &array)
{
  auto retval = bson_append_array (d->bson.data(),
				   qPrintable(key),
				   key.length(),
				   array.d->bson.data());
  return *this;
}

static bson_subtype_t toBsonSubtype (QBson::Subtype subtype)
{
  switch (subtype) {
  case QBson::BINARY:
    return BSON_SUBTYPE_BINARY;
  case QBson::FUNCTION:
    return BSON_SUBTYPE_FUNCTION;
  case QBson::UUID:
    return BSON_SUBTYPE_UUID;
  case QBson::MD5:
    return BSON_SUBTYPE_MD5;
  case QBson::USER:
    return BSON_SUBTYPE_USER;
  default:
    return BSON_SUBTYPE_BINARY_DEPRECATED;
  }
}

QBson &QBson :: appendBinary (QString const &key, Subtype subtype, QByteArray const &binary)
{
  auto bsonSubtype = toBsonSubtype (subtype);
  auto blob = reinterpret_cast<const uint8_t*>(binary.constData());
  auto retval = bson_append_binary (d->bson.data(),
				    qPrintable(key),
				    key.length(),
				    bsonSubtype,
				    blob,
				    binary.length());
  return *this;
}

QBson &QBson :: appendCode (QString const &key, QString const &javaScript)
{
  auto retval = bson_append_code (d->bson.data(),
				  qPrintable(key),
				  key.length(),
				  javaScript.toUtf8().constData());
  return *this;
}

QBson &QBson :: appendCodeWithScope (QString const &key, QString const &javaScript, QBson const &scope)
{
  auto retval = bson_append_code_with_scope (d->bson.data(),
					     qPrintable(key),
					     key.length(),
					     javaScript.toUtf8().constData(),
					     scope.d->bson.data());
  return *this;
}

QBson &QBson :: appendDateTime (QString const &key, QDateTime const &dateTime)
{
  auto msecsSinceEpoch = dateTime.toMSecsSinceEpoch();
  auto retval = bson_append_date_time (d->bson.data(),
				       qPrintable (key),
				       key.length(),
				       msecsSinceEpoch);
  return *this;
}

QBson &QBson :: appendDocument (QString const &key, QBson const &value)
{
  auto retval = bson_append_document (d->bson.data(),
				      qPrintable (key),
				      key.length(),
				      value.d->bson.data());
  return *this;
}

QBson &QBson :: appendDocumentBegin (QString const &key, QBson &child)
{
  auto retval = bson_append_document_begin (d->bson.data(),
					    qPrintable (key),
					    key.length(),
					    child.d->bson.data());
  return *this;
}

QBson &QBson :: appendDocument (QString const &key, std::function<void(QBson&)> fn)
{
  QBson child;
  auto retval = bson_append_document_begin (d->bson.data(),
					    qPrintable (key),
					    key.length(),
					    child.d->bson.data());
  
  fn (child);
  retval = bson_append_document_end (d->bson.data(),					  
					  child.d->bson.data());
  return *this;				    
}

QBson &QBson :: appendDocument (std::function<void(QBson&)> fn)
{
  return appendDocument("_id", fn);
}


QBson &QBson :: appendArray (QString const &key, std::function<void(QBson&)> fn)
{
  QBson arr;
  
  auto retval = bson_append_array_begin (d->bson.data(),
					 qPrintable(key),
					 key.length(),
					 arr.d->bson.data());
  fn (arr);
  retval = bson_append_array_end (d->bson.data(),
				  arr.d->bson.data());
				  
  return *this;
}

QBson &QBson :: appendArray (std::function<void(QBson&)> fn)
{
  return appendArray ("_id", fn);
}

QBson &QBson :: appendDocumentEnd (QBson &child)
{
  auto retval = bson_append_document_end (d->bson.data(),					  
					  child.d->bson.data());
  return *this;
}


QBson &QBson :: appendNull (QString const &key)
{
  auto retval = bson_append_null (d->bson.data(),
				  qPrintable (key),
				  key.length());
  return *this;
}

QBson &QBson :: appendRegex (QString const &key, QString const &regex, QString const &options)
{
  auto retval = bson_append_regex (d->bson.data(),
				   qPrintable (key),
				   key.length(),
				   qPrintable (regex),
				   qPrintable (options));
  return *this;				  
}

QBson &QBson :: appendSymbol (QString const &key, QString const &value)
{
  auto retval = bson_append_symbol (d->bson.data(),
				    qPrintable (key),
				    key.length(),
				    qPrintable (value),
				    value.length());
  return *this;				    
}

QBson &QBson :: append (QString const &key, qint32 value)
{
  return appendInt32 (key, value);
}

QBson &QBson :: append (QString const &key, double value)
{
  return appendDouble (key, value);
}

QBson &QBson :: append (QString const &key, bool value)
{
  return appendBool (key, value);
}

QBson &QBson :: append (QString const &key, QString const &value)
{
  return appendUtf8 (key, value);
}

QBson &QBson :: append (QString const &key, Subtype subtype, QByteArray const &binary)
{
  return appendBinary (key, subtype, binary);
}

QBson &QBson :: append (QString const &key, QDateTime const &dateTime)
{
  return appendDateTime (key, dateTime);
}

QBson &QBson :: append (QString const &key)
{
  return appendNull (key);
}

QBson &QBson :: appendUndefined (QString const &key)
{
  auto retval = bson_append_undefined (d->bson.data(),
				       qPrintable (key),
				       key.length());
  return *this;
}

QBson &QBson :: appendUtf8 (QString const &key, QString const &value)
{
  auto retval = bson_append_utf8 (d->bson.data(),
				  qPrintable (key),
				  key.length(),
				  value.toUtf8().constData(),
				  value.length());
  return *this;
}

int QBson :: compare (QBson const &other) const
{
  auto retval = bson_compare(d->bson.data(),
			     other.d->bson.data());
  return retval;
}

bool QBson :: concat (QBson &dst, QBson const &src)
{
  auto retval = bson_concat (dst.d->bson.data(),
			     src.d->bson.data());
  return retval;
}

quint32 QBson :: countKeys() const
{
  return bson_count_keys (d->bson.data());
}

QBson &QBson :: operator= (QBson const &other)
{
  d->bson.reset(bson_copy(other.d->bson.data()),
		[](bson_t *ptr)
		{
		  bson_destroy (ptr);
		});
  return *this;
}

bool QBson :: operator== (QBson const &other) const
{
  return bson_equal (d->bson.data(),
		     other.d->bson.data());
}

QByteArray QBson :: data () const
{
  auto ptr = bson_get_data (d->bson.data());
  QByteArray buffer(reinterpret_cast<const char *>(ptr));
  return buffer;
}

QJsonDocument QBson :: toJsonDocument() const
{
  return QJsonDocument::fromJson(this->asJson().toUtf8());
}

QBson fromJsonDocument(QJsonDocument const &jsonDoc)
{
  return QBson::fromJson(QString::fromUtf8(jsonDoc.toJson (QJsonDocument::Compact)));
}


NAMESPACE_SOFT_END
