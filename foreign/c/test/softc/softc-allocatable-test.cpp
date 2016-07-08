#include <gtest/gtest.h>
#include <softc/softc-allocatable.h>

class AllocatableTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
};

TEST_F(AllocatableTest, rank1_double)
{
  auto ref = softc_allocatable_createv(double, 1, 3);
  double *vec = (double*)softc_allocatable_data(ref);
  vec[0] = 0.0;
  vec[1] = 1.0;
  vec[2] = 1.0;
  softc_allocatable_free(double, ref);
}

TEST_F(AllocatableTest, rank2_double)
{
  auto ref = softc_allocatable_createv(double, 2, 3, 2);
  double **vec = (double**)softc_allocatable_data(ref);
  vec[0][0] = 0.0;
  vec[1][0] = 1.0;
  softc_allocatable_free(double, ref);
}

TEST_F(AllocatableTest, rank3_double)
{
  auto ref = softc_allocatable_createv(double,3, 3, 2, 3);
  double ***vec = (double***)softc_allocatable_data(ref);
  vec[0][0][0] = 0.0;
  vec[1][0][1] = 1.0;
  softc_allocatable_free(double,ref);
}

TEST_F(AllocatableTest, rank4_double)
{
  auto ref = softc_allocatable_createv(double,4, 3, 2, 3, 3);
  double ****vec = (double****)softc_allocatable_data(ref);
  vec[0][0][0][0] = 0.0;
  vec[1][0][1][0] = 1.0;
  softc_allocatable_free(double,ref);
}

TEST_F(AllocatableTest, dimensions_double)
{
  size_t *dims;
  size_t rank;
  auto ref = softc_allocatable_createv(double,3, 300, 200, 89);
  softc_allocatable_dimensions(ref, &rank, &dims);
  ASSERT_EQ(dims[0], 300);
  ASSERT_EQ(dims[1], 200);
  ASSERT_EQ(dims[2], 89);
  ASSERT_EQ(rank, 3);
  softc_allocatable_free(double,ref);
}

TEST_F(AllocatableTest, createAndReshape_double)
{
  const size_t dims[2] = {3,4};
  const size_t dims2[1] = {12};
  const size_t rank = 2;
  auto alloc = softc_allocatable_create(double,rank, dims);
  double** vec = (double**)softc_allocatable_data(alloc);
  const double num = 1.1;

  vec[1][1] = num;
  
  softc_allocatable_reshape(double,alloc, 1, dims2);
  double *vec1 = (double*)softc_allocatable_data(alloc);

  ASSERT_EQ(vec1[4], num);
  softc_allocatable_free(double,alloc);
}

TEST_F(AllocatableTest, shallowCopy_double)
{
  auto v1 = softc_allocatable_createv(double,3, 300, 200, 89);
  auto v2 = softc_allocatable_shallow_copy(double,v1);
  double ***vec = (double***) softc_allocatable_data(v1);
  double ***copy = (double***) softc_allocatable_data(v2);

  vec[0][0][0] = 0.0;
  vec[10][10][10] = 10.1010;
  vec[1][10][100] = 1.10100;

  ASSERT_EQ(vec[0][0][0], copy[0][0][0]);
  ASSERT_EQ(vec[10][10][10], copy[10][10][10]);
  ASSERT_EQ(vec[1][10][100], copy[1][10][100]);

  softc_allocatable_free(double,v1);
  softc_allocatable_free(double,v2);
}

TEST_F(AllocatableTest, reshape_double)
{
  size_t dims[2] = {3,2};
  size_t newdims[1] = {6};
  double value   = 1.2345;
  auto alloc = softc_allocatable_create(double, 2, dims);
  double **vec1 = (double**)softc_allocatable_data(alloc);
  vec1[1][1] = value;
  softc_allocatable_reshape(double, alloc, 1, newdims);
  vec1 = NULL;
  double *vec = (double *)softc_allocatable_data(alloc);
  ASSERT_EQ(vec[4], value);

  softc_allocatable_free(double, alloc);
}


TEST_F(AllocatableTest, reshapev_double)
{
  double value   = 1.2345;
  auto alloc = softc_allocatable_createv(double, 2, 3, 2);
  double **vec1 = (double**)softc_allocatable_data(alloc);
  vec1[1][1] = value;
  softc_allocatable_reshapev(double, alloc, 1, 6);
  vec1 = NULL;
  double *vec = (double *)softc_allocatable_data(alloc);
  ASSERT_EQ(vec[4], value);

  softc_allocatable_free(double, alloc);
}

