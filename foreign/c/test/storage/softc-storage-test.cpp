#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <softc/softc.h>
#include <vector>
#include <softc/softc-utils.h>
#include <softc/softc-datamodel.h>
#include <softc/softc-storage.h>
#include <softc/softc-collection.h>

template <typename T>
void arrayToPtr(T **dest, std::vector<T> const &source)
{
  const int size = source.size();
  (*dest) = new T[size];
  std::copy(source.cbegin(),source.cend(), *dest);
}

template <typename T>
void arrayToPtr(T ***dest, std::vector<std::vector<T>> const &source)
{
  *dest = new T*[source.size()];
  auto ptr = *dest;
  for(auto &s: source) {
    arrayToPtr(&(*ptr++), s);
  }
}

template <typename T>
void arrayToPtr(T ****dest, std::vector<std::vector<std::vector<T>>> const &source)
{
  *dest = new T**[source.size()];
  auto ptr = *dest;
  for(auto &s: source) {
    arrayToPtr(&(*ptr++), s);
  }
}

template <typename T>
void ptrToArray(std::vector<T> &dest, const T * const src, size_t length)
{
  dest.resize(length);
  dest.assign(src, src+length);
}

template <typename T>
void ptrToArray(std::vector<std::vector<T> > &dest, const T ** const src, size_t i, size_t j)
{
  dest.resize(j);
  for (size_t idx = 0; idx < j; ++idx) {
    ptrToArray(dest[idx], src[idx], i);
  }
}

template <typename T>
void ptrToArray(std::vector<std::vector<std::vector<T> > > &dest, const T *** const src, size_t i, size_t j, size_t k)
{
  dest.resize(k);
  for (size_t idx = 0; idx < k; ++idx) {
      ptrToArray(dest[idx], src[idx], i, j);
  }
}

class SoftC_StorageTest : public ::testing::Test {
protected:
  SoftC_StorageTest()
    : hello_string("Hello softc")
    , int32_number(42)
    , double_number(3.14)
    , intvec({0,1,2,3,4,5,6,7,8,9})
    , doublevec({0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9})
    , doublevec2d({{0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9},
	           {1.01,1.02,1.03,1.04,1.05,1.06,1.07,1.08,1.09},
	           {2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9},
	           {3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9}})
    , doublevec3d({
	{{0.1, 0.2, 0.3},
	 {0.1, 0.2, 0.3},
	 {0.1, 0.2, 0.3}},
	{{1.1, 0.2, 0.3},
	 {1.1, 0.2, 0.3},
	 {1.1, 0.2, 0.3}},
	{{2.1, 0.2, 0.3},
	 {2.1, 0.2, 0.3},
	 {2.1, 0.2, 0.3}}})
    , strlist({"this is", "a test"})

  {}

  ~SoftC_StorageTest()
  {}
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}

  const char *hello_string;
  const int32_t int32_number;
  const double double_number;
  const std::vector<int32_t> intvec;
  const std::vector<double> doublevec;
  const std::vector<std::vector<double> > doublevec2d;
  const std::vector<std::vector<std::vector<double> > > doublevec3d;
  const std::vector<std::string> strlist;

public:
  static const char *uuid_string;
};

const char *SoftC_StorageTest::uuid_string = softc_uuidgen();

typedef char* soft_string;
typedef double* soft_double_array;

