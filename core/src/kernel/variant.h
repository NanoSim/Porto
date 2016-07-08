/* 
   Based on "Implementing a variant type from scratch in C++" Posted
   by Tibor Djurica Potpara on October 10, 2013 in "C++, Programming"
*/
#ifndef VARIANT_H_DEF
#define VARIANT_H_DEF

#include <iostream>
#include <utility>
#include <typeinfo>
#include <type_traits>

#include "softns.h"

SOFT_BEGIN_NAMESPACE
template <typename T>
struct VariantType
{
  enum {value = -1};
};
SOFT_END_NAMESPACE

template <size_t arg1, size_t ...rest>
struct staticMax;

template <size_t arg>
struct staticMax<arg>
{
  static const size_t value = arg;
};

template <size_t arg1, size_t arg2, size_t ...rest>
struct staticMax<arg1, arg2, rest...>
{
  static const size_t value = arg1 >= arg2 ? staticMax<arg1, rest...>::value :
    staticMax<arg2, rest...>::value;
};

template <typename... Ts>
struct VariantHelper;

template <typename F, typename... Ts>
struct VariantHelper<F, Ts...>
{
  inline static void destroy (size_t id, void *data)
  {
    if (id == typeid(F).hash_code())
      reinterpret_cast<F*>(data)->~F();
    else
      VariantHelper<Ts...>::destroy(id, data);
  }
  

  inline static void move (size_t oldType, void *oldVariant, void *newVariant)
  {
    if (oldType == typeid(F).hash_code())
      new (newVariant) F(std::move(*reinterpret_cast<F*>(oldVariant)));
    else
      VariantHelper<Ts...>::move(oldType, oldVariant, newVariant);    
  }

  inline static void copy (size_t oldType, void const *oldVariant, void *newVariant)
  {
    if (oldType == typeid(F).hash_code())
      new (newVariant) F(*reinterpret_cast<const F*>(oldVariant));
    else
      VariantHelper<Ts...>::copy(oldType, oldVariant, newVariant);
  }
};

template <>
struct VariantHelper<>
{
  inline static void destroy(size_t , void *) {}
  inline static void move(size_t , void *, void*) {}
  inline static void copy(size_t, void const*, void *) {}
};

SOFT_BEGIN_NAMESPACE

template <typename... Ts>
struct Variant {
private:
  static const size_t dataSize = staticMax<sizeof(Ts)...>::value;
  static const size_t dataAlign = staticMax<alignof(Ts)...>::value;
  using DataType = typename std::aligned_storage<dataSize, dataAlign>::type;
  using Helper = VariantHelper<Ts...>;

  static inline size_t invalidType() {
    return typeid(void).hash_code();
  }
  size_t typeId;
  DataType data;
  int typeNum;
  bool isIntegral;
public:
  Variant()
    : typeId(invalidType())
    , typeNum(-1)
    , isIntegral(false)
   
  {}
  
  Variant(Variant<Ts...> const &old)
    : typeId (old.typeId)
  {
    Helper::copy (old.typeId, &old.data, &data);
  }

  Variant(Variant<Ts...> &&old)
    : typeId (old.typeId)
  {
    Helper::move (old.typeId, &old.data, &data);
  }

  Variant<Ts...>& operator= (Variant<Ts...> old)
  {
    std::swap(typeId, old.typeId);
    std::swap(data, old.data);
    return *this;
  }

  template <typename T>
  bool is() const {
    return (typeId == typeid(T).hash_code());
  }

  bool valid() const {
    return (typeId != invalidType());
  }

  
  size_t typeHash() const {
    return typeId;
  }
  

  int type() const {
    return typeNum;
  }

 
  template <typename T, typename... Args>
  void set(Args&&... args)
  {
    Helper::destroy(typeId, &data);
    new (&data) T(std::forward<Args>(args)...);
    typeId = typeid(T).hash_code();
    typeNum = VariantType<T>::value;
    isIntegral = std::is_integral<T>::value;
  }

  template <typename T>
  const T &get() const
  {
    if (typeId == typeid(T).hash_code())
      return *reinterpret_cast<const T*>(&data);
    else
      throw std::bad_cast();  
  }


  const int toInt() const
  {
    if (isIntegral) 
      return *reinterpret_cast<const int*>(&data);
    else
      throw std::bad_cast();     
  }
  
  ~Variant()   
  {
    Helper::destroy(typeId, &data);
  }
};

SOFT_END_NAMESPACE

#endif
