/* -*- C -*-  (not really, but good for syntax highlighting) */

/*
%typemap(in,numinputs=0) char **value (const char *str)   { $1 = &temp; };
%typemap(argout)         char **value                     { $result = PyString_FromString(*$1); };
*/

softc_string_s softc_string_create(const char *str);
const char *from_softc_string(const softc_string_s s);
void softc_string_destroy(softc_string_s str);

softc_string_s* softc_string_createlist(const char *str[], size_t n);
void softc_string_destroylist(softc_string_s* strlist, size_t n);

/*
%typemap(in,numinputs=1) softc_string_s {
   $1 = softc_string_create( PyString_AsString( $1 ) );
}
*/

%typemap(in,numinputs=1) softc_string_s const {
   if (!PyString_Check( $input )) {
     PyErr_SetString(PyExc_ValueError, "Expecting a string");
     return NULL;
   }
   $1 = softc_string_create( PyString_AsString( $input ) );
}

/* Set the input argument to point to a temporary variable */
/*
%typemap(argout,numinputs=0) softc_string_s {
  $result = PyString_FromString( from_softc_string( $1 ));
}
*/

/*
bool softc_datamodel_get_string(const softc_datamodel_t *model, const char *key, softc_string_s *value);
%typemap(argout,numinputs=1) (softc_string_s *value)
*/
/*
%typemap(in,numinputs=1) (const char *key, softc_string_s *value)
                         (const char *key, softc_string_s value) {
  $1 = PyString_AsString( $input );
  $2 = &value;
}
*/
%typemap(in,numinputs=0) softc_string_s *value (softc_string_s value) {
  $1 = &value;
}

%typemap(argout,numinputs=2) (const softc_datamodel_t *model, const char *key, softc_string_s *value) {
  softc_string_s* v = $3;
  $result = PyString_FromString( from_softc_string( *v ));
  softc_string_destroy(*v);
}

/*
%typemap(argout,numinputs=0) softc_string_s *value {
  softc_string_s s = *$1;
  $result = PyString_FromString( from_softc_string( s ));
}

%typemap(argout) softc_string_s const {
  $result = PyString_FromString( from_softc_string( $1 ));
}
*/

%typemap(freearg) softc_string_s {
   softc_string_destroy( $1 );
}

/*
 * The next two typemaps should specifically match this prototype, including the
 * names of the arguments:
 * bool softc_datamodel_append_string_list(softc_datamodel_t *model, const char *key,
 *                        const softc_string_s *strlist, size_t n_elements);
 */
/* Converts a single Python string list argument into 2 arguments for the C routine */
%typemap(in,numinputs=1) (const softc_string_s* strlist, size_t n_elements)
                         (const softc_string_s* strlist, size_t n) {
  int i;
  char** lst;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  $2 = n = PyList_Size($input);
  lst = (char **) malloc(n * sizeof(char *));
  for (i = 0; i < n; ++i) {
    PyObject *s = PyList_GetItem($input, i);
    if (!PyString_Check(s)) {
        free(lst);
        PyErr_SetString(PyExc_ValueError, "List items must be strings");
        return NULL;
    }
    lst[i] = PyString_AsString(s);
  }
  $1 = softc_string_createlist( (const char**)lst, n );
  free(lst);
}

/* After having called softc_datamodel_append_string_list, this one cleans up temporary
 * resources; It complements the 'softc_string_createlist' above.
 */
%typemap(freearg) (const softc_string_s* strlist, size_t n_elements)
                  (softc_string_s* strlist, size_t n) {
  softc_string_destroylist( $1, $2 );
}

/*
 * The next two typemaps should specifically match this prototype:
 * bool softc_datamodel_get_string_list(const softc_datamodel_t *model, const char *key,
 *                        softc_string_s **strlist, size_t *n_elements);
 */
/* Typemap to strip away the 2 return values from the Python argument list */
%typemap(in,numinputs=0) (softc_string_s **strlist, size_t *n_elements)
                         (softc_string_s* strlist, size_t n) {
  $1 = &strlist;
  $2 = &n;
}

/* Typemap to converted the 2 return values from the argument list to the Python result & type */
%typemap(argout,numinputs=0) (softc_string_s **strlist, size_t *n_elements)
                             (softc_string_s* strlist, size_t n) {
  int i;
  size_t len = 0;
  PyObject *lst = 0;
  softc_string_s* a = 0;
  PyObject *s = 0;
  const char* p = 0;
  len = *$2;
  a = *$1;
  lst = PyList_New( len );
  for (i = 0; i < len; ++i) {
    p = from_softc_string( a[i] );
    s = PyString_FromString( p );
    PyList_SetItem( lst, i, s );
  }
  softc_string_destroylist( a, len );
  $result = lst;
}

