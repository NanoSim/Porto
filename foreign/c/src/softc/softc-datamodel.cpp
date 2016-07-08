#include "softtypes.h"
#include "softc-datamodel.h"
#include "softc-datamodel-private.hpp"
#include "softc-string.h"
#include "softc-string-private.hpp"
#include "softc-bytearray-private.hpp"
#include <cstring>
#include <cstdlib>      // for malloc
#include <string>

template <typename T>
void arrayToPtr(T **dest, std::vector<T> const &source)
{
  const int size = source.size();
  (*dest) = new T[size];
  std::copy(source.cbegin(),source.cend(), *dest);
}

template <typename T>
void arrayToPtr(T ***dest, std::vector<std::vector<T>> const &source)
{
  *dest = new T*[source.size()];
  auto ptr = *dest;
  for(auto &s: source) {
    arrayToPtr(&(*ptr++), s);
  }
}

template <typename T>
void arrayToPtr(T ****dest, std::vector<std::vector<std::vector<T>>> const &source)
{
  *dest = new T**[source.size()];
  auto ptr = *dest;
  for(auto &s: source) {
    arrayToPtr(&(*ptr++), s);
  }
}

template <typename T>
void ptrToArray(std::vector<T> &dest, const T *src, size_t length)
{
  dest.resize(length);
  dest.assign(src, src+length);
}

template <typename T>
void ptrToArray(std::vector<std::vector<T>> &dest, const T* const* const& src, size_t i, size_t j)
{
  dest.resize(j);
  for (size_t idx = 0; idx < j; ++idx) {
    ptrToArray(dest[idx], src[idx], i);
  }
}

template <typename T>
void ptrToArray(std::vector<std::vector<std::vector<T> > > &dest, const T* const* const* src, size_t i, size_t j, size_t k)
{
  dest.resize(k);
  for (size_t idx = 0; idx < k; ++idx) {
      ptrToArray(dest[idx], src[idx], i, j);
  }
}

bool softc_datamodel_append_dimension    (softc_datamodel_t *model, const char *key, uint32_t value)
{
  if (model->ref) {
    return model->ref->appendDimension(key, value);
  }
  return false;
}

bool softc_datamodel_append_string (softc_datamodel_t *model, const char *key, const softc_string_s value)
{
  if (model->ref && value) {
    return model->ref->appendString(key, value->str);    
  }
  return false;
}

bool softc_datamodel_append_int8 (softc_datamodel_t *model, const char *key, int8_t value)
{
  if (model->ref) {
    return model->ref->appendInt8(key, value);
  }
  return false;
}

bool softc_datamodel_append_uint8 (softc_datamodel_t *model, const char *key, uint8_t value)
{
  if (model->ref) {
    return model->ref->appendUInt8(key, value);
  }
  return false;
}

bool softc_datamodel_append_int16 (softc_datamodel_t *model, const char *key, int16_t value)
{
  if (model->ref) {
    return model->ref->appendInt16(key, value);
  }
  return false;
}

bool softc_datamodel_append_uint16 (softc_datamodel_t *model, const char *key, uint16_t value)
{
  if (model->ref) {
    return model->ref->appendUInt16(key, value);
  }
  return false;
}

bool softc_datamodel_append_int32 (softc_datamodel_t *model, const char *key, int32_t value)
{
  if (model->ref) {
    return model->ref->appendInt32(key, value);
  }
  return false;
}

bool softc_datamodel_append_uint32 (softc_datamodel_t *model, const char *key, uint32_t value)
{
  if (model->ref) {
    return model->ref->appendUInt32(key, value);
  }
  return false;
}

bool softc_datamodel_append_int64 (softc_datamodel_t *model, const char *key, int64_t value)
{
  if (model->ref) {
    return model->ref->appendInt64(key, value);
  }
  return false;
}

bool softc_datamodel_append_uint64 (softc_datamodel_t *model, const char *key, uint64_t value)
{
  if (model->ref) {
    return model->ref->appendUInt64(key, value);
  }
  return false;
}

bool softc_datamodel_append_float (softc_datamodel_t *model, const char *key, float value)
{
  if (model->ref) {
    return model->ref->appendFloat(key, value);
  }
  return false;
}

bool softc_datamodel_append_double (softc_datamodel_t *model, const char *key, double value)
{
  if (model->ref) {
    return model->ref->appendDouble(key, value);
  }
  return false;
}

bool softc_datamodel_append_bool (softc_datamodel_t *model, const char *key, bool value)
{
  if (model->ref) {
    return model->ref->appendBool(key, value);
  }
  return false;
}

bool softc_datamodel_append_blob (softc_datamodel_t *model, const char *key, const softc_bytearray_s value)
{
  if (model->ref) {
    soft::StdBlob blob(value->bytearray.begin(), value->bytearray.end());
    return model->ref->appendByteArray(key, blob);
  }
  return false;
}

