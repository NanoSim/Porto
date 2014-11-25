#include "qh5property.h"

QH5Property :: QH5Property (QObject *parent)
  : QObject (parent)
  , propertyId (-1)
{}

QH5Property :: QH5Property (hid_t propertyClass, QObject *parent)
  : QObject (parent)
  , propertyId (H5Pcreate (propertyClass))
{
}

QH5Property :: ~QH5Property()
{
  if (propertyId >= 0) {
    H5Pclose (propertyId);
  }
}

hid_t QH5Property :: id() const
{
  return propertyId;
}

