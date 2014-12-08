#ifndef QH5PROTOTYPE_H_DEF
#define QH5PROTOTYPE_H_DEF

#include <QObject>
#include <QScriptable>
#include <QScriptValue>

class QH5Prototype : public QObject
		   , public QScriptable
{
   Q_OBJECT

public:
   explicit QH5Prototype (QObject *parent = nullptr);
   virtual ~QH5Prototype();

public slots:
   QScriptValue read (QString const &key, QScriptValue callback = QScriptValue());
   QScriptValue write (QString const &key, QVariant const &value, QScriptValue const attr = QScriptValue());
};

#endif // QH5PROTOTYPE_H_DEF
