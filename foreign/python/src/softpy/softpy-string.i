/* -*- C -*-  (not really, but good for syntax highlighting) */

%{
#include "softc-string.h"

PyObject *string_FromString(const char *s)
{
#if PY_MAJOR_VERSION <= 2
  return PyString_FromString(s);
#else
  return  PyUnicode_FromString(s);
#endif
}

%}


/* String input typemaps */
%typemap("doc") softc_string_s IN_SOFTSTRING "$1_name: string"

%typemap(in) softc_string_s IN_SOFTSTRING {
  char *s;
  if (PyUnicode_Check( $input )) {
    s = PyString_AsString(PyUnicode_AsUTF8String($input));
  } else if (PyString_Check( $input )) {
    s = PyString_AsString($input);
  } else {
    PyErr_SetString(PyExc_ValueError, "Expecting a string");
    return NULL;
  }
  $1 = softc_string_create(s);
}

%typemap(freearg) softc_string_s IN_SOFTSTRING {
  softc_string_destroy($1);
}


/* String argout typemaps */
%typemap(in,numinputs=0) softc_string_s *ARGOUT_SOFTSTRING (softc_string_s v) {
  $1 = &v;
}

%typemap(argout,numinputs=0) (softc_string_s *ARGOUT_SOFTSTRING) {
  const char *s = from_softc_string(*$1);
  $result = string_FromString(s);
}


/* String out typemaps
 * This exposes softc_string_s as ordinary immutable Python strings.
 *
 * Pros: simple and natural interface + we get rid of an annoying
 * memory leak due to failing to freeing the underlying softc_string_s *
 * pointer for objects returned by string_create().
 *
 * Cons: softc_string_append() cannot be exposed in Python, logically
 * because Python strings are immutable, but also because that the
 * underlying softc_string_s object is destroyed when softc_string_create()
 * returns.
 */
%typemap(out) softc_string_s {
  $result = string_FromString(from_softc_string($1));
}

%typemap(newfree) softc_string_s {
  softc_string_destroy($1);
}


/* Tell swig that softc_string_create() returns a new object */
%newobject softc_string_create;

/* Declarations */
softc_string_s softc_string_create(const char *str);
