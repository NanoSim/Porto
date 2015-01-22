#include <QDebug>
#include <QString>

#include "qbson.h"
#include "qbsoniter.h"
#include "qbsonvalue.h"
#include "pipe.h"

using soft::QBson;
using soft::QBsonIter;
using soft::QBsonValue;

Pipe :: Pipe(const char *ref)
  : Entity (ref)
{}

Pipe :: ~Pipe()
{}

void Pipe :: store (QBson &bson) const
{
  bson
    .appendArray(uuid(), [this](QBson &props) {
	props
	  .append("length", d_length)
	  .append("diameter", d_diameter)
	  .append("angle", d_angle)
	  .appendUtf8("text", QString::fromUtf8(d_text.c_str()));
      });
}    

void Pipe :: read (QBson const &bson)
{
  QBsonIter it(bson);
  if (it.find(uuid())) {
    it.recurse([this](QBsonIter &sub_it){
	sub_it.next(); d_length = sub_it.value().asDouble();
	sub_it.next(); d_diameter = sub_it.value().asDouble();
	sub_it.next(); d_angle = sub_it.value().asDouble();
	sub_it.next(); d_text = sub_it.value().asConstCharPtr();
      });
  }
}

double Pipe :: length() const
{
  return d_length;
}

double Pipe :: diameter() const
{
  return d_diameter;
}

double Pipe :: angle() const
{
  return d_angle;
}

std::string Pipe :: name() const
{
  return d_text;
}

void Pipe :: setName (const std::string &value)
{
  d_text = value;
}

void Pipe :: setLength (const double &value)
{
  d_length = value;
}

void Pipe :: setDiameter (const double &value)
{
  d_diameter = value;
}

void Pipe :: setAngle (const double &value)
{
  d_angle = value;
}

