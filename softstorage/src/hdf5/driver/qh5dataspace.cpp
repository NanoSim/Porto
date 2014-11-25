#include <QtDebug>
#include <algorithm>
#include <vector>
#include <iterator>
#include <hdf5.h>
#include "qh5dataspace.h"


QH5Dataspace :: QH5Dataspace (QObject *parent)
  : QObject (parent)
  , dataspaceId (H5T_NATIVE_HERR)
{}

QH5Dataspace :: ~QH5Dataspace()
{
  if (this->dataspaceId != H5T_NATIVE_HERR) {
    H5Sclose(this->dataspaceId);
  }
}

QH5Dataspace :: QH5Dataspace (QH5Dataspace &&other)
  : QObject (nullptr)
  , dataspaceId (std::move(other.dataspaceId))
{}

hid_t QH5Dataspace :: id() const
{
  return this->dataspaceId;
}

QH5Dataspace :: QH5Dataspace (QVector<int> const &dims, QObject * parent)
  : QObject (parent)
  , dataspaceId ([dims] () -> hid_t {
	std::vector<hsize_t> sizes(dims.size());
	std::copy (dims.constBegin(), dims.constEnd(), sizes.begin());
	return H5Screate_simple(dims.size(), &sizes[0], nullptr);	
     }())
{}