TEST_F(SoftC_StorageTest, writeData)
{
  auto storage  = softc_storage_create("hdf5", "test.h5", "");
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);
  double **v2d;
  double ***v3d;
  bool isOk;
  softc_string_list_s *strList;

  arrayToPtr(&v3d, doublevec3d);
  arrayToPtr(&v2d, doublevec2d);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");
  softc_datamodel_set_meta_namespace(model, "meta");

  softc_string_s hello_s = softc_string_create(hello_string);
  isOk = softc_datamodel_append_string(model, "message", hello_s); ASSERT_TRUE(isOk);
  softc_string_destroy(hello_s);
  isOk = softc_datamodel_append_int32(model, "int32-number", int32_number); ASSERT_TRUE(isOk);
  isOk = softc_datamodel_append_double(model, "double-number", double_number); ASSERT_TRUE(isOk);
  isOk = softc_datamodel_append_array_int32(model, "int-array", intvec.data(), intvec.size()); ASSERT_TRUE(isOk);
  isOk = softc_datamodel_append_array_double(model, "double-array", doublevec.data(), doublevec.size()); ASSERT_TRUE(isOk);
  isOk = softc_datamodel_append_array_double_2d(model, "double-array-2d", (const double**)v2d, doublevec2d[0].size(), doublevec2d.size()); ASSERT_TRUE(isOk);
  isOk = softc_datamodel_append_array_double_3d(model, "double-array-3d", (const double***)v3d, doublevec3d[0][0].size(), doublevec3d[0].size(), doublevec3d.size()); ASSERT_TRUE(isOk);

  strList = softc_string_list_create();
  size_t n=0;
  for (const auto& it: strlist) {
     softc_string_s s = softc_string_create(it.c_str());
     softc_string_list_append(strList, s);
     n++;
  }
  ASSERT_EQ(n, strlist.size());
  ASSERT_EQ(n, softc_string_list_count(strList));

  isOk = softc_datamodel_append_string_list(model, "string-list", strList); ASSERT_TRUE(isOk);

  softc_storage_strategy_store(strategy, model);
}

TEST_F(SoftC_StorageTest, readString)
{
  softc_string_s message;

  auto storage  = softc_storage_create("hdf5", "test.h5", NULL);
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  auto isOk = softc_datamodel_get_string(model, "message", &message);
  ASSERT_TRUE(isOk);
  softc_storage_strategy_end_retrieve(strategy, model);

  ASSERT_STREQ(from_softc_string(message), hello_string);
  softc_string_destroy(message);
}

TEST_F(SoftC_StorageTest, readInt32)
{
  int32_t x;

  auto storage  = softc_storage_create("hdf5", "test.h5", NULL);
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  auto isOk = softc_datamodel_get_int32(model, "int32-number", &x); ASSERT_TRUE(isOk);
  softc_storage_strategy_end_retrieve(strategy, model);

  ASSERT_EQ(x, int32_number);
}

TEST_F(SoftC_StorageTest, readDouble)
{
  double x;

  auto storage  = softc_storage_create("hdf5", "test.h5", NULL);
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  auto isOk = softc_datamodel_get_double(model, "double-number", &x);
  ASSERT_TRUE(isOk);
  softc_storage_strategy_end_retrieve(strategy, model);

  ASSERT_EQ(x, double_number);
}

TEST_F(SoftC_StorageTest, readIntVec)
{
  int32_t *intarray;
  size_t intarray_size;
  std::vector<int32_t> intvec_cmp;

  auto storage  = softc_storage_create("hdf5", "test.h5", NULL);
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  auto isOk = softc_datamodel_get_array_int32(model, "int-array", &intarray, &intarray_size);
  ASSERT_TRUE(isOk);
  softc_storage_strategy_end_retrieve(strategy, model);
  ASSERT_EQ(intarray_size, intvec.size());
  ptrToArray(intvec_cmp, intarray, intarray_size);
  ASSERT_EQ(intvec, intvec_cmp);
}

TEST_F(SoftC_StorageTest, doubleVec)
{
  double *doublearray;
  size_t doublearray_size;
  std::vector<double> doublevec_cmp;

  auto storage  = softc_storage_create("hdf5", "test.h5", "");
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  auto isOk = softc_datamodel_get_array_double(model, "double-array", &doublearray, &doublearray_size);
  ASSERT_TRUE(isOk);
  softc_storage_strategy_end_retrieve(strategy, model);
  ASSERT_EQ(doublearray_size, doublevec.size());
  ptrToArray(doublevec_cmp, doublearray, doublearray_size);
  ASSERT_EQ(doublevec, doublevec_cmp);
}

TEST_F(SoftC_StorageTest, doubleVec2D)
{
  double **da2;
  size_t da2_i, da2_j;

  std::vector<std::vector<double> > doublevec2d_cmp;

  auto storage  = softc_storage_create("hdf5", "test.h5", "");
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  auto isOk = softc_datamodel_get_array_double_2d(model, "double-array-2d", &da2, &da2_i, &da2_j);
  ASSERT_TRUE(isOk);
  softc_storage_strategy_end_retrieve(strategy, model);

  ptrToArray(doublevec2d_cmp, (const double**)da2, da2_i, da2_j);
  ASSERT_EQ(doublevec2d, doublevec2d_cmp);
}

