#include <QtCore>
#include "qh5.h"
#include "jsonmodel.h"
#include "hdf5strategy.h"

SOFT_BEGIN_NAMESPACE

const char *HDF5Strategy::staticMetaType = "http://sintef.no/soft/TR/storage-strategy#hdf5:0.1-SNAPSHOT-1";

const char *HDF5Strategy::metaType() const
{
  return HDF5Strategy::staticMetaType;
}

struct StrategyFtor {
  StrategyFtor(QString const &uri, const std::string &uuid)
    : uuid(uuid.c_str())
  {
    h5.open(uri);
  }

  ~StrategyFtor()
  {}

  bool getDimension(const char *key, StdUInt &retValue) {
    auto ret = h5.read(QString("%1/dimensions/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::Int)) {
      retValue = ret.value<int>();
      return true;
    }
    return false;
  }  // FIXME - valgrind reports a memory leak here...

  bool getString(const char *key, StdString &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::QString)) {
      retValue = ret.value<QString>().toStdString();
      return true;
    }
    return false;
  }

  bool getInt8(const char *key, int8_t &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::Int)) {
      retValue = ret.value<int>();
      return true;
    }
    return false;
  }

  bool getUInt8(const char *key, uint8_t &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::UInt)) {
      retValue = ret.value<uint>();
      return true;
    }
    return false;
  }

  bool getInt16(const char *key, int16_t &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::Int)) {
      retValue = ret.value<int>();
      return true;
    }
    return false;
  }

  bool getUInt16(const char *key, uint16_t &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::UInt)) {
      retValue = ret.value<uint>();
      return true;
    }
    return false;
  }

  bool getInt32(const char *key, int32_t &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::Int)) {
      retValue = ret.value<int>();
      return true;
    }
    return false;
  }

  bool getUInt32(const char *key, uint32_t &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::UInt)) {
      retValue = ret.value<uint>();
      return true;
    }
    return false;
  }

  bool getInt64(const char *key, int64_t &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::Int)) {
      retValue = ret.value<qint64>();
      return true;
    }
    return false;
  }

  bool getUInt64(const char *key, uint64_t &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::UInt)) {
      retValue = ret.value<quint64>();
      return true;
    }
    return false;
  }

  bool getFloat(const char *key, float &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::Float)) {
      retValue = ret.value<float>();
      return true;
    }
    return false;
  }

  bool getDouble(const char *key, double &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::Double)) {
      retValue = ret.value<double>();
      return true;
    }
    return false;
  }

  bool getBool(const char *key, bool &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::Bool)) {
      retValue = ret.value<bool>();
      return true;
    }
    return false;
  }

  bool getInt32Array(const char *key, std::vector<int32_t> &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::QVariantList)) {

      auto variantList = ret.value<QVariantList>();
      retValue.resize(variantList.size());

      auto it = variantList.constBegin();
      auto retIt = retValue.begin();
      while (it != variantList.constEnd()) {
	(*retIt++) = (*it++).toInt();
      }
      return true;
    }
    return false;
  }


  bool getDoubleArray(const char *key, std::vector<double> &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::QVariantList)) {

      auto variantList = ret.value<QVariantList>();
      retValue.resize(variantList.size());

      auto it = variantList.constBegin();
      auto retIt = retValue.begin();
      while (it != variantList.constEnd()) {
	(*retIt++) = (*it++).toDouble();
      }
      return true;
    }
    return false;
  }

  bool getDoubleArray2D(const char *key, std::vector<std::vector<double> >&retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::QVariantList)) {

      auto variantList = ret.value<QVariantList>();
      retValue.resize(variantList.size());
      for (int i = 0; i < variantList.size(); ++i) {
	auto variantListR2 = variantList.value(i).toList();
	retValue[i].resize(variantListR2.size());
	for (int j = 0; j < variantListR2.size(); ++j) {
	  retValue[i][j] = variantListR2[j].toDouble();
	}
      }
      return true;
    }
    return false;
  }

  bool getDoubleArray3D(const char *key, std::vector<std::vector<std::vector<double> > >&retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::QVariantList)) {
      auto zs = ret.value<QVariantList>();
      const int NK = zs.size();
      retValue.resize(NK);
      for (int k = 0; k < NK; ++k) {
	auto ys = zs.value(k).toList();
	const int NJ = ys.size();
        retValue[k].resize(NJ);
	for (int j = 0; j < NJ; ++j) {
	  auto xs = ys[j].toList();
	  auto NI = xs.size();
          retValue[k][j].resize(NI);
	  for (int i = 0; i < NI; ++i) {
	    retValue[k][j][i] = xs[i].toDouble();
	  }
	}
      }

      return true;
    }
    return false;
  }

  bool getStringArray(const char *key, std::vector<std::string> &retValue) {
    auto ret = h5.read(QString("%1/properties/%2").arg(uuid).arg(key));
    if (ret.isValid() && ret.canConvert(QMetaType::QVariantList)) {
      auto variantList = ret.value<QVariantList>();
      retValue.resize(variantList.size());
      auto it = variantList.constBegin();
      auto retIt = retValue.begin();
      while (it != variantList.constEnd()) {
	(*retIt++) = (*it++).toString().toStdString();
      }
      return true;
    }
    return false;
  }

  QString uuid;
  soft::hdf5::QH5 h5;
};

