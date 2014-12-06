#ifndef QGSLCONSTANTS_H_DEF
#define QGSLCONSTANTS_H_DEF

#include <QObject>

class QGLConstant : public QObject
{
   Q_OBJECT
   Q_PROPERTY (double E MEMBER  m_E CONSTANT);
   Q_PROPERTY (double LOG2E MEMBER m_LOG2E CONSTANT);

public:
   explicit QGLConstant (QObject *parent = nullptr);
   virtual ~QGLConstant();

private:
   double const m_E;
   double const m_LOG2E;
};
#endif // QGSLCONSTANTS_H_DEF

