#include <QMongo>
#include <QtCore>

int main (int argc, char **argv)
{
  QCoreApplication app(argc, argv);
  auto client = new soft::mongo::Client("mongodb://localhost", &app);
  auto collection = client->collection("meta", "entities");
  auto bson = new soft::bson::Bson("{}", client);
  auto count = collection->count(bson);

  QTextStream(stdout) << "number of entries:" << count << endl;
  return app.exec();
}
