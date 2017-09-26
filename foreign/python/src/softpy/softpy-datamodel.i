/* -*- C -*-  (not really, but good for syntax highlighting) */


/*
 * Wrap softc_datamodel_append_*()
 * -------------------------------
 */
%apply (softc_bytearray_s IN_SOFTBYTEARRAY)                              {(const softc_bytearray_s value)};
%apply (softc_string_s IN_SOFTSTRING)                                    {(const softc_string_s value)};
%apply (softc_string_list_s *IN_SOFTSTRINGLIST)                          {(const softc_string_list_s *value)};
%apply (int32_t       *IN_ARRAY1, size_t DIM1)                           {(const int32_t  *value, size_t size)};
%apply (double        *IN_ARRAY1, size_t DIM1)                           {(const double   *value, size_t size)};
%apply (double      **IN_ARRAY2D, size_t DIM1, size_t DIM2)              {(const double* const*value, size_t size_i, size_t size_j)};
%apply (double     ***IN_ARRAY3D, size_t DIM1, size_t DIM2, size_t DIM3) {(const double* const* const*value, size_t size_i, size_t size_j, size_t size_k)};
bool softc_datamodel_append_dimension      (softc_datamodel_t *model, const char *key, uint32_t value);
bool softc_datamodel_append_string         (softc_datamodel_t *model, const char *key, const softc_string_s value);
bool softc_datamodel_append_int8           (softc_datamodel_t *model, const char *key, int8_t value);
bool softc_datamodel_append_uint8          (softc_datamodel_t *model, const char *key, uint8_t value);
bool softc_datamodel_append_int16          (softc_datamodel_t *model, const char *key, int16_t value);
bool softc_datamodel_append_uint16         (softc_datamodel_t *model, const char *key, uint16_t value);
bool softc_datamodel_append_int32          (softc_datamodel_t *model, const char *key, int32_t value);
bool softc_datamodel_append_uint32         (softc_datamodel_t *model, const char *key, uint32_t value);
bool softc_datamodel_append_int64          (softc_datamodel_t *model, const char *key, int64_t value);
bool softc_datamodel_append_uint64         (softc_datamodel_t *model, const char *key, uint64_t value);
bool softc_datamodel_append_float          (softc_datamodel_t *model, const char *key, float value);
bool softc_datamodel_append_double         (softc_datamodel_t *model, const char *key, double value);
bool softc_datamodel_append_bool           (softc_datamodel_t *model, const char *key, bool value);
bool softc_datamodel_append_blob           (softc_datamodel_t *model, const char *key, const softc_bytearray_s value);
bool softc_datamodel_append_string_list    (softc_datamodel_t *model, const char *key, const softc_string_list_s *value);
bool softc_datamodel_append_array_int32    (softc_datamodel_t *model, const char *key, const int32_t  *value, size_t size);
bool softc_datamodel_append_array_double   (softc_datamodel_t *model, const char *key, const double   *value, size_t size);
bool softc_datamodel_append_array_double_2d (softc_datamodel_t *model, const char *key, const double* const* value, size_t size_i, size_t size_j);
bool softc_datamodel_append_array_double_3d (softc_datamodel_t *model, const char *key, const double* const* const* value, size_t size_i, size_t size_j, size_t size_k);
%clear (softc_bytearray_s *value);
%clear (softc_string_s value);
%clear (softc_string_list_s *value);
%clear (unsigned char *value, size_t length);
%clear (int32_t       *value, size_t size);
%clear (double        *value, size_t size);
%clear (double       **value, size_t size_i, size_t size_j);
%clear (double      ***value, size_t size_i, size_t size_j, size_t size_k);

/*
 * Wrap softc_datamodel_get_*()
 * ----------------------------
 */
