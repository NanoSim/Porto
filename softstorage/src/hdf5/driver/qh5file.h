#ifndef QH5FILE_H_DEF
#define QH5FILE_H_DEF

#include <QObject>
#include <QString>
#include <hdf5.h>
#include "h5ns.h"

SOFT_BEGIN_NAMESPACE
H5_BEGIN_NAMESPACE

class QH5File : public QObject
{
  Q_OBJECT
  Q_ENUMS (HandleFlag)
  
public:
  explicit QH5File (QObject *parent = nullptr);
  QH5File (hid_t id, QObject *parent = nullptr);
  QH5File (QString const &filename, QObject *parent = nullptr);
  virtual ~QH5File();

  enum HandleFlag {Undefined = -1, ReadWrite, ReadOnly};
				       
public slots:
  bool create(QString const &filename);
  bool open(QString const &filename);
  bool create();
  HandleFlag intent() const;
  bool flush();
  int objectCount() const;
  bool close();
  
protected:
  friend class QH5;
  friend class QH5Dataset;
  friend class QH5Group;

public:
  hid_t id() const;
  void setId(hid_t i);

private:
  hid_t fileId;
  QString name; 

};

H5_END_NAMESPACE
SOFT_END_NAMESPACE

#endif // QH5FILE_H_DEF
