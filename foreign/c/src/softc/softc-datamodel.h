#ifndef SOFTC_DATAMODEL_H_DEF
#define SOFTC_DATAMODEL_H_DEF

#include <stdint.h>
#include <stdbool.h>
#include "softc-macros.h"
#include "softc-string.h"
#include "softc-string-list.h"
#include "softc-bytearray.h"

SOFTC_BEGIN_DECLS

typedef struct _softc_datamodel_t softc_datamodel_t;

bool softc_datamodel_append_dimension    (softc_datamodel_t *model, const char *key, uint32_t value);
bool softc_datamodel_append_string       (softc_datamodel_t *model, const char *key, const softc_string_s value);
bool softc_datamodel_append_int8         (softc_datamodel_t *model, const char *key, int8_t value);
bool softc_datamodel_append_uint8        (softc_datamodel_t *model, const char *key, uint8_t value);
bool softc_datamodel_append_int16        (softc_datamodel_t *model, const char *key, int16_t value);
bool softc_datamodel_append_uint16       (softc_datamodel_t *model, const char *key, uint16_t value);
bool softc_datamodel_append_int32        (softc_datamodel_t *model, const char *key, int32_t value);
bool softc_datamodel_append_uint32       (softc_datamodel_t *model, const char *key, uint32_t value);
bool softc_datamodel_append_int64        (softc_datamodel_t *model, const char *key, int64_t value);
bool softc_datamodel_append_uint64       (softc_datamodel_t *model, const char *key, uint64_t value);
bool softc_datamodel_append_float        (softc_datamodel_t *model, const char *key, float value);
bool softc_datamodel_append_double       (softc_datamodel_t *model, const char *key, double value);
bool softc_datamodel_append_bool         (softc_datamodel_t *model, const char *key, bool value);
bool softc_datamodel_append_blob         (softc_datamodel_t *model, const char *key, const softc_bytearray_s value);
bool softc_datamodel_append_string_list  (softc_datamodel_t *model, const char *key, const softc_string_list_s *value);
bool softc_datamodel_append_array_int32  (softc_datamodel_t *model, const char *key, const int32_t *value, size_t size);
bool softc_datamodel_append_array_int32_2d  (softc_datamodel_t *model, const char *key, const int32_t* const* value, size_t size_i, size_t size_j);
bool softc_datamodel_append_array_int32_3d  (softc_datamodel_t *model, const char *key, const int32_t* const* const* value, size_t size_i, size_t size_j, size_t size_k);
bool softc_datamodel_append_array_double (softc_datamodel_t *model, const char *key, const double *value, size_t size);
bool softc_datamodel_append_array_double_2d (softc_datamodel_t *model, const char *key, const double* const* value, size_t size_i, size_t size_j);
bool softc_datamodel_append_array_double_3d (softc_datamodel_t *model, const char *key, const double* const* const* value, size_t size_i, size_t size_j, size_t size_k);

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
//bool softc_datamodel_get_string_list     (const softc_datamodel_t *model, const char *key, softc_string_s **value, size_t *n_elements);
bool softc_datamodel_get_string_list     (const softc_datamodel_t *model, const char *key, softc_string_list_s **value);
bool softc_datamodel_get_array_int32     (const softc_datamodel_t *model, const char *key, int32_t **value, size_t *size);
bool softc_datamodel_get_array_int32_2d  (const softc_datamodel_t *model, const char *key, int32_t ***value, size_t *size_i, size_t *size_j);
bool softc_datamodel_get_array_int32_3d  (const softc_datamodel_t *model, const char *key, int32_t ****value, size_t *size_i, size_t *size_j, size_t *size_k);
bool softc_datamodel_get_array_double    (const softc_datamodel_t *model, const char *key, double **value, size_t *size);
bool softc_datamodel_get_array_double_2d (const softc_datamodel_t *model, const char *key, double ***value, size_t *size_i, size_t *size_j);
bool softc_datamodel_get_array_double_3d (const softc_datamodel_t *model, const char *key, double ****value, size_t *size_i, size_t *size_j, size_t *size_k);

bool softc_datamodel_set_id              (softc_datamodel_t* model, const char *id);
bool softc_datamodel_set_meta_name       (softc_datamodel_t* model, const char *meta_name);
bool softc_datamodel_set_meta_version    (softc_datamodel_t* model, const char *meta_version);
bool softc_datamodel_set_meta_namespace  (softc_datamodel_t* model, const char *meta_namespace);
const char * softc_datamodel_get_id              (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_name       (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_version    (const softc_datamodel_t* model);
const char * softc_datamodel_get_meta_namespace  (const softc_datamodel_t* model);

SOFTC_END_DECLS
#endif 
