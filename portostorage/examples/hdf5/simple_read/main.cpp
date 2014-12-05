#include <QtCore>
#include <qg5.h>

int main (int argc, char *argv[])
{
  QH5 h5;
  h5.create("test.h5");
  h5.write("foo", [1,2,3,4,5,6,7,8,10]);
  h5.close();

  QVariant dataset;
  h5.open("test.h5");
  h5.read("foo", &dataset);
  h5.close();
  
  return 0;
}
