#include <string>
#include "idatamodel.h"
#include "testentity.h"

using namespace soft;

TestEntity :: TestEntity()
  : IEntity()
{}


TestEntity :: ~TestEntity()
{}

IEntity* create (const std::string &)
{
  return dynamic_cast<IEntity*>(new TestEntity());
}

void TestEntity :: save(IDataModel *dataModel) const
{
  dataModel->appendDouble("a", this->a);
  dataModel->appendDoubleArray("vec", vec);
  dataModel->appendString("text", this->text.c_str());
}

void TestEntity :: load(IDataModel const *dataModel)
{
  dataModel->getDouble("a", this->a);
  dataModel->getDoubleArray("vec", this->vec);
  dataModel->getString("text", this->text);
}

std::vector<std::string> TestEntity :: dimensions() const
{
  std::vector<std::string> ret;
  return ret;
			       
}

int TestEntity::getDimensionSize(std::string const &dim) const 
{
  return -1;
}
