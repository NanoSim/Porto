#ifndef QH5FILE_H_DEF
#define QH5FILE_H_DEF

#include <QObject>
#include <QString>
#include <hdf5.h>

class QH5File : public QObject
{
  Q_OBJECT

public:
  explicit QH5File (QObject *parent = nullptr);
  QH5File (QString const &filename, QObject *parent = nullptr);
  virtual ~QH5File();

  bool create(QString const &filename);
  bool create();

protected:
  friend class QH5;
  friend class QH5Dataset;
  friend class QH5Group;
  hid_t id() const;

private:
  hid_t fileId;
  QString name; 

};

#endif // QH5FILE_H_DEF