TEST_F(SoftC_StorageTest, doubleVec3D)
{
  double ***da3;
  size_t da3_i, da3_j, da3_k;

  std::vector<std::vector<std::vector<double> > > doublevec3d_cmp;

  auto storage  = softc_storage_create("hdf5", "test.h5", "");
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  auto isOk = softc_datamodel_get_array_double_3d(model, "double-array-3d", &da3, &da3_i, &da3_j, &da3_k);
  ASSERT_TRUE(isOk);
  softc_storage_strategy_end_retrieve(strategy, model);
  ptrToArray(doublevec3d_cmp, (const double***)da3, da3_i, da3_j, da3_k);
  ASSERT_EQ(doublevec3d, doublevec3d_cmp);
}

TEST_F(SoftC_StorageTest, strList)
{
  softc_string_list_s *slist = nullptr;

  auto storage  = softc_storage_create("hdf5", "test.h5", "");
  ASSERT_TRUE(storage != nullptr);
  auto strategy = softc_storage_get_storage_strategy(storage);
  ASSERT_TRUE(strategy != nullptr);
  auto model    = softc_storage_strategy_get_datamodel(strategy);
  ASSERT_TRUE(model != nullptr);

  softc_datamodel_set_id (model, uuid_string);
  softc_datamodel_set_meta_name(model, "meta");
  softc_datamodel_set_meta_version(model, "meta");

  softc_storage_strategy_start_retrieve(strategy, model);
  auto isOk = softc_datamodel_get_string_list(model, "string-list", &slist);
  ASSERT_TRUE(isOk);
  size_t n_elements = softc_string_list_count(slist);
  ASSERT_EQ(n_elements, strlist.size());
  softc_storage_strategy_end_retrieve(strategy, model);
  for (size_t i = 0; i < strlist.size(); ++i) {
    softc_string_s s = softc_string_at(slist, i);
    ASSERT_STREQ( from_softc_string(s), strlist[i].c_str());
  }
  softc_string_list_free(slist);
}

TEST_F(SoftC_StorageTest, collectionStorage)
{
  auto storage  = softc_storage_create("hdf5", "coll.h5", "");
  ASSERT_TRUE(storage != nullptr);
  auto collection = softc_collection_create_new();
  ASSERT_TRUE(collection != nullptr);

  softc_collection_set_name(collection, "Mine");
  softc_collection_set_version(collection, "V1");
  softc_storage_save(storage, (const softc_entity_t*) collection);

  softc_collection_free(collection);
  softc_storage_free(storage);
}

TEST_F(SoftC_StorageTest, collectionRetrieval)
{
  auto storage  = softc_storage_create("hdf5", "retrieve-test.h5", "");
  ASSERT_TRUE(storage != nullptr);
  auto collection = softc_collection_create_new();
  ASSERT_TRUE(collection != nullptr);
  softc_collection_add_relation(collection, "apple", "is", "fruit");
  softc_collection_add_relation(collection, "pear", "is", "fruit");
  softc_storage_save(storage, (const softc_entity_t*)collection);

  auto id = softc_entity_get_id((const softc_entity_t*)collection);
  auto collection_copy = softc_collection_create (id);
  softc_storage_load(storage, (softc_entity_t*)collection_copy);

  auto lst = softc_collection_find_relations(collection_copy, "fruit", "^is");
  ASSERT_EQ(2, softc_string_list_count(lst));
  const char *expected_names[] = {"apple", "pear"};
  for (int i=0; i < 2; i++) {
    int found = 0;
    for (int j=0; j < softc_string_list_count(lst); j++)
      if (strcmp(expected_names[i],
		 from_softc_string(softc_string_at(lst, j))) == 0) found = 1;
    ASSERT_TRUE(found);
  }
  softc_string_list_free(lst);

  softc_collection_free(collection);
  softc_collection_free(collection_copy);
  softc_storage_free(storage);

}
