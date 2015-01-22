#include <locale.h>
#include <QtCore>
#include <QJsonDocument>
#include <QMongo>

#include "mongostrategy.h"
#include "datastream.h"
#include "transactor.h"
#include "pipe.h"

using soft::QMongoClient;
using soft::QBson;
using soft::QBsonIter;

int main (int argc, char **argv)
{
  QCoreApplication app(argc, argv);
  setlocale(LC_NUMERIC, "C");
  
  QMongoClient::init();

  QBson scope;
  scope.append("foo","bar");

  Pipe p("foobar");
  p.setLength(42);
  p.setDiameter(2);
  p.setAngle(0.2);
  p.setName("wow");
  Pipe p2("another");
  p2.setLength(2);
  p2.setDiameter(4);
  p2.setAngle(1.0);
  Transactor t;
  t << p << p2;

  t.save();

  MongoStrategy strategy;
  soft::DataStream stream(&strategy);

  stream << 2.14;
  
  return 0;
  
  QBson entity;
  entity
    .append("name",    "Foo")
    .append("version", "0.1-SNAPSHOT-1")
    .append("date",    QDateTime::currentDateTime())
    .append("list", {1,2,3,4,5})
    .append("dlist", {1.2, 2.12, 4.3, 2.3})
    .append("str", {QString("en"), QString("test")})
    .appendCodeWithScope("fnc", "function inc(a){return a+1;}", scope)
    .appendDocument("foo", [](QBson &props) {
	props.append("a", 1)
	  .append("b",2)
	  .append("c",3);
      })
    .appendArray("properties", [](QBson &props) {
	props.appendDocument([](QBson &doc){
	    doc	      
	      .append("name", "foo")
	      .append("type", "int")
	      .append("unit", "s");
	  });
      })
    .appendArray("arrays", [](QBson &arr) {
	arr.appendArray([](QBson &arr) {
	    arr.append("somevalue", 42);
	    arr.append("someothervalue", 42);	      
	  });
      });

  QBsonIter it(entity);
  if (it.find("foo")) {
    it.recurse([](QBsonIter &sub_it){
	while (sub_it.next()) {
	  auto v = sub_it.value();
	  qDebug() << "found key" << sub_it.key() << sub_it.value().type();
	}
      });
  }
  
  //  QJsonDocument jsonDoc = entity.toJsonDocument();
  //  qDebug() << jsonDoc.toJson();
  
  qDebug() << entity.asJson();
  QMongoClient::cleanup();

  return 0;
}
