#include "scenario.h"
#include <QJsonArray>

QList<QString> buildRefsList (QJsonArray const &array)
{
  QList<QString> refsList;
  auto it = array.constBegin();
  while (it != array.constEnd()) {   
    refsList.append ((*it).toString());
    ++it;
  }
  return refsList;
}

Scenario buildScenario (QJsonObject const &scenarioObject)
{
  Scenario scenario;
  scenario.name = scenarioObject.value("name").toString();
  scenario.version = scenarioObject.value("version").toString();
  scenario.description = scenarioObject.value("description").toString();
  scenario.refs = buildRefsList(scenarioObject.value("refs").toArray());

  return scenario;
}
