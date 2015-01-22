#ifndef QMONGOCURSOR_H_DEF
#define QMONGOCURSOR_H_DEF

#include <QObject>
#include "softns.h"

struct _mongoc_cursor_t;
NAMESPACE_SOFT_BEGIN

class QMongoCursor : public QObject
{
  Q_OBJECT
  
public:
  explicit QMongoCursor (QObject *parent = nullptr);
  QMongoCursor (_mongoc_cursor_t *cursor, QObject *parent = nullptr);
  virtual ~QMongoCursor();

protected:
  _mongoc_cursor_t *cursor() const;
  
private:
  class Private;
  Private *d;
};

NAMESPACE_SOFT_END

#endif // QMONGOCURSOR_H_DEF
