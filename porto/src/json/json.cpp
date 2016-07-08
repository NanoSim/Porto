// Test plugin that also functions as an example of various features.

#include "json.h"

#include <string.h>
#include <algorithm>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QUrl>
#include <collection.h>
#include <softc/softc-datamodel-private.hpp>
#include "financial.hxx"
#include "physics.hxx"


int softc_plugin_identify( char* name, int maxlen ) {
  // TODO: Is this the name we want to return?
  const char plugin_name[] = "porto-json";

  // Copy no more than the length of the string above or the given maximum length
  const size_t len = std::min<size_t>(sizeof(plugin_name), maxlen);
  strncpy(name, plugin_name, len);

  // Ensure that the returned string is 0-terminated, no matter what length constraints it had
  name[len - 1] = '\0';

  // TODO: Return failure if the target string does not have sufficient space?
  return SOFTC_STATUS_OK;
}

int softc_plugin_capabilities() {
  return SOFTC_CAPABILITY_READ | SOFTC_CAPABILITY_WRITE;
}

// Loads from an URI into a datamodel
int softc_plugin_load( softc_datamodel_t* datamodel, const char* uri, const char* options ) {

  // TODO: This should be replaced by a generated c++ entity, i.e.:
  soft::Financial entity1;
  soft::Physics entity2;

  // Build up collection with structure
  soft::Collection coll;
  coll.attachEntity("finance", &entity1);
  coll.attachEntity("physics", &entity2);

  // Read from JSON
  QFile f(uri);
  if (!f.open(QIODevice::ReadOnly)) {
    return SOFTC_STATUS_FAILURE;
  }

  QByteArray ba = f.readAll();

  QJsonDocument doc = QJsonDocument::fromJson(ba);

  double amount = doc.object()["amount"].toDouble();
  double coefficient = doc.object()["coefficient"].toDouble();

  // Now store the items retrieved in the entities
  entity1.amount = amount;
  entity2.coefficient = coefficient;

  // ... and store the entire collection. The datamodel now contains the full
  // information from all the contained entities.
  coll.save(datamodel->ref);

  return SOFTC_STATUS_OK;
}

// Stores a datamodel into an URI
int softc_plugin_save( const softc_datamodel_t* datamodel, const char* uri, const char* options) {

  // TODO: This should be replaced by a generated c++ entity, i.e.:
  soft::Financial entity1;
  soft::Physics entity2;

  // Build up collection with structure
  soft::Collection coll;
  coll.attachEntity("finance", &entity1);
  coll.attachEntity("physics", &entity2);

  // This should now load and the contents from the datamodel into the collection
  // and its contained entities, as long as it was originally created with the
  // same structure:
  //   collection { "finance" -> Financial, "physics" -> SomeModelConfiguration }
  coll.load(datamodel->ref);

  // At this point the collection and its entities are fully popualted, and we can
  // write this to a json file.
  QUrl url(uri);

  // Assuming "amount" is in the first entity:
  double amount = entity1.amount; // TODO: Actual generated structure may vary
  double coefficient = entity2.coefficient;

  QJsonObject root;
  root["amount"] = amount;
  root["coefficient"] = coefficient;

  QJsonDocument doc(root);
  QFile f(url.path());
  if (!f.open(QIODevice::WriteOnly)) {
    return SOFTC_STATUS_FAILURE;
  }

  // Can f.write fail? If so, return error code
  f.write(doc.toJson());

  return SOFTC_STATUS_OK;
}