class HDF5Strategy::Private
{
  friend class HDF5Strategy;
  Private (QString const &uri, QString const &options)
    : uri(uri)
    , options (options)
    , append(false)
  {
    for (auto &s : options.split(";")) {
      auto o = s.split("=");
      if (o.size() > 1) {
	auto keyword = o[0];
	auto value = o[1];
	if (keyword.compare("append", Qt::CaseInsensitive ) == 0) {
	  auto isYes = (value.compare("yes", Qt::CaseInsensitive) == 0) || (value.compare("true", Qt::CaseInsensitive) == 0);
	  append = isYes;
	}
      }
    }
  }

  QString uri;
  QString options;
  bool append;
};

HDF5Strategy :: HDF5Strategy()
  : IStorageStrategy()
  , d (new HDF5Strategy::Private(QString(), QString()))
{}

HDF5Strategy :: HDF5Strategy(const char *uri, const char *options)
  : IStorageStrategy()
  , d (new HDF5Strategy::Private(uri, options))
{}

HDF5Strategy :: ~HDF5Strategy()
{
  delete d;
}

IDataModel* HDF5Strategy :: dataModel() const
{
  IDataModel* ret = (IDataModel*)new JSONModel();
  return ret;
}

void HDF5Strategy :: store (IDataModel const *model)
{
  JSONModel const * jsonModel = dynamic_cast<JSONModel const*>(model);
  auto jsonObj = jsonModel->propsJson();
  auto dimsObj = jsonModel->dimsJson();
  QJsonDocument doc(*jsonObj);
  QJsonDocument dimsDoc(*dimsObj);

  soft::hdf5::QH5 h5;
  if (!QFileInfo::exists(d->uri) || !d->append) {
    h5.create(d->uri);
  } else {
    h5.open(d->uri, false);
  }

  auto id = QString::fromStdString(model->id());
  h5.createGroup(id);
  h5.createGroup(QString("%1/properties").arg(id));
  h5.createGroup(QString("%1/dimensions").arg(id));
  h5.createGroup(QString("%1/meta").arg(id));

  if (dimsDoc.isObject()) {
    auto jsonObject = dimsDoc.object();
    for (auto key: jsonObject.keys()) {
      auto value = jsonObject.value(key);
      h5.write(QString("%1/dimensions/%2").arg(id).arg(key), value.toVariant());
    }
  }

  if (doc.isObject()) {
    auto jsonObject = doc.object();
    for (auto key: jsonObject.keys()) {
      auto value = jsonObject.value(key);
      h5.write(QString("%1/properties/%2").arg(id).arg(key), value.toVariant());
    }
  }
  h5.write(QString("%1/meta/name").arg(id), QString::fromStdString(model->metaName()));
  h5.write(QString("%1/meta/version").arg(id), QString::fromStdString(model->metaVersion()));
  h5.write(QString("%1/meta/namespace").arg(id), QString::fromStdString(model->metaNamespace()));
  h5.close();
}

