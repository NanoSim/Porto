#include <QtCore>
#include <tuple>
#include <map>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <softtypes.h>
#include "genericentity.h"
#include "jsonmodel.h"

SOFT_BEGIN_NAMESPACE

#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");


namespace {
  static void appendNotImplemented(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    NOT_IMPLEMENTED
  }

  static void getNotImplemented(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    NOT_IMPLEMENTED
  }
 
  static void appendString(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isString());
    dm->appendString(key, value.toString().toStdString());
  }

  static void appendBool(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isBool());
    dm->appendBool(key, value.toBool());
  }

  static void appendFloat(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isDouble());
    dm->appendFloat(key, (float)value.toDouble());
  }

  static void appendDouble(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isDouble());
    dm->appendDouble(key, value.toDouble());
  }

  static void appendInt64(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isDouble());
    dm->appendInt64(key, (qint64)value.toInt());
  }

  static void appendInt32(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(!value.isNull());
    assert(!value.isUndefined());
    dm->appendInt32(key, value.toInt());
  }

  static std::vector<unsigned char> toUCharVector(QByteArray const &bytes)
  {
    std::vector<unsigned char> cp(bytes.size());
    std::memcpy(cp.data(), bytes.constData(), bytes.size());
    return cp;
  }

  static void appendByteArray(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isString());
    soft::StdBlob blob = toUCharVector(value.toString().toLocal8Bit());
    dm->appendByteArray(key, blob);
  }

  static void appendStringArray(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isArray());
    auto valueList = value.toArray();    
    soft::StdStringList array(valueList.size());
    std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(),[] (QJsonValue const &value) {
        assert(value.isString());
        return value.toString().toStdString();
      });
    dm->appendStringArray(key, array);
  }

  static void appendInt32Array(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isArray());
    auto valueList = value.toArray();
    soft::StdIntArray array(valueList.size());
    std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(),[] (QJsonValue const &value) {
        assert(!value.isUndefined());
        assert(!value.isNull());
        return value.toInt();
      });
    dm->appendInt32Array(key, array);
  }

  static void appendDoubleArray(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isArray());
    auto valueList = value.toArray();
    soft::StdDoubleArray array(valueList.size());
    std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(),[] (QJsonValue const &value) {
        assert(value.isDouble());
        return value.toDouble();
      });
    dm->appendDoubleArray(key, array);
  }

  static void appendDoubleArray2D(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isArray());
    auto valueList2D = value.toArray();
    soft::StdDoubleArray2D array2D(valueList2D.size());
    std::transform(valueList2D.constBegin(), valueList2D.constEnd(), array2D.begin(),[] (QJsonValue const &value) {
        assert(value.isArray());
        auto valueList = value.toArray();
        soft::StdDoubleArray array(valueList.size());
        std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(), [] (QJsonValue const &value) {
            assert(value.isDouble());
            return value.toDouble();
          });
        return array;      
      });
    dm->appendDoubleArray2D(key, array2D);
  }

  static void appendDoubleArray3D(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isArray());
    auto valueList3D = value.toArray();
    soft::StdDoubleArray3D array3D(valueList3D.size());
    std::transform(valueList3D.constBegin(), valueList3D.constEnd(), array3D.begin(),[] (QJsonValue const &value) {    
        assert(value.isArray());
        auto valueList2D = value.toArray();
        soft::StdDoubleArray2D array2D(valueList2D.size());
        std::transform(valueList2D.constBegin(), valueList2D.constEnd(), array2D.begin(),[] (QJsonValue const &value) {
            assert(value.isArray());
            auto valueList = value.toArray();
            soft::StdDoubleArray array(valueList.size());
            std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(), [] (QJsonValue const &value) {
                assert (value.isDouble());
                return value.toDouble();
              });
            return array;      
          });
        return array2D;
      });
    dm->appendDoubleArray3D(key, array3D);
  }

  // This method is not yet supported by IDataModel
  /*
  static void appendDoubleArray4D(IDataModel* dm, const char* key, QJsonValue const &value)
  {
    assert(value.isArray());
    auto valueList4D = value.toArray();
    soft::StdDoubleArray4D array4D(valueList4D.size());
    std::transform(valueList4D.constBegin(), valueList4D.constEnd(), array4D.begin(),[] (QJsonValue const &value) {        
        assert(value.isArray());
        auto valueList3D = value.toArray();
        soft::StdDoubleArray3D array3D(valueList3D.size());
        std::transform(valueList3D.constBegin(), valueList3D.constEnd(), array3D.begin(),[] (QJsonValue const &value) {    
            assert(value.isArray());
            auto valueList2D = value.toArray();
            soft::StdDoubleArray2D array2D(valueList2D.size());
            std::transform(valueList2D.constBegin(), valueList2D.constEnd(), array2D.begin(),[] (QJsonValue const &value) {
                assert(value.isArray());
                auto valueList = value.toArray();
                soft::StdDoubleArray array(valueList.size());
                std::transform(valueList.constBegin(), valueList.constEnd(), array.begin(), [] (QJsonValue const &value) {
                    return value.toDouble();
                  });
                return array;      
              });
            return array2D;
          });
        return array3D;
      });
    dm->appendDoubleArray4D(key, array4D);
  }
  */

  static void getString(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdString v;
    dm->getString(key, v);
    value = QJsonValue(QString::fromStdString(v));
  }

  static void getByteArray(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdBlob blob;
    dm->getByteArray(key, blob);
    QByteArray buffer((const char*)blob.data(), blob.size());
    value = QString::fromLocal8Bit(buffer);
  }

  static void getBool(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdBool v;
    dm->getBool(key, v);
    value = QJsonValue(v);
  }

  static void getFloat(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdFloat v;
    dm->getFloat(key, v);
    value = QJsonValue((double)v);
  }

  static void getDouble(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdDouble v;
    dm->getDouble(key, v);
    value = QJsonValue(v);
  }

  static void getStringArray(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdStringList v;
    dm->getStringArray(key, v);
    QJsonArray jsonArray;
    for (auto i: v) {
      jsonArray.push_back(QString::fromStdString(i));
    }
    value = jsonArray;
  }

  static void getDoubleArray(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdDoubleArray doubleArray;
    dm->getDoubleArray(key, doubleArray);
    QJsonArray jsonArray;
    std::for_each(doubleArray.cbegin(), doubleArray.cend(), [&jsonArray](double const &value) {
        jsonArray.append(value);
      });
    value = jsonArray;
  }

  static void getDoubleArray2D(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdDoubleArray2D doubleArray2D;
    dm->getDoubleArray2D(key, doubleArray2D);
    QJsonArray jsonArray2D;
    std::for_each(doubleArray2D.cbegin(), doubleArray2D.cend(), [&jsonArray2D](soft::StdDoubleArray const &doubleArray) {
        QJsonArray jsonArray;
        std::for_each(doubleArray.cbegin(), doubleArray.cend(), [&jsonArray](double const &value) {
            jsonArray.append(value);
          });
        jsonArray2D.append(jsonArray);
      });
    value = jsonArray2D;
  }

  static void getDoubleArray3D(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdDoubleArray3D doubleArray3D;
    dm->getDoubleArray3D(key, doubleArray3D);
    QJsonArray jsonArray3D;
    std::for_each(doubleArray3D.cbegin(), doubleArray3D.cend(),[&jsonArray3D](soft::StdDoubleArray2D const &doubleArray2D) {    
        QJsonArray jsonArray2D;
        std::for_each(doubleArray2D.cbegin(), doubleArray2D.cend(), [&jsonArray2D](soft::StdDoubleArray const &doubleArray) {
            QJsonArray jsonArray;
            std::for_each(doubleArray.cbegin(), doubleArray.cend(), [&jsonArray](double const &value) {
                jsonArray.append(value);
              });
            jsonArray2D.append(jsonArray);
          });
        jsonArray3D.append(jsonArray2D);
      });
    value = jsonArray3D;
  }

  static void getInt32(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdInt v;
    dm->getInt32(key, v);
    value = QJsonValue(v);
  }

  static void getInt64(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdInt64 v;
    dm->getInt64(key, v);
    value = QJsonValue((int)v);
  }

  static void getInt32Array(IDataModel const* dm, const char* key, QJsonValue &value)
  {
    soft::StdIntArray stdIntArray;
    dm->getInt32Array(key, stdIntArray);
    QJsonArray jsonArray;
    for(auto i: stdIntArray) {
      jsonArray.push_back(i);
    }
    value = jsonArray;
  }

}

