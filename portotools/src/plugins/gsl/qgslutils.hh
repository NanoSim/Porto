#pragma once

#include <QVariantList>

template <typename T>
QVector<T> toVector(QVariantList const &xs)
{
  QVector<T> vec(xs.length());
 
  auto xsIt = xs.constBegin();
  auto i = vec.begin();
 
  while (xsIt != xs.constEnd()) {
    *(i++) = ((*(xsIt++)).value<T>());
  }
  return vec;
}