TEST_F(AllocatableTest, resize_double)
{
  size_t dims[2] = {3,2};
  size_t newdims[3] = {6,6,6};
  double value   = 1.2345;
  auto alloc = softc_allocatable_create(double, 2, dims);
  double **vec1 = (double**)softc_allocatable_data(alloc);
  vec1[1][1] = value;
  softc_allocatable_resize(double, alloc, 3, newdims);
  
  softc_allocatable_free(double, alloc);
}

TEST_F(AllocatableTest, resizev_double)
{
  double value   = 1.2345;
  auto alloc = softc_allocatable_createv(double, 2, 3, 2);
  double **vec1 = (double**)softc_allocatable_data(alloc);
  vec1[1][1] = value;
  softc_allocatable_resizev(double, alloc, 3, 6,6,6);
  double ***vec2 = (double***)softc_allocatable_data(alloc);
  vec2[0][0][0] = 0.00;
  vec2[1][1][1] = 1.11;
  vec2[2][2][2] = 2.22;
  vec2[3][3][3] = 3.33;
  vec2[4][4][4] = 4.44;
  vec2[5][5][5] = 5.55;
  softc_allocatable_free(double, alloc);
}


TEST_F(AllocatableTest, toStdVector1D_double)
{
  auto alloc = softc_allocatable_createv(double, 1,8);
  double *vec = (double*)softc_allocatable_data(alloc);
  for (unsigned int i = 0; i < 8; ++i) {
    vec[i] = 1.0 * (double)i;
  }
    
  std::vector<double> stdvec(vec, vec+8);
  for (unsigned int i = 0; i < 8; ++i) {
    ASSERT_EQ(vec[i], stdvec[i]);
  }

  vec[0] = 3.14;
  softc_allocatable_free(double, alloc);
}

TEST_F(AllocatableTest, toStdVector2D_double)
{
  unsigned int nj = 4;
  unsigned int ni = 3;  
  auto alloc = softc_allocatable_createv(double, 2,ni,nj);
  double **vec = (double**)softc_allocatable_data(alloc);
  for (unsigned int j = 0; j < nj; ++j) {
    for (unsigned int i = 0; i < ni; ++i) {
      vec[j][i] = 3.4 * (double)i + (double)j/2.0;
    }
  }

  std::vector< std::vector<double> > stdvec(nj);
  for (size_t j = 0; j < nj; ++j) {
    stdvec[j].assign(vec[j], vec[j] + ni);
  }

  for (unsigned int j = 0; j < nj; ++j) {
    for (unsigned int i = 0; i < ni; ++i) {
      ASSERT_EQ(vec[j][i], stdvec[j][i]);
    }
  }

  softc_allocatable_free(double, alloc);
}

TEST_F(AllocatableTest, toStdVector3D_double)
{
  unsigned int nk = 5;
  unsigned int nj = 4;
  unsigned int ni = 3;  
  auto alloc = softc_allocatable_createv(double, 3,ni,nj,nk);
  double ***vec = (double***)softc_allocatable_data(alloc);

  for (unsigned int k = 0; k < nk; ++k) {
    for (unsigned int j = 0; j < nj; ++j) {
      for (unsigned int i = 0; i < ni; ++i) {
	vec[k][j][i] = 3.4 * (double)i + (double)j/2.0 + (double)k*3.0;
      }
    }
  }

  std::vector<std::vector< std::vector<double> > >stdvec(nk);
  for (size_t k = 0; k < nk; ++k) {
    stdvec[k].resize(nj);
    for (size_t j = 0; j < nj; ++j) {
      stdvec[k][j].assign(vec[k][j], vec[k][j] + ni);
    }
  }
  
  for (unsigned int k = 0; k < nk; ++k) {
    for (unsigned int j = 0; j < nj; ++j) {
      for (unsigned int i = 0; i < ni; ++i) {
	ASSERT_EQ(vec[k][j][i], stdvec[k][j][i]);
      }
    }
  }

  softc_allocatable_free(double, alloc);
}


TEST_F(AllocatableTest, rank1_float)
{
  auto ref = softc_allocatable_createv(float, 1, 3);
  float *vec = (float*)softc_allocatable_data(ref);
  vec[0] = 0.0;
  vec[1] = 1.0;
  vec[2] = 1.0;
  softc_allocatable_free(float, ref);
}

TEST_F(AllocatableTest, rank2_float)
{
  auto ref = softc_allocatable_createv(float, 2, 3, 2);
  float **vec = (float**)softc_allocatable_data(ref);
  vec[0][0] = 0.0;
  vec[1][0] = 1.0;
  softc_allocatable_free(float, ref);
}