typedef void(*GetFnPtr)(IDataModel const*,const char*,QJsonValue&);
typedef void(*AppendFnPtr)(IDataModel*,const char*,QJsonValue const&);
using PropertyClass = QPair<QString,int>;
static std::map<PropertyClass, GetFnPtr> getMap = {
  {qMakePair<QString,int>("int",     0), &getInt32},
  {qMakePair<QString,int>("int32",   0), &getInt32},
  {qMakePair<QString,int>("integer", 0), &getInt32},
  {qMakePair<QString,int>("int",     1), &getInt32Array},
  {qMakePair<QString,int>("int32",   1), &getInt32Array},
  {qMakePair<QString,int>("integer", 1), &getInt32Array},
  {qMakePair<QString,int>("string",  0), &getString},
  {qMakePair<QString,int>("string",  1), &getStringArray},
  {qMakePair<QString,int>("bool",    0), &getBool},
  {qMakePair<QString,int>("float",   0), &getFloat},
  {qMakePair<QString,int>("double",  0), &getDouble},
  {qMakePair<QString,int>("double",  1), &getDoubleArray},
  {qMakePair<QString,int>("double",  2), &getDoubleArray2D},
  {qMakePair<QString,int>("double",  3), &getDoubleArray3D},
  {qMakePair<QString,int>("blob",    0), &getByteArray},
  {qMakePair<QString,int>("int8",    0), &getNotImplemented},
  {qMakePair<QString,int>("uint8",   0), &getNotImplemented},
  {qMakePair<QString,int>("int16",   0), &getNotImplemented},
  {qMakePair<QString,int>("uint16",  0), &getNotImplemented},
  {qMakePair<QString,int>("uint32",  0), &getNotImplemented},
  {qMakePair<QString,int>("int64",   0), &getInt64},
  {qMakePair<QString,int>("uint64",  0), &getNotImplemented}
};
 

