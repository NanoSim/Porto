#include <Soft>
#include <QtCore>

#include "common.h"
#include "remark.h"
#include "portoinput.h"
#include "portooutput.h"

int main(int argc, char **argv)
{
  ::soft::init(argc, argv); 
  if (argc <= 1) {
    QTextStream(stderr) << "usage: " << argv[0] << "<uuid>" << endl;
    return 1;
  }

  soft::Collection collection(argv[1]);
  collection.setName("DFTPrep");
  collection.setVersion("0.1");
  
  soft::Storage storage(const_global_driver, const_global_uri, const_global_options);
  storage.load(&collection);

  PortoInput *input = new PortoInput(&collection);
  Remark *remarc = new Remark(&collection);
  PortoOutput *output = new PortoOutput(&collection);

  QObject::connect(input, SIGNAL(finished()), remarc, SLOT(run()));
  QObject::connect(input, SIGNAL(finished()), input, SLOT(deleteLater()));
  QObject::connect(remarc, SIGNAL(finished()), output, SLOT(run()));
  QObject::connect(remarc, SIGNAL(finished()), remarc, SLOT(deleteLater()));
  QObject::connect(output, SIGNAL(finished()), output, SLOT(deleteLater()));
  QObject::connect(output, SIGNAL(finished()), QCoreApplication::instance(), SLOT(quit()));

  input->run();

  return QCoreApplication::instance()->exec();
}