/*
%typemap(in,numinputs=0) char **value (char *temp)        { $1 = &temp; };
%typemap(argout)         char **value                     { $result = PyString_FromString(*$1); };
*/
%typemap(in,numinputs=0) int8_t *value (int8_t temp)      { $1 = &temp; };
%typemap(argout)         int8_t *value                    { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint8_t *value (uint8_t temp)    { $1 = &temp; };
%typemap(argout)         uint8_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int16_t *value (int16_t temp)    { $1 = &temp; };
%typemap(argout)         int16_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint16_t *value (uint16_t temp)  { $1 = &temp; };
%typemap(argout)         uint16_t *value                  { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int32_t *value (int32_t temp)    { $1 = &temp; };
%typemap(argout)         int32_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint32_t *value (uint32_t temp)  { $1 = &temp; };
%typemap(argout)         uint32_t *value                  { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) int64_t *value (int64_t temp)    { $1 = &temp; };
%typemap(argout)         int64_t *value                   { $result = PyInt_FromLong(*$1); };

%typemap(in,numinputs=0) uint64_t *value (uint64_t temp)  { $1 = &temp; };
%typemap(argout)         uint64_t *value                  { $result = PyLong_FromUnsignedLong(*$1); };

%typemap(in,numinputs=0) float *value (float temp)        { $1 = &temp; };
%typemap(argout)         float *value                     { $result = PyFloat_FromDouble(*$1); };

%typemap(in,numinputs=0) double *value (double temp)      { $1 = &temp; };
%typemap(argout)         double *value                    { $result = PyFloat_FromDouble(*$1); };

%typemap(in,numinputs=0) bool *value (bool temp)          { $1 = &temp; };
%typemap(argout)         bool *value                      { $result = PyBool_FromLong(*$1); };


%apply (softc_bytearray_s *ARGOUT_SOFTBYTEARRAY)          {(softc_bytearray_s *value)};
%apply (softc_string_s *ARGOUT_SOFTSTRING)                {(softc_string_s *value)};
%apply (softc_string_list_s **ARGOUT_SOFTSTRINGLIST)      {(softc_string_list_s **value)};
%apply (unsigned char       **ARGOUT_BYTES, size_t *LEN)  {(unsigned char **value, size_t *length)};
%apply (unsigned char **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(unsigned char **value, size_t *size)};
%apply (int32_t       **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(int32_t       **value, size_t *size)};
%apply (double        **ARGOUTVIEWM_ARRAY1, size_t *DIM1) {(double        **value, size_t *size)};
%apply (double           ***ARGOUT_ARRAY2D, size_t *DIM1, size_t *DIM2) {(double       ***value, size_t *size_i, size_t *size_j)};
%apply (double          ****ARGOUT_ARRAY3D, size_t *DIM1, size_t *DIM2, size_t *DIM3) {(double        ****value, size_t *size_i, size_t *size_j, size_t *size_k)};
bool softc_datamodel_get_dimension       (const softc_datamodel_t *model, const char *key, uint32_t *value);
bool softc_datamodel_get_string          (const softc_datamodel_t *model, const char *key, softc_string_s *value);
bool softc_datamodel_get_int8            (const softc_datamodel_t *model, const char *key, int8_t *value);
bool softc_datamodel_get_uint8           (const softc_datamodel_t *model, const char *key, uint8_t *value);
bool softc_datamodel_get_int16           (const softc_datamodel_t *model, const char *key, int16_t *value);
bool softc_datamodel_get_uint16          (const softc_datamodel_t *model, const char *key, uint16_t *value);
bool softc_datamodel_get_int32           (const softc_datamodel_t *model, const char *key, int32_t *value);
bool softc_datamodel_get_uint32          (const softc_datamodel_t *model, const char *key, uint32_t *value);
bool softc_datamodel_get_int64           (const softc_datamodel_t *model, const char *key, int64_t *value);
bool softc_datamodel_get_uint64          (const softc_datamodel_t *model, const char *key, uint64_t *value);
bool softc_datamodel_get_float           (const softc_datamodel_t *model, const char *key, float *value);
bool softc_datamodel_get_double          (const softc_datamodel_t *model, const char *key, double *value);
bool softc_datamodel_get_bool            (const softc_datamodel_t *model, const char *key, bool *value);
bool softc_datamodel_get_blob            (const softc_datamodel_t *model, const char *key, softc_bytearray_s *value);
bool softc_datamodel_get_string_list     (const softc_datamodel_t *model, const char *key, softc_string_list_s **value);
bool softc_datamodel_get_array_int32     (const softc_datamodel_t *model, const char *key, int32_t **value, size_t *size);
bool softc_datamodel_get_array_double    (const softc_datamodel_t *model, const char *key, double **value, size_t *size);
bool softc_datamodel_get_array_double_2d (const softc_datamodel_t *model, const char *key, double ***value, size_t *size_i, size_t *size_j);
bool softc_datamodel_get_array_double_3d (const softc_datamodel_t *model, const char *key, double ****value, size_t *size_i, size_t *size_j, size_t *size_k);
%clear int8_t *value;
%clear uint8_t *value;
%clear int16_t *value;
%clear uint16_t *value;
%clear int32_t *value;
%clear uint32_t *value;
%clear int64_t *value;
%clear uint64_t *value;
%clear float *value;
%clear double *value;
%clear bool *value;
//%clear (char ***value, size_t *n_elements);
%clear (softc_string_s *value);
%clear (unsigned char **value, size_t *length);
%clear (int32_t **value, size_t *size);
%clear (double **value, size_t *size);
%clear (double ***value, size_t *size_i, size_t *size_j);
%clear (double ****value, size_t *size_i, size_t *size_j, size_t *size_k);


/*
 * Wrap other functions
 * --------------------
 */
bool softc_datamodel_set_id              (softc_datamodel_t* model, const char *id);
bool softc_datamodel_set_meta_name       (softc_datamodel_t* model, const char *meta_name);
bool softc_datamodel_set_meta_version    (softc_datamodel_t* model, const char *meta_version);
bool softc_datamodel_set_meta_namespace  (softc_datamodel_t* model, const char *meta_namespace);
const char * softc_datamodel_get_id              (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_name       (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_version    (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_namespace  (const softc_datamodel_t* model);
