#ifndef QBSONITER_H_DEF
#define QBSONITER_H_DEF

#include <functional>
#include <QObject>
#include "softns.h"

NAMESPACE_SOFT_BEGIN

class QBson;
class QBsonValue;
class QBsonIter : public QObject
{
  Q_OBJECT
  explicit QBsonIter (QObject *parent = nullptr);
public:
  QBsonIter (QBson const &bson, QObject *parent = nullptr);
  virtual ~QBsonIter();

public slots:
  bool find (QString const &key);
  bool recurse (QBsonIter &child);
  bool next();
  QString key() const;

public:
  QBsonValue value() const;

  bool recurse (std::function<void(QBsonIter&)> fn);
 
private:
  class Private;
  Private *d;
};

NAMESPACE_SOFT_END

#endif // QBSONITER_H_DEF
