#include <QtCore>
#include "qh5.h"

int main()
{
  QH5 h5;
  QVariantMap attributes, groupAttr, fileAttr;
  attributes.insert("something", "something else");
  groupAttr.insert("name", "TestComponent");
  groupAttr.insert("version", "0.1-SNAPSHOT-1");
  groupAttr.insert("uuid", "9f3c2c36-b09f-4773-9923-85de9a33e16c");

  fileAttr.insert("strtype", "blabla");
  fileAttr.insert("inttype", 42);
  
  h5.create("simpler.h5", fileAttr);
  h5.addGroup("foo", groupAttr);
  h5.write("/foo/QString", "This is another new test", attributes);
  h5.write("/foo/scalarInt", 2);
  h5.write("/foo/scalarDouble", 3.14);
  h5.write("/foo/StringList", QStringList() << "this" << "is" << "a list of" << "string");

  return 0;
}
