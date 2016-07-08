#ifndef SCENARIO_H_DEF
#define SCENARIO_H_DEF

#include <QString>
#include <QList>
#include <QJsonObject>

struct Scenario {
  QString name;
  QString version;
  QString description;
  QList<QString> refs;
};

Scenario buildScenario (QJsonObject const &scenarioObject);
#endif // SCENARIO_H_DEF
