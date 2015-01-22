#include <QDebug>
#include "mongostrategy.h"

MongoStrategy :: MongoStrategy()  
  : StorageStrategy()
{}

MongoStrategy :: ~MongoStrategy()
{}

bool MongoStrategy :: store (double value)
{
  qDebug() << "storing" << value;
  return true;
}
