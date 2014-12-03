#include <QH5>
#include <QtCore>

int main (int argc, char **argv)
{
  using namespace soft;
  QCoreApplication app (argc, argv);

  auto file = new hdf5::QH5File("test.h5", &app);
  auto isOk = file->create();

  QTextStream(stdout) << "file creation " << (isOk ?
					     QString("succeeded") :
					     QString("failed")) << endl;
  file->close();
  return 0;
}
	  
