#include <QtScript>
#include <gsl/gsl_math.h>
#include "qgslconstants.h"

QGLConstants :: QGLConstants (QObject *parent)
   : QObject (parent)
   , m_E (M_E)
   , m_LOG2E (M_LOG2E)
{}

QGLConstants ~QGLConstants()
{}