static std::map<PropertyClass, AppendFnPtr> appendMap = {
  {qMakePair<QString,int>("int",     0), &appendInt32},
  {qMakePair<QString,int>("int32",   0), &appendInt32},
  {qMakePair<QString,int>("integer", 0), &appendInt32},
  {qMakePair<QString,int>("int",     1), &appendInt32Array},
  {qMakePair<QString,int>("int32",   1), &appendInt32Array},
  {qMakePair<QString,int>("integer", 1), &appendInt32Array},
  {qMakePair<QString,int>("string",  0), &appendString},
  {qMakePair<QString,int>("string",  1), &appendStringArray},
  {qMakePair<QString,int>("bool",    0), &appendBool},
  {qMakePair<QString,int>("float",   0), &appendFloat},
  {qMakePair<QString,int>("double",  0), &appendDouble},
  {qMakePair<QString,int>("double",  1), &appendDoubleArray},
  {qMakePair<QString,int>("double",  2), &appendDoubleArray2D},
  {qMakePair<QString,int>("double",  3), &appendDoubleArray3D},
  {qMakePair<QString,int>("blob",    0), &appendByteArray},
  {qMakePair<QString,int>("int8",    0), &appendNotImplemented},
  {qMakePair<QString,int>("uint8",   0), &appendNotImplemented},
  {qMakePair<QString,int>("int16",   0), &appendNotImplemented},
  {qMakePair<QString,int>("uint16",  0), &appendNotImplemented},
  {qMakePair<QString,int>("uint32",  0), &appendNotImplemented},
  {qMakePair<QString,int>("int64",   0), &appendInt64},
  {qMakePair<QString,int>("uint64",  0), &appendNotImplemented}

};

