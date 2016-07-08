#include <QSharedPointer>
#include <QTextStream>
#include <QVector>
#include <QTextStream>
#include <nlopt.h>
#include "qnlopt.h"

#include <functional>

SOFT_BEGIN_NAMESPACE
NLOPT_BEGIN_NAMESPACE

class QNLopt::Private
{
  friend class QNLopt;
  Private (nlopt_opt opt)
    : value (opt, [](nlopt_opt opt)
	     {
	       nlopt_destroy (opt);
	     })
  {}
  
  Private (QSharedPointer <nlopt_opt_s> value)
    : value(value)
  {}

  QSharedPointer<nlopt_opt_s> value;
};

QNLopt :: QNLopt (QObject *parent)
  : QObject (parent)
  , d (nullptr)
{}

QNLopt :: QNLopt (int algorithm, unsigned int dim, QObject *parent)
  : QObject (parent)
  , d (q_check_ptr(new QNLopt::Private(nlopt_create((nlopt_algorithm)algorithm, dim))))
{}


QNLopt :: QNLopt (nlopt_opt_s *opt, QObject *parent)
  : QObject (parent)
  , d (q_check_ptr (new QNLopt::Private (reinterpret_cast<nlopt_opt>(opt))))
{}


QNLopt :: QNLopt (QNLopt const &other)
  : QObject (nullptr)
  , d (q_check_ptr (new QNLopt::Private (other.d->value)))
{}

QNLopt :: ~QNLopt()
{}

int QNLopt :: setLowerBounds(QVariant const &list)
{
  if (list.type() != QVariant::List) {
    return -1;
  }

  auto dlist = list.toList();
  QVector<double> v(dlist.size());
  std::transform (dlist.constBegin(), dlist.constEnd(), v.begin(), [](QVariant v) -> double
		  {
		    return v.toDouble();
		  });
  
  auto opt = reinterpret_cast<nlopt_opt>(d->value.data());
  auto ret = (int)nlopt_set_lower_bounds(opt, v.constData());
  
  return ret;
}

double QNLopt :: testCallback(std::function<double (unsigned, const double*)>fn)
{
  QTextStream(stdout) << "QNLopt :: testCallback\n";
  int n = 3;
  double v[] = {2.1, 2.2, 3.4};
  auto ret = fn(n, v);
  QTextStream(stdout) << "QNLopt :: testCallback returned " << ret << '\n';
  return 0.0;
}

int QNLopt :: setMinObjective(std::function<double(unsigned, const double*, double*, void*)>fn, void *data)
{
  auto opt = reinterpret_cast<nlopt_opt>(d->value.data());
  auto ret = nlopt_set_min_objective(opt, fn.target<double(unsigned, const double*, double*, void*)>(), data);
  return ret;
}

SOFT_END_NAMESPACE
NLOPT_END_NAMESPACE