bool softc_datamodel_append_string_list (softc_datamodel_t *model, const char *key, const softc_string_s *value, size_t n_elements)
{
  if (model->ref) {
    soft::StdStringList stringList(n_elements);
    for (size_t i = 0; i < n_elements; ++i) {
      if (value[i]) {     // Silently treat nullptr as empty strings (no error returned)
        stringList[i] = value[i]->str;      
      }
    }
    return model->ref->appendStringArray(key, stringList);
  }
  return false;
}

bool softc_datamodel_append_array_int32  (softc_datamodel_t *model, const char *key, const int32_t *value, size_t size)
{
  if (model->ref) {
    soft::StdIntArray valueVec;
    ptrToArray(valueVec, value, size);
    return model->ref->appendInt32Array(key, valueVec);
  }
  return false;
}

bool softc_datamodel_append_array_int32_2d  (softc_datamodel_t *model, const char *key, const int32_t* const* value, size_t size_i, size_t size_j)
{
  return false;
}

bool softc_datamodel_append_array_int32_3d  (softc_datamodel_t *model, const char *key, const int32_t* const* const*value, size_t size_i, size_t size_j, size_t size_k)
{
  return false;
}

bool softc_datamodel_append_array_double (softc_datamodel_t *model, const char *key, const double *value, size_t size)
{
  if (model->ref) {
    soft::StdDoubleArray valueVec;
    ptrToArray(valueVec, value, size);
    return model->ref->appendDoubleArray(key, valueVec);
  }
  return false;
}

bool softc_datamodel_append_array_double_2d (softc_datamodel_t *model, const char *key, const double* const* value, size_t size_i, size_t size_j)
{
  if (model->ref) {
    soft::StdDoubleArray2D valueVec2D;
    ptrToArray(valueVec2D, value, size_i, size_j);
    return model->ref->appendDoubleArray2D(key, valueVec2D);
  }
  return false;
}

bool softc_datamodel_append_array_double_3d (softc_datamodel_t *model, const char *key, const double* const* const* value, size_t size_i, size_t size_j, size_t size_k)
{
  if (model->ref) {
    soft::StdDoubleArray3D valueVec3D;
    ptrToArray(valueVec3D, value, size_i, size_j, size_k);
    return model->ref->appendDoubleArray3D(key, valueVec3D);
  }
  return false;
}

bool softc_datamodel_get_dimension       (const softc_datamodel_t *model, const char *key, uint32_t *value)
{
  if (model->ref) {
    soft::StdUInt ret;
    auto isOk = model->ref->getDimension(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }
  }
}

bool softc_datamodel_get_array_double_2d (const softc_datamodel_t *model, const char *key, double ***value, size_t *size_i, size_t *size_j)
{
  if (model->ref) {
    soft::StdDoubleArray2D ret;
    auto isOk = model->ref->getDoubleArray2D(key, ret);
    if (isOk) {
      arrayToPtr(value, ret);
      *size_i = ret[0].size();
      *size_j = ret.size();
      return true;
    }
  }
  return false;
}

bool softc_datamodel_get_array_double_3d (const softc_datamodel_t *model, const char *key, double ****value, size_t *size_i, size_t *size_j, size_t *size_k)
{
  if (model->ref) {
    soft::StdDoubleArray3D ret;
    auto isOk = model->ref->getDoubleArray3D(key, ret);
    if (isOk) {
      arrayToPtr(value, ret);
      *size_i = ret[0][0].size();
      *size_j = ret[0].size();
      *size_k = ret.size();
      return true;
    }
  }
  return false;
}

bool softc_datamodel_get_string (const softc_datamodel_t *model, const char *key, softc_string_s *value)
{
  if (model->ref && value) {
    soft::StdString str;
    if (model->ref->getString(key, str)) {
      *value = softc_string_create( str.c_str() );
      return true;
    }
  }
  return false;
}

bool softc_datamodel_get_int8 (const softc_datamodel_t *model, const char *key, int8_t *value)
{
  if (model->ref) {
    soft::StdInt8 ret;
    auto isOk = model->ref->getInt8(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }
  }
    
  return false;
}