struct GenericEntity :: Private
{
  QJsonObject schema;
  QJsonObject doc;
  QJsonObject dim;
};

GenericEntity :: GenericEntity()
  : IEntity()
  , d(new GenericEntity::Private())
{}

GenericEntity :: GenericEntity(std::string const &id)
  : IEntity(id)
  , d(new GenericEntity::Private())
{}

GenericEntity :: GenericEntity(const IEntity *other)
  : IEntity(other)
  , d(new GenericEntity::Private())
{}

GenericEntity :: ~GenericEntity()
{}

void GenericEntity :: save(IDataModel *dataModel) const 
{
  auto obj = d->schema;
  auto properties = obj.value("properties").toArray();
  auto dimensions = obj.value("dimensions").toArray();

  auto data = d->doc;
  
  dataModel->setMetaName(obj.value("name").toString().toStdString());
  dataModel->setMetaVersion(obj.value("version").toString().toStdString());
  dataModel->setMetaNamespace(obj.value("namespace").toString().toStdString());
  foreach(auto property, properties) {
    auto obj = property.toObject();
    auto propName = obj.value("name").toString();
    auto propType = obj.value("type").toString();
    auto propRank = obj.contains("dims") ? obj.value("dims").toArray().size() : 0;
    auto propValue = data.value(propName);
    if (!propValue.isUndefined()) {
      PropertyClass pc(propType, propRank);
      auto it = appendMap.find(pc);
      if (it != appendMap.end()) {
        (*it->second)(dataModel, qPrintable(propName), propValue);
      }
    }
  }
}

void GenericEntity :: load(IDataModel const *dataModel) 
{
  auto obj = d->schema;
  auto properties = obj.value("properties").toArray();
  auto dimensions = obj.value("dimensions").toArray();  

  foreach(auto property, properties) {
    auto obj = property.toObject();
    auto propName = obj.value("name").toString();
    auto propType = obj.value("type").toString();
    auto propRank = obj.contains("dims") ? obj.value("dims").toArray().size() : 0;
    PropertyClass pc(propType, propRank);    
    auto it = getMap.find(pc);
    if (it != getMap.end()) {
      QJsonValue propValue;
      (*it->second)(dataModel, qPrintable(propName), propValue);
      d->doc.insert(propName, propValue);
      QJsonDocument doc(d->doc);
    }     
  }
}

IEntity* GenericEntity :: create (const std::string &uuid)
{
    if (uuid.empty()) {
      return new GenericEntity();
    }

    return new GenericEntity(uuid);
}

std::vector<std::string> GenericEntity :: dimensions() const
{
    NOT_IMPLEMENTED
}

int GenericEntity :: getDimensionSize(std::string const &) const
{
    NOT_IMPLEMENTED
}

void GenericEntity :: setSchema(std::string const &json)
{
  QByteArray byteArray(QString::fromStdString(json).toUtf8());
  auto schema = QJsonDocument::fromJson(byteArray);
  d->schema = schema.object(); 
}

void GenericEntity :: debug()
{
  QJsonDocument sch(d->schema);
  QJsonDocument doc(d->doc);
}

QVariant GenericEntity :: property(QString const &key) const
{
  return d->doc.value(key).toVariant();
}

void GenericEntity :: setProperty(QString const &key, QVariant const &value)
{
  d->doc.insert(key, QJsonValue::fromVariant(value));
}

SOFT_END_NAMESPACE
