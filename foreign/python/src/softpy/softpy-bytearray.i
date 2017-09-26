/* -*- C -*-  (not really, but good for syntax highlighting) */

%{
#include "softc-bytearray.h"

/* Wrapper around PyBytes_AsStringAndSize() */
int bytes_AsStringAndSize(PyObject *o, char **s, Py_ssize_t *n)
{
#if PY_MAJOR_VERSION <= 2
  return PyString_AsStringAndSize(o, s, n);
#else
  return PyBytes_AsStringAndSize(o, s, n);
#endif
 }

/* Wrapper around PyBytes_FromStringAndSize() */
PyObject *bytes_FromStringAndSize(const char *s, Py_ssize_t n)
{
#if PY_MAJOR_VERSION <= 2
  return PyString_FromStringAndSize(s, n);
#else
  return PyBytes_FromStringAndSize(s, n);
#endif
}

%}


/* Bytearray input typemaps */
%typemap("doc") softc_bytearray_s IN_SOFTBYTEARRAY "$1_name: bytes"

%typemap(in) softc_bytearray_s IN_SOFTBYTEARRAY {
  char *s=NULL;
  Py_ssize_t n=0;
  if (bytes_AsStringAndSize($input, &s, &n)) return NULL;
  $1 = softc_bytearray_create(s, n);
}

%typemap(freearg) softc_bytearray_s IN_SOFTBYTEARRAY {
  softc_bytearray_destroy($1);
}


/* Bytearray argout typemaps */
%typemap(in,numinputs=0) softc_bytearray_s *ARGOUT_SOFTBYTEARRAY
(softc_bytearray_s v) {
  $1 = &v;
}

%typemap(argout,numinputs=0) softc_bytearray_s *ARGOUT_SOFTBYTEARRAY {
  const unsigned char *s = from_softc_bytearray(*$1);
  size_t n = softc_bytearray_size(*$1);
  $result = bytes_FromStringAndSize(s, n);
}