TEST_F(AllocatableTest, rank3_float)
{
  auto ref = softc_allocatable_createv(float,3, 3, 2, 3);
  float ***vec = (float***)softc_allocatable_data(ref);
  vec[0][0][0] = 0.0;
  vec[1][0][1] = 1.0;
  softc_allocatable_free(float,ref);
}

TEST_F(AllocatableTest, rank4_float)
{
  auto ref = softc_allocatable_createv(float,4, 3, 2, 3, 3);
  float ****vec = (float****)softc_allocatable_data(ref);
  vec[0][0][0][0] = 0.0;
  vec[1][0][1][0] = 1.0;
  softc_allocatable_free(float,ref);
}

TEST_F(AllocatableTest, dimensions_float)
{
  size_t *dims;
  size_t rank;
  auto ref = softc_allocatable_createv(float,3, 300, 200, 89);
  softc_allocatable_dimensions(ref, &rank, &dims);
  ASSERT_EQ(dims[0], 300);
  ASSERT_EQ(dims[1], 200);
  ASSERT_EQ(dims[2], 89);
  ASSERT_EQ(rank, 3);
  softc_allocatable_free(float,ref);
}

TEST_F(AllocatableTest, createAndReshape_float)
{
  const size_t dims[2] = {3,4};
  const size_t dims2[1] = {12};
  const size_t rank = 2;
  auto alloc = softc_allocatable_create(float,rank, dims);
  float** vec = (float**)softc_allocatable_data(alloc);
  const float num = 1.1;

  vec[1][1] = num;
  
  softc_allocatable_reshape(float,alloc, 1, dims2);
  float *vec1 = (float*)softc_allocatable_data(alloc);

  ASSERT_EQ(vec1[4], num);
  softc_allocatable_free(float,alloc);
}

TEST_F(AllocatableTest, shallowCopy_float)
{
  auto v1 = softc_allocatable_createv(float,3, 300, 200, 89);
  auto v2 = softc_allocatable_shallow_copy(float,v1);
  float ***vec = (float***) softc_allocatable_data(v1);
  float ***copy = (float***) softc_allocatable_data(v2);

  vec[0][0][0] = 0.0;
  vec[10][10][10] = 10.1010;
  vec[1][10][100] = 1.10100;

  ASSERT_EQ(vec[0][0][0], copy[0][0][0]);
  ASSERT_EQ(vec[10][10][10], copy[10][10][10]);
  ASSERT_EQ(vec[1][10][100], copy[1][10][100]);

  softc_allocatable_free(float,v1);
  softc_allocatable_free(float,v2);
}

TEST_F(AllocatableTest, reshape_float)
{
  size_t dims[2] = {3,2};
  size_t newdims[1] = {6};
  float value   = 1.2345;
  auto alloc = softc_allocatable_create(float, 2, dims);
  float **vec1 = (float**)softc_allocatable_data(alloc);
  vec1[1][1] = value;
  softc_allocatable_reshape(float, alloc, 1, newdims);
  vec1 = NULL;
  float *vec = (float *)softc_allocatable_data(alloc);
  ASSERT_EQ(vec[4], value);

  softc_allocatable_free(float, alloc);
}


TEST_F(AllocatableTest, reshapev_float)
{
  float value   = 1.2345;
  auto alloc = softc_allocatable_createv(float, 2, 3, 2);
  float **vec1 = (float**)softc_allocatable_data(alloc);
  vec1[1][1] = value;
  softc_allocatable_reshapev(float, alloc, 1, 6);
  vec1 = NULL;
  float *vec = (float *)softc_allocatable_data(alloc);
  ASSERT_EQ(vec[4], value);

  softc_allocatable_free(float, alloc);
}

TEST_F(AllocatableTest, toStdVector1D_float)
{
  auto alloc = softc_allocatable_createv(float, 1,8);
  float *vec = (float*)softc_allocatable_data(alloc);
  for (unsigned int i = 0; i < 8; ++i) {
    vec[i] = 1.0 * (float)i;
  }
    
  std::vector<float> stdvec(vec, vec+8);
  for (unsigned int i = 0; i < 8; ++i) {
    ASSERT_EQ(vec[i], stdvec[i]);
  }

  vec[0] = 3.14;
  softc_allocatable_free(float, alloc);
}