bool softc_datamodel_get_uint8 (const softc_datamodel_t *model, const char *key, uint8_t *value)
{
  if (model->ref) {
    soft::StdUInt8 ret;
    auto isOk = model->ref->getUInt8(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_int16 (const softc_datamodel_t *model, const char *key, int16_t *value)
{
  if (model->ref) {
    soft::StdInt16 ret;
    auto isOk = model->ref->getInt16(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_uint16 (const softc_datamodel_t *model, const char *key, uint16_t *value)
{
  if (model->ref) {
    soft::StdUInt16 ret;
    auto isOk = model->ref->getUInt16(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_int32 (const softc_datamodel_t *model, const char *key, int32_t *value)
{
  if (model->ref) {
    soft::StdInt ret;
    auto isOk = model->ref->getInt32(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_uint32 (const softc_datamodel_t *model, const char *key, uint32_t *value)
{
  if (model->ref) {
    soft::StdUInt ret;
    auto isOk = model->ref->getUInt32(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_int64 (const softc_datamodel_t *model, const char *key, int64_t *value)
{
  if (model->ref) {
    soft::StdInt64 ret;
    auto isOk = model->ref->getInt64(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_uint64 (const softc_datamodel_t *model, const char *key, uint64_t *value)
{
  if (model->ref) {
    soft::StdUInt64 ret;
    auto isOk = model->ref->getUInt64(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_float (const softc_datamodel_t *model, const char *key, float *value)
{
  if (model->ref) {
    soft::StdFloat ret;
    auto isOk = model->ref->getFloat(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_double (const softc_datamodel_t *model, const char *key, double *value)
{
  if (model->ref) {
    soft::StdDouble ret;
    auto isOk = model->ref->getDouble(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_bool (const softc_datamodel_t *model, const char *key, bool *value)
{
  if (model->ref) {
    soft::StdBool ret;
    auto isOk = model->ref->getBool(key, ret);
    if (isOk) {
      *value = ret;
      return true;
    }	       
  }
  return false;
}

bool softc_datamodel_get_blob (const softc_datamodel_t *model, const char *key, softc_bytearray_s *value)
{
  if (model->ref) {
    soft::StdBlob ret;
    auto isOk = model->ref->getByteArray(key, ret);
    if (isOk) {
      (*value)->bytearray =QByteArray((const char*)ret.data(), (int)ret.size());
    }
  }
  return false;
}

bool softc_datamodel_get_string_list (const softc_datamodel_t *model, const char *key, softc_string_s **value, size_t *n_elements)
{
  if (model->ref) {
    soft::StdStringList source;
    auto isOk = model->ref->getStringArray(key, source);
    if (isOk) {
      auto siz = source.size();
      *value = (softc_string_s *)malloc(siz*sizeof(softc_string_s));
      for (size_t i = 0; i < siz; ++i) {
        (*value)[i] = softc_string_create( source[i].c_str() );
      }      
      *n_elements = source.size();
      return true;
    }
  }
  return false;
}

bool softc_datamodel_get_array_int32 (const softc_datamodel_t *model, const char *key, int32_t **value, size_t *size)
{
  if (model->ref) {
    soft::StdIntArray res;
    if (model->ref->getInt32Array(key, res)) {
      *size = res.size();
      *value = (int32_t*)malloc(sizeof(int32_t)*res.size());
      std::memcpy(*value, res.data(), sizeof(int32_t)*res.size());
      return true;
    }
  }
  return false;
}

bool softc_datamodel_get_array_int32_2d (const softc_datamodel_t *model, const char *key, int32_t ***value, size_t *size_i, size_t *size_j)
{
  return false;
}

bool softc_datamodel_get_array_int32_3d (const softc_datamodel_t *model, const char *key, int32_t ****value, size_t *size_i, size_t *size_j, size_t *size_k)
{
  return false;
}

bool softc_datamodel_get_array_double (const softc_datamodel_t *model, const char *key, double **value, size_t *size)
{
  if (model->ref) {
    soft::StdDoubleArray res;
    if (model->ref->getDoubleArray(key, res)) {
      *size = res.size();
      *value = (double*)malloc(sizeof(double)*res.size());
      std::memcpy(*value, res.data(), sizeof(double)*res.size());
      return true;
    }
  }
  return false;
}

bool softc_datamodel_set_id              (softc_datamodel_t* model, const char *id)
{
  if (!model) return false;
  model->ref->setId(id);
  return true;
}

bool softc_datamodel_set_meta_name       (softc_datamodel_t* model, const char *meta_name)
{
  if (!model) return false;
  model->ref->setMetaName(meta_name);
  return true;
}

bool softc_datamodel_set_meta_version    (softc_datamodel_t* model, const char *meta_version)
{
  if (!model) return false;
  model->ref->setMetaVersion(meta_version);
  return true;

}

bool softc_datamodel_set_meta_namespace  (softc_datamodel_t* model, const char *meta_namespace)
{
  if (!model) return false;
  model->ref->setMetaNamespace(meta_namespace);
  return true;

}

const char * softc_datamodel_get_id            (const softc_datamodel_t* model)
{
  if (!model) return NULL;
  auto std_id = model->ref->id();
  return std_id.c_str();
}

const char * softc_datamodel_get_meta_name     (const softc_datamodel_t* model)
{
  if (!model) return NULL;
  auto ret = model->ref->metaName();
  return ret.c_str();
}

const char * softc_datamodel_get_meta_version  (const softc_datamodel_t* model)
{
  if (!model) return NULL;
  auto ret = model->ref->metaVersion();
  return ret.c_str();
}

const char * softc_datamodel_get_meta_namespace(const softc_datamodel_t* model)
{
  if (!model) return NULL;
  auto ret = model->ref->metaNamespace();
  return ret.c_str();
}

