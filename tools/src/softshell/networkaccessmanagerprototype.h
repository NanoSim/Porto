#ifndef NETWORKACCESSMANAGERPROTOTYPE_H_DEF
#define NETWORKACCESSMANAGERPROTOTYPE_H_DEF

#include <QScriptable>
#include <QObject>

class QNetworkReply;
class NetworkAccessManagerPrototype : public QObject,
				      public QScriptable
{
  Q_OBJECT
public:
  explicit NetworkAccessManagerPrototype(QObject *parent = nullptr);
  virtual ~NetworkAccessManagerPrototype();

public slots:
  QScriptValue get(QString const &url);

signals:
  void finished(QNetworkReply*);
  void encrypted(QNetworkReply*);
};

#endif // NETWORKACCESSMANAGERPROTOTYPE_H_DEF