void HDF5Strategy :: startRetrieve (IDataModel *model) const
{
  using std::placeholders::_1;
  using std::placeholders::_2;
  JSONModel * jsonModel = dynamic_cast<JSONModel*>(model);
  auto jsonObj = jsonModel->propsJson();
  auto dimsObj = jsonModel->dimsJson();
  auto id = model->id();
  StrategyFtor *ftor = new StrategyFtor(d->uri, id);
  jsonModel->data = (void*)ftor;

  std::function<bool(const char*, StdUInt&)>getDimension = std::bind(&StrategyFtor::getDimension, &(*ftor), _1, _2);
  std::function<bool(const char*, std::string&)>getString = std::bind(&StrategyFtor::getString, &(*ftor), _1, _2);
  std::function<bool(const char*, int8_t&)>getInt8 = std::bind(&StrategyFtor::getInt8, &(*ftor), _1, _2);
  std::function<bool(const char*, uint8_t&)>getUInt8 = std::bind(&StrategyFtor::getUInt8, &(*ftor), _1, _2);
  std::function<bool(const char*, int16_t&)>getInt16 = std::bind(&StrategyFtor::getInt16, &(*ftor), _1, _2);
  std::function<bool(const char*, uint16_t&)>getUInt16 = std::bind(&StrategyFtor::getUInt16, &(*ftor), _1, _2);
  std::function<bool(const char*, int32_t&)>getInt32 = std::bind(&StrategyFtor::getInt32, &(*ftor), _1, _2);
  std::function<bool(const char*, uint32_t&)>getUInt32 = std::bind(&StrategyFtor::getUInt32, &(*ftor), _1, _2);
  std::function<bool(const char*, int64_t&)>getInt64 = std::bind(&StrategyFtor::getInt64, &(*ftor), _1, _2);
  std::function<bool(const char*, uint64_t&)>getUInt64 = std::bind(&StrategyFtor::getUInt64, &(*ftor), _1, _2);
  std::function<bool(const char*, float&)>getFloat = std::bind(&StrategyFtor::getFloat, &(*ftor), _1, _2);
  std::function<bool(const char*, double&)>getDouble = std::bind(&StrategyFtor::getDouble, &(*ftor), _1, _2);
  std::function<bool(const char*, bool&)>getBool = std::bind(&StrategyFtor::getBool, &(*ftor), _1, _2);
  std::function<bool(const char*, std::vector<int32_t> &)>getInt32Array = std::bind(&StrategyFtor::getInt32Array, &(*ftor), _1, _2);
  std::function<bool(const char*, std::vector<double> &)>getDoubleArray = std::bind(&StrategyFtor::getDoubleArray, &(*ftor), _1, _2);
  std::function<bool(const char*, std::vector<std::vector<double> >&)>getDoubleArray2D = std::bind(&StrategyFtor::getDoubleArray2D, &(*ftor), _1, _2);
  std::function<bool(const char*, std::vector<std::vector<std::vector<double> > >&)>getDoubleArray3D = std::bind(&StrategyFtor::getDoubleArray3D, &(*ftor), _1, _2);
  std::function<bool(const char*, std::vector<std::string> &)>getStringArray = std::bind(&StrategyFtor::getStringArray, &(*ftor), _1, _2);

  jsonModel->registerGetDimension(getDimension);
  jsonModel->registerGetStringFn(getString);
  jsonModel->registerGetInt8Fn(getInt8);
  jsonModel->registerGetUInt8Fn(getUInt8);
  jsonModel->registerGetInt16Fn(getInt16);
  jsonModel->registerGetUInt16Fn(getUInt16);
  jsonModel->registerGetInt32Fn(getInt32);
  jsonModel->registerGetUInt32Fn(getUInt32);
  jsonModel->registerGetInt64Fn(getInt64);
  jsonModel->registerGetUInt64Fn(getUInt64);
  jsonModel->registerGetFloatFn(getFloat);
  jsonModel->registerGetDoubleFn(getDouble);
  jsonModel->registerGetBoolFn(getBool);
  jsonModel->registerGetInt32ArrayFn(getInt32Array);
  jsonModel->registerGetDoubleArrayFn(getDoubleArray);
  jsonModel->registerGetDoubleArray2DFn(getDoubleArray2D);
  jsonModel->registerGetDoubleArray3DFn(getDoubleArray3D);
  jsonModel->registerGetStringArrayFn(getStringArray);
}

void HDF5Strategy :: endRetrieve (IDataModel *model) const
{
  JSONModel * jsonModel = dynamic_cast<JSONModel*>(model);
  StrategyFtor *ptr = (StrategyFtor*)jsonModel->data;
  delete ptr;
  jsonModel->data = nullptr;
}

IStorageStrategy* HDF5Strategy :: create(char const *uri, char const *opts)
{
  auto s = new HDF5Strategy(uri, opts);
  return s;
}

SOFT_END_NAMESPACE
