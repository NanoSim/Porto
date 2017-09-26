#include <gtest/gtest.h>
#include <softc/softc.h>
#include <cstdint>

class DatamodelTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}    
};


typedef struct {
  const char *uuid;
  softc_storage_t *storage;
  softc_storage_strategy_t *strategy;
  softc_datamodel_t *datamodel;
} Model;


Model *create_model(const char *uuid)
{
  Model *m = (Model *)calloc(1, sizeof(Model));
  if (!m) return nullptr;

  m->storage = softc_storage_create("hdf5", "datamodel-test.h5", "");
  if (!m->storage) return nullptr;

  m->strategy = softc_storage_get_storage_strategy(m->storage);
  if (!m->strategy) return nullptr;

  m->datamodel = softc_storage_strategy_get_datamodel(m->strategy);
  if (!m->datamodel) return nullptr;

  softc_datamodel_set_id(m->datamodel, uuid);
  softc_datamodel_set_meta_name(m->datamodel, "softc-datamodel-test");
  softc_datamodel_set_meta_version(m->datamodel, "0.1");
  softc_datamodel_set_meta_namespace(m->datamodel, "http://sintef.no/SOFT");
  return m;
}

void free_model(Model *m)
{
  softc_storage_strategy_free_datamodel(m->datamodel);
  softc_storage_free_storage_strategy(m->strategy);
  softc_storage_free(m->storage);
  free(m);
}


TEST_F(DatamodelTest, store_and_load)
{
  size_t i, j, k;
  double v;
  bool stat;
  Model *m;
  char *uuid;
  
  char const *string_c = "This is a test...";
  char const *strlist_c[] = {"first", "second", "third"};
  const size_t strlist_n = sizeof(strlist_c)/sizeof(strlist_c[0]);
  double dbl = 3.14;
  double dbl_array[] = {0.0, 1.0, 2.0, 3.0};
  double **dbl_array2;
  double ***dbl_array3;
  uint32_t NI, NJ;
  
  /* assign dbl_array2 */
  double data2[5][3];
  double *ptr2[5];
  for (i=0, v=0.0; i<5; i++) {
    for (j=0; j<3; j++) {
      data2[i][j] = v;
      v += 1.0;
    }
    ptr2[i] = &data2[i][0];
  }
  dbl_array2 = ptr2;
  
  /* assign dbl_array3 */
  double data3[2][3][5];
  double *ptr3ij[2][3];
  double **ptr3i[2];
  for (i=0, v=0.0; i<2; i++) {
    for (j=0; j<3; j++) {
      for (k=0; k<5; k++) {
	data3[i][j][k] = v;
	v += 1.0;
      }
      ptr3ij[i][j] = &data3[i][j][0];
    }
    ptr3i[i] = &ptr3ij[i][0];
  }
  dbl_array3 = ptr3i;


  /* store */
  uuid = (char *)softc_uuidgen();                                                                                   ASSERT_TRUE(uuid);
  m = create_model(uuid);                                                                                           ASSERT_TRUE(m);
  stat = softc_datamodel_append_dimension(m->datamodel, "NI", 50);                                                  ASSERT_TRUE(stat);
  stat = softc_datamodel_append_dimension(m->datamodel, "NJ", 40);                                                  ASSERT_TRUE(stat);
  softc_string_s string = softc_string_create( string_c );
  stat = softc_datamodel_append_string(m->datamodel, "string", string);                                             ASSERT_TRUE(stat);
  softc_string_destroy( string );
  /*TODO: strlist type not implemented yet
  softc_string_s* strlist = softc_string_createlist( strlist_c, strlist_n );
  stat = softc_datamodel_append_string_list(m->datamodel, "strlist", strlist, strlist_n);                           ASSERT_TRUE(stat);
  softc_string_destroylist( strlist );
  */
  stat = softc_datamodel_append_double(m->datamodel, "dbl", dbl);                                                   ASSERT_TRUE(stat);
  stat = softc_datamodel_append_array_double(m->datamodel, "dbl_array", dbl_array, 4);                              ASSERT_TRUE(stat);
  // NB: soft assumes fortran order - reverse dimensions
  stat = softc_datamodel_append_array_double_2d(m->datamodel, "dbl_array2", (const double **)dbl_array2, 3, 5);     ASSERT_TRUE(stat);
  stat = softc_datamodel_append_array_double_3d(m->datamodel, "dbl_array3", (const double ***)dbl_array3, 5, 3, 2); ASSERT_TRUE(stat);
  softc_storage_strategy_store(m->strategy, m->datamodel);
  free_model(m);
  

  /* load */
  size_t ni, nj, nk;
  softc_string_s new_string;
  softc_string_s *new_strlist;
  double new_dbl;
  double *new_dbl_array;
  double **new_dbl_array2;
  double ***new_dbl_array3;
  
  m = create_model(uuid);
  softc_storage_strategy_start_retrieve(m->strategy, m->datamodel);

  stat = softc_datamodel_get_string(m->datamodel, "string", &new_string);
  ASSERT_TRUE(stat);
  ASSERT_EQ(strcmp(string_c, from_softc_string(new_string)), 0);
  softc_string_destroy(new_string);

  /*TODO: strlist type not implemented yet
  stat = softc_datamodel_get_string_list(m->datamodel, "strlist", &new_strlist, &ni);
  ASSERT_TRUE(stat);
  ASSERT_EQ(strlist_n, ni);
  for (i=0; i<ni; i++) ASSERT_EQ( strcmp(strlist_c[i], from_softc_string(new_strlist[i])), 0);
  softc_destroylist( new_strlist, ni );
  */
  
  stat = softc_datamodel_get_double(m->datamodel, "dbl", &new_dbl);
  ASSERT_TRUE(stat);
  ASSERT_EQ(dbl, new_dbl);

  stat = softc_datamodel_get_array_double(m->datamodel, "dbl_array", &new_dbl_array, &ni);
  ASSERT_TRUE(stat);
  ASSERT_EQ(4, ni);
  for (i=0; i<3; i++) ASSERT_EQ(dbl_array[i], new_dbl_array[i]);
  free(new_dbl_array);
  
  stat = softc_datamodel_get_array_double_2d(m->datamodel, "dbl_array2", &new_dbl_array2, &nj, &ni);
  ASSERT_TRUE(stat);
  ASSERT_EQ(5, ni);
  ASSERT_EQ(3, nj);
  for (i=0; i<ni; i++)
    for (j=0; j<nj; j++)
      ASSERT_EQ(dbl_array2[i][j], new_dbl_array2[i][j]);
  //free(new_dbl_array2);

  stat = softc_datamodel_get_array_double_3d(m->datamodel, "dbl_array3", &new_dbl_array3, &nk, &nj, &ni);
  ASSERT_TRUE(stat);
  ASSERT_EQ(2, ni);
  ASSERT_EQ(3, nj);
  ASSERT_EQ(5, nk);
  for (i=0; i<ni; i++)
    for (j=0; j<nj; j++)
      for (k=0; k<nk; k++)
	ASSERT_EQ(dbl_array3[i][j][k], new_dbl_array3[i][j][k]);
  //free(new_dbl_array3);

  stat = softc_datamodel_get_dimension(m->datamodel, "NI", &NI);  
  ASSERT_TRUE(stat);
  ASSERT_EQ(NI, 50);
  stat = softc_datamodel_get_dimension(m->datamodel, "NJ", &NJ);                                                  
  ASSERT_TRUE(stat);
  ASSERT_EQ(NJ, 40);

  softc_storage_strategy_end_retrieve(m->strategy, m->datamodel);
  free_model(m);
  free(uuid);
}
