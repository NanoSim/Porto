#ifndef QNLOPT_H_DEF
#define QNLOPT_H_DEF

#include <QObject>
#include <functional>
#include "nloptns.h"

struct nlopt_opt_s;

SOFT_BEGIN_NAMESPACE
NLOPT_BEGIN_NAMESPACE

class QNLopt : public QObject
{
  Q_OBJECT

public:
  explicit QNLopt (QObject *parent = nullptr);
  QNLopt (int algorithm, unsigned int dim, QObject *parent = nullptr);
  QNLopt (nlopt_opt_s *opt, QObject *parent = nullptr);
  QNLopt (QNLopt const &other);
  virtual ~QNLopt();

public slots:
  int setLowerBounds(QVariant const &list);

public:
  int setMinObjective(std::function<double (unsigned, const double*, double*, void*)>fn, void *data);
  double testCallback(std::function<double (unsigned, const double*)>fn);
private:
  class Private;
  Private *d;
};

NLOPT_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QNLOPT_H_DEF