TEST_F(AllocatableTest, toStdVector2D_float)
{
  unsigned int nj = 4;
  unsigned int ni = 3;  
  auto alloc = softc_allocatable_createv(float, 2,ni,nj);
  float **vec = (float**)softc_allocatable_data(alloc);
  for (unsigned int j = 0; j < nj; ++j) {
    for (unsigned int i = 0; i < ni; ++i) {
      vec[j][i] = 3.4 * (float)i + (float)j/2.0;
    }
  }

  std::vector< std::vector<float> > stdvec(nj);
  for (size_t j = 0; j < nj; ++j) {
    stdvec[j].assign(vec[j], vec[j] + ni);
  }

  for (unsigned int j = 0; j < nj; ++j) {
    for (unsigned int i = 0; i < ni; ++i) {
      ASSERT_EQ(vec[j][i], stdvec[j][i]);
    }
  }

  softc_allocatable_free(float, alloc);
}

TEST_F(AllocatableTest, toStdVector3D_float)
{
  unsigned int nk = 5;
  unsigned int nj = 4;
  unsigned int ni = 3;  
  auto alloc = softc_allocatable_createv(float, 3,ni,nj,nk);
  float ***vec = (float***)softc_allocatable_data(alloc);

  for (unsigned int k = 0; k < nk; ++k) {
    for (unsigned int j = 0; j < nj; ++j) {
      for (unsigned int i = 0; i < ni; ++i) {
	vec[k][j][i] = 3.4 * (float)i + (float)j/2.0 + (float)k*3.0;
      }
    }
  }

  std::vector<std::vector< std::vector<float> > >stdvec(nk);
  for (size_t k = 0; k < nk; ++k) {
    stdvec[k].resize(nj);
    for (size_t j = 0; j < nj; ++j) {
      stdvec[k][j].assign(vec[k][j], vec[k][j] + ni);
    }
  }
  
  for (unsigned int k = 0; k < nk; ++k) {
    for (unsigned int j = 0; j < nj; ++j) {
      for (unsigned int i = 0; i < ni; ++i) {
	ASSERT_EQ(vec[k][j][i], stdvec[k][j][i]);
      }
    }
  }

  softc_allocatable_free(float, alloc);
}

TEST_F(AllocatableTest, rank1_int)
{
  auto ref = softc_allocatable_createv(int, 1, 3);
  int *vec = (int*)softc_allocatable_data(ref);
  vec[0] = 0;
  vec[1] = 1;
  vec[2] = 2;
  softc_allocatable_free(int, ref);
}

TEST_F(AllocatableTest, rank2_int)
{
  auto ref = softc_allocatable_createv(int, 2, 3, 2);
  int **vec = (int**)softc_allocatable_data(ref);
  vec[0][0] = 1;
  vec[1][0] = 2;
  softc_allocatable_free(int, ref);
}

TEST_F(AllocatableTest, rank3_int)
{
  auto ref = softc_allocatable_createv(int,3, 3, 2, 3);
  int ***vec = (int***)softc_allocatable_data(ref);
  vec[0][0][0] = 1;
  vec[1][0][1] = 2;
  softc_allocatable_free(int,ref);
}

TEST_F(AllocatableTest, rank4_int)
{
  auto ref = softc_allocatable_createv(int,4, 3, 2, 3, 3);
  int ****vec = (int****)softc_allocatable_data(ref);
  vec[0][0][0][0] = 1;
  vec[1][0][1][0] = 2;
  softc_allocatable_free(int,ref);
}

TEST_F(AllocatableTest, dimensions_int)
{
  size_t *dims;
  size_t rank;
  auto ref = softc_allocatable_createv(int,3, 300, 200, 89);
  softc_allocatable_dimensions(ref, &rank, &dims);
  ASSERT_EQ(dims[0], 300);
  ASSERT_EQ(dims[1], 200);
  ASSERT_EQ(dims[2], 89);
  ASSERT_EQ(rank, 3);
  softc_allocatable_free(int,ref);
}

TEST_F(AllocatableTest, createAndReshape_int)
{
  const size_t dims[2] = {3,4};
  const size_t dims2[1] = {12};
  const size_t rank = 2;
  auto alloc = softc_allocatable_create(int,rank, dims);
  int** vec = (int**)softc_allocatable_data(alloc);
  const int num = 43;

  vec[1][1] = num;
  
  softc_allocatable_reshape(int,alloc, 1, dims2);
  int *vec1 = (int*)softc_allocatable_data(alloc);

  ASSERT_EQ(vec1[4], num);
  softc_allocatable_free(int,alloc);
}

