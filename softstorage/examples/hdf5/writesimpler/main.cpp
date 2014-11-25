#include <QtCore>
#include "qh5.h"

int main()
{
  QH5 h5;
  h5.create("simpler.h5");
  h5.write("/QString", "This is another new test");
  h5.write("/scalarInt", 2);
  h5.write("/scalarDouble", 3.14);
  h5.write("/StringList", QStringList() << "this" << "is" << "a list of" << "string");
  h5.write("/Poem", QStringList() <<
           "A fight is a contract that takes two people to honor." <<
	   "A combative stance means that you've accepted the contract." <<
	   "In which case, you deserve what you get." <<
	   "  --  Professor Cheng Man-ch'ing");

  return 0;
}
