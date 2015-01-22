#ifndef QBSONVALUE_H_DEF
#define QBSONVALUE_H_DEF

#include <QObject>
#include "softns.h"

struct _bson_value_t;

NAMESPACE_SOFT_BEGIN

class QBsonValue : public QObject
{
  Q_OBJECT
  explicit QBsonValue (QObject *parent = nullptr);

public:
  QBsonValue (QBsonValue && other);
  QBsonValue (_bson_value_t const *ptr, QObject *parent = nullptr);

  virtual ~QBsonValue();
  int type() const;
  double asDouble() const;
  const char *asConstCharPtr() const;
  
private:
  class Private;
  Private const *d;
  
};

NAMESPACE_SOFT_END

#endif // QBSONVALUE_H_DEF
