/* -*- C -*-  (not really, but good for syntax highlighting) */

/* We only add some typemaps here without exposing the softc_string_list api. */

%{
#include "softc-string-list.h"
%}


/* String_list input typemaps */
%typemap("doc") (softc_string_list_s *IN_SOFTSTRINGLIST) "$1_name: list of strings"

%typemap(in) (softc_string_list_s *IN_SOFTSTRINGLIST) {
  int i;
  PyObject *s;
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a sequence");
    return NULL;
  }
  $1 = softc_string_list_create();
  for (i = 0; i < PySequence_Size($input); i++) {
    s = PySequence_GetItem($input, i);
    if (PyUnicode_Check(s)) {
      softc_string_list_append_cstr($1, PyString_AsString(PyUnicode_AsUTF8String(s)));
    } else if (PyString_Check(s)) {
      softc_string_list_append_cstr($1, PyString_AsString(s));
    } else {
      softc_string_list_free($1);
      Py_DECREF(s);
      PyErr_SetString(PyExc_ValueError, "Sequence items must be strings");
      return NULL;
    }
    Py_DECREF(s);
  }
}

%typemap(freearg) softc_string_list_s *IN_SOFTSTRINGLIST {
  softc_string_list_free($1);
}


/* String_list output typemaps */

/* Set output argument to point to temporary variable */
%typemap(in,numinputs=0) (softc_string_list_s **ARGOUT_SOFTSTRINGLIST)
                         (softc_string_list_s* tmp) {
  $1 = &tmp;
}

%typemap(argout,numinputs=0) (softc_string_list_s **ARGOUT_SOFTSTRINGLIST) {
  int i;
  size_t n = softc_string_list_count(*$1);
  PyObject *lst = PyList_New(n);
  for (i = 0; i < n; ++i) {
    softc_string_s s = softc_string_at(*$1, i);
    const char *p = from_softc_string(s);
    PyObject *str = string_FromString(p);
    PyList_SetItem(lst, i, str);
  }
  $result = lst;
}


/* Tell swig that softc_string_list_create() returns a new object and
 * that it is cleaned up by softc_string_list_free() */
%newobject softc_string_list_create;
%delobject softc_string_list_free;


/* Declarations */
softc_string_list_s * softc_string_list_create();
void softc_string_list_free(softc_string_list_s *self);
void softc_string_list_append(softc_string_list_s *self, softc_string_s str);
void softc_string_list_append_cstr(softc_string_list_s *self, const char *str);
void softc_string_list_clear(softc_string_list_s *self);
size_t softc_string_list_count(const softc_string_list_s *self);
softc_string_s softc_string_list_first(softc_string_list_s *self);
softc_string_s softc_string_list_last(softc_string_list_s *self);
softc_string_s softc_string_at(const softc_string_list_s *self, int);
