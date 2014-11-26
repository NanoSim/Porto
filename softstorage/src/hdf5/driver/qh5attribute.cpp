#include "qh5attribute.h"

#include "qh5dataset.h"
#include "qh5dataspace.h"
#include "qh5datatype.h"

QH5Attribute :: QH5Attribute (QObject *parent)
  : QObject (parent)
  , attributeId (H5T_NATIVE_HERR)
{}

QH5Attribute :: QH5Attribute (QH5Dataset *dataset,
			      QString const &attributeName,
			      QH5Datatype *datatype,
			      QH5Dataspace *dataspace,
			      QObject *parent)
  : QObject (parent)
  , attributeId (H5Acreate(dataset->id(),
			   qPrintable(attributeName),
			   datatype->id(),
			   dataspace->id(),
			   H5P_DEFAULT,
			   H5P_DEFAULT))
{
}

QH5Attribute :: ~QH5Attribute()
{
  if (attributeId != H5T_NATIVE_HERR) {
    auto status = H5Aclose (attributeId);
    Q_UNUSED (status);
  }
}

