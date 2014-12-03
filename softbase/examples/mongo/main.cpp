#include <QMongo>
#include <QtCore>

int main (int argc, char **argv)
{
  using namespace soft;
  QCoreApplication app(argc, argv);

  mongo::Client::initialize();
  auto client = new mongo::Client("mongodb://localhost", &app);
  auto collection = client->collection("meta", "entities");
  auto bson = new bson::Bson("{}", client);
  auto count = collection->count(bson);

  QTextStream(stdout) << "number of entries:" << count << endl;
  auto retval = app.exec();
  mongo::Client::cleanup();

  return retval;
}
