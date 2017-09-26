#include <string>
#include <QString>
#include <QPair>
#include <QMultiHash>
#include <QMultiMap>
#include <QTextStream>
#include <QXmlStreamWriter>

#include "tripletstore.h"

SOFT_BEGIN_NAMESPACE

class TripletStore :: Private
{
  friend class TripletStore;
  using TripletHash = QMultiHash<QString,QPair<QString,QString>>;
  using PredicateMap = QMultiMap<QString,QString>;
  PredicateMap predicateMap;
  TripletHash  triplets;
};

/*!
  Constructs the store
 */
TripletStore :: TripletStore()
  : d(new TripletStore::Private())
{}

/*!
  Destroy the store
 */
TripletStore :: ~TripletStore()
{
  delete d;
}

/*!
  Clear all triples
 */
void TripletStore :: clear()
{
  d->triplets.clear();
}

/*!
  Add a triplet to the store
 */
void TripletStore :: addTriplet(std::string const &subj_, std::string const &pred_, std::string const &obj_)
{
  const QString &&pred = QString::fromStdString(pred_);
  const QString &&subj = QString::fromStdString(subj_);
  const QString &&obj  = QString::fromStdString(obj_);
  const QString &&rpred = "^"+pred;

  d->triplets.insert(subj, qMakePair<QString,QString>(pred, obj));
  d->triplets.insert(obj, qMakePair<QString,QString>(rpred, subj));
  Private::PredicateMap::const_iterator i = d->predicateMap.find(pred);
  while (i != d->predicateMap.cend() && i.key() == pred) {
    d->triplets.insert(obj, qMakePair<QString,QString>(i.value(), subj));
    ++i;
  }
}

/*!
  return all triples as a CSV formatted string
 */
std::string TripletStore :: toCSV() const
{
  QByteArray byteArray;
  QTextStream stream(&byteArray, QIODevice::WriteOnly);
  for (auto key: d->triplets.uniqueKeys()) {
    auto i = d->triplets.find(key);
    while (i != d->triplets.cend() && i.key() == key) {
      if (!i.value().first.startsWith("^")) {
        stream << key << "," << i.value().first << "," << i.value().second << endl;
      }
      ++i;
    }
  }
  return byteArray.toStdString();
}

/*!
  Create a triplestore from a CSV file

  \todo Should this be a constructor instead?
 */
void TripletStore::fromCSV(std::string const &csv) {
  const QString c(csv.c_str());
  const QStringList relations = c.split("\n", QString::SkipEmptyParts);
  for (QString r: relations) {
    const QStringList triplet = r.split(",", QString::SkipEmptyParts);
    if (triplet.size() == 3) {
      const std::string subject = triplet[0].toStdString();
      const std::string predicate = triplet[1].toStdString();
      const std::string object = triplet[2].toStdString();
      addTriplet(subject, predicate, object);
    }
    else {
      throw std::runtime_error("Malformed triplet");
    }
  }
}

/*!
  Return a list of objects given the subject \subj and a predicate \pred
 */
std::list<std::string> TripletStore :: findTriplets(const std::string &subj_, const std::string &pred_) const
{
  auto const subj = QString::fromStdString(subj_);
  auto const pred = QString::fromStdString(pred_);
  std::list<std::string>retval;

  auto i = d->triplets.find(subj);
  while (i != d->triplets.cend() && i.key() == subj) {
    if (pred.isNull() || pred == i.value().first) {
      retval.push_back(i.value().second.toStdString());
    }
    ++i;
  }
  return retval;
}

/*!
  return all triples as a RDF-style formatted string (XML)
 */

std::string TripletStore :: toRdf() const
{
  QByteArray byteArray;
  QXmlStreamWriter xml(&byteArray);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();
  xml.writeStartElement("rdf:RDF");
  xml.writeAttribute("xmlns:rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
  xml.writeAttribute("xmlns:soft", "http://www.sintef.no/rdf#");
  for (auto key: d->triplets.uniqueKeys()) {
    auto i = d->triplets.find(key);
    bool first = true;
    while (i != d->triplets.cend() && i.key() == key) {
      if (!i.value().first.startsWith("^")) {
  if (first) {
      xml.writeStartElement("rdf:Description");
      xml.writeAttribute("xmlns:about", key);
      first = !first;
  }
  xml.writeTextElement(i.value().first, i.value().second);
      }
      ++i;
    }
    if (!first)
      xml.writeEndElement();

  }
  xml.writeEndElement();
  xml.writeEndDocument();
  return byteArray.toStdString();
}

/*!
  return number of triples
 */
int TripletStore :: size() const
{
  return d->triplets.size();
}

SOFT_END_NAMESPACE