TEST_F(AllocatableTest, shallowCopy_int)
{
  auto v1 = softc_allocatable_createv(int,3, 300, 200, 89);
  auto v2 = softc_allocatable_shallow_copy(int,v1);
  int ***vec = (int***) softc_allocatable_data(v1);
  int ***copy = (int***) softc_allocatable_data(v2);

  vec[0][0][0] = 0;
  vec[10][10][10] = 101;
  vec[1][10][100] = 11;

  ASSERT_EQ(vec[0][0][0], copy[0][0][0]);
  ASSERT_EQ(vec[10][10][10], copy[10][10][10]);
  ASSERT_EQ(vec[1][10][100], copy[1][10][100]);

  softc_allocatable_free(int,v1);
  softc_allocatable_free(int,v2);
}

TEST_F(AllocatableTest, reshape_int)
{
  size_t dims[2] = {3,2};
  size_t newdims[1] = {6};
  int value   = 12345;
  auto alloc = softc_allocatable_create(int, 2, dims);
  int **vec1 = (int**)softc_allocatable_data(alloc);
  vec1[1][1] = value;
  softc_allocatable_reshape(int, alloc, 1, newdims);
  vec1 = NULL;
  int *vec = (int *)softc_allocatable_data(alloc);
  ASSERT_EQ(vec[4], value);

  softc_allocatable_free(int, alloc);
}


TEST_F(AllocatableTest, reshapev_int)
{
  int value   = 12345;
  auto alloc = softc_allocatable_createv(int, 2, 3, 2);
  int **vec1 = (int**)softc_allocatable_data(alloc);
  vec1[1][1] = value;
  softc_allocatable_reshapev(int, alloc, 1, 6);
  vec1 = NULL;
  int *vec = (int *)softc_allocatable_data(alloc);
  ASSERT_EQ(vec[4], value);

  softc_allocatable_free(int, alloc);
}

TEST_F(AllocatableTest, toStdVector1D_int)
{
  auto alloc = softc_allocatable_createv(int, 1,8);
  int *vec = (int*)softc_allocatable_data(alloc);
  for (unsigned int i = 0; i < 8; ++i) {
    vec[i] = 2 * i;
  }
    
  std::vector<int> stdvec(vec, vec+8);
  for (unsigned int i = 0; i < 8; ++i) {
    ASSERT_EQ(vec[i], stdvec[i]);
  }

  softc_allocatable_free(int, alloc);
}

TEST_F(AllocatableTest, toStdVector2D_int)
{
  unsigned int nj = 4;
  unsigned int ni = 3;  
  auto alloc = softc_allocatable_createv(int, 2,ni,nj);
  int **vec = (int**)softc_allocatable_data(alloc);
  for (unsigned int j = 0; j < nj; ++j) {
    for (unsigned int i = 0; i < ni; ++i) {
      vec[j][i] = 4 * (int)i + (int)j/2;
    }
  }

  std::vector< std::vector<int> > stdvec(nj);
  for (size_t j = 0; j < nj; ++j) {
    stdvec[j].assign(vec[j], vec[j] + ni);
  }

  for (unsigned int j = 0; j < nj; ++j) {
    for (unsigned int i = 0; i < ni; ++i) {
      ASSERT_EQ(vec[j][i], stdvec[j][i]);
    }
  }

  softc_allocatable_free(int, alloc);
}

TEST_F(AllocatableTest, toStdVector3D_int)
{
  unsigned int nk = 5;
  unsigned int nj = 4;
  unsigned int ni = 3;  
  auto alloc = softc_allocatable_createv(int, 3,ni,nj,nk);
  int ***vec = (int***)softc_allocatable_data(alloc);

  for (unsigned int k = 0; k < nk; ++k) {
    for (unsigned int j = 0; j < nj; ++j) {
      for (unsigned int i = 0; i < ni; ++i) {
	vec[k][j][i] = 34 * (int)i + (int)j/20 + (int)k*30;
      }
    }
  }

  std::vector<std::vector< std::vector<int> > >stdvec(nk);
  for (size_t k = 0; k < nk; ++k) {
    stdvec[k].resize(nj);
    for (size_t j = 0; j < nj; ++j) {
      stdvec[k][j].assign(vec[k][j], vec[k][j] + ni);
    }
  }
  
  for (unsigned int k = 0; k < nk; ++k) {
    for (unsigned int j = 0; j < nj; ++j) {
      for (unsigned int i = 0; i < ni; ++i) {
	ASSERT_EQ(vec[k][j][i], stdvec[k][j][i]);
      }
    }
  }

  softc_allocatable_free(int, alloc);
}
