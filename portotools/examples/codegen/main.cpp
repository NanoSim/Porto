#include "storage.h"
#include "unit.h"
#include <iostream>

int main(int, char**)
{
  Unit unit;
  std::cout << unit._name << unit._version << std::endl;
}
