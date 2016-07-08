#include <cstdint>
#include <algorithm>
#include <gtest/gtest.h>
#include <json.h>
#include <collection.h>
#include "financial.hxx"
#include "physics.hxx"
#include "json/jsonmodel.h"
#include "softc/softc-datamodel-private.hpp"

TEST(PortoJsonTests, TestIdentify) {
  {
    const char expected[] = "porto-json";
    const int maxlen = sizeof(expected);

    char name[maxlen + 1]; // String with a canari bird at the end
    name[maxlen] = '*'; // The function should never overwrite past maxlen!

    int error = softc_plugin_identify(name, maxlen);

    ASSERT_EQ(SOFTC_STATUS_OK, error);

    ASSERT_STREQ("porto-json", name);
    ASSERT_EQ('*', name[maxlen]);
  }

  {
    // Here we construct a too small buffer, but expect the canari to be intact as well
    // as a null-termination on the actual string
    const char expected[] = "porto-json";
    const int maxlen = sizeof(expected) - 2;

    char name[maxlen + 1];
    name[maxlen] = '*';

    int error = softc_plugin_identify(name, maxlen);

    ASSERT_EQ(SOFTC_STATUS_OK, error);

    ASSERT_STREQ("porto-js", name);
    ASSERT_EQ('*', name[maxlen]);
    ASSERT_EQ('\0', name[maxlen - 1]);
  }
}


TEST(PortoJsonTests, TestCapabilities)
{
  int capabilities = softc_plugin_capabilities();

  ASSERT_TRUE(capabilities & SOFTC_CAPABILITY_READ);
  ASSERT_TRUE(capabilities & SOFTC_CAPABILITY_WRITE);
}

TEST(PortoJsonTests, TestSaveLoad) {

  // TODO: This should be replaced by a generated c++ entity, i.e.:
  soft::Financial written_entity1;
  soft::Physics written_entity2;

  // Build up collection with structure
  soft::Collection written_coll;
  written_coll.attachEntity("finance", &written_entity1);
  written_coll.attachEntity("physics", &written_entity2);

  // Populate the entities
  written_entity1.amount = 20.9;
  written_entity2.coefficient = 0.5;

  // Construct a dummy data model and populate it
  soft::JSONModel j;
  written_coll.save(&j);

  // Construct a wrapper for the data model so that we can pass it to the c interface
  softc_datamodel_t d{&j};

  int error;
  const std::string uri = "test.json";

  // Check that it is possible to store the data in the given .json file
  error = softc_plugin_save(&d, uri.c_str(), "");
  ASSERT_EQ(SOFTC_STATUS_OK, error);

  // Construct a dummy target data model
  soft::JSONModel k;
  softc_datamodel_t e{&k};

  // Then check that we can read the data from the same .json file
  error = softc_plugin_load(&e, uri.c_str(), "");
  ASSERT_EQ(SOFTC_STATUS_OK, error);

  // Recreate an expected data collection
  soft::Financial read_entity1;
  soft::Physics read_entity2;

  // Build up collection with structure
  soft::Collection read_coll;
  read_coll.attachEntity("finance", &read_entity1);
  read_coll.attachEntity("physics", &read_entity2);

  read_coll.load(&k);

  // Finally, check that the read collection is the same as the written
  ASSERT_EQ(written_entity1.amount, read_entity1.amount);
  ASSERT_EQ(written_entity2.coefficient, read_entity2.coefficient);
}
