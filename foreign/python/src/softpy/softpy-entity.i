/* -*- C -*-  (not really, but good for syntax highlighting) */

/*
 * C code
 */    
%{
#define CK_ALLOC(alloc) if (!(alloc)) do {	 \
    PyErr_SetString(PyExc_MemoryError, # alloc); \
    softpy_entity_error = 1;			 \
    goto fail;					 \
  } while (0)

#define ENTITY_ERROR(PyType, msg) do {		\
    softpy_entity_error = 1;			\
    fprintf(stderr, "ERROR: %s\n", msg);	\
    PyErr_SetString(PyType, msg);		\
    goto fail;					\
  } while (0)

  
  /* Mirror the opaque definition in softc-entity.c */
  typedef struct _softpy_entity_t {

    /* Header common to all softc-entity_t, see softc-entity.c */
    struct softc_entity_vtable_ *vtable_;
    const char *id;

    PyObject *this;       // Reference to Python pointer object to this entity
    PyObject *user_data;  // Reference to user-provided Python object

    /* References to Python callables */
    PyObject *get_meta_name;
    PyObject *get_meta_version;
    PyObject *get_meta_namespace;
    PyObject *get_dimensions;
    PyObject *get_dimension_size;
    PyObject *store;
    PyObject *load;

    /* Entity data */
    const char *name;
    const char *version;
    const char *namespace;
    size_t ndims;
    const char **dims;
    size_t *dim_sizes;
  } softpy_entity_t;


  /* Flag indicating that an error has occured */
  static int softpy_entity_error = 0;
  

  /* Help function setting self->ndims and self->dims.  Returns
   * non-zero on error. */
  static int softpy_set_dimensions(softpy_entity_t *self, PyObject *seq)
  {
    int i, n;
    PyObject *label=NULL;
    assert(PySequence_Check(seq));

    for (i=0; i<self->ndims; i++) free((char *)self->dims[i]);
    self->ndims = 0;

    n = PySequence_Length(seq);
    CK_ALLOC(self->dims = realloc(self->dims, n * sizeof(char *)));

    for (self->ndims=0; self->ndims < n; self->ndims++) {
      if (!(label = PySequence_GetItem(seq, self->ndims))) goto fail;
      if (!(self->dims[self->ndims] = pystring(label))) goto fail;
      Py_DECREF(label);
      label = NULL;
    }
    assert(self->ndims == n);
    return 0;
  fail:
    softpy_entity_error = 1;
    if (label) Py_DECREF(label);
    return 1;
  }

  /* Help function setting self->ndims and self->dim_sizes.  Returns
   * non-zero on error. */
  static int softpy_set_dimension_size(softpy_entity_t *self, PyObject *seq)
  {
    int i, n;
    PyObject *item=NULL;

    assert(PySequence_Check(seq));
    n = PySequence_Length(seq);
    if ((!self->get_dimensions) && self->ndims != n)
      ENTITY_ERROR(PyExc_TypeError, 
	    "length of sequence must match number of dimensions");
    CK_ALLOC(self->dim_sizes = realloc(self->dim_sizes, n * sizeof(char *)));
    for (i=0; i<n; i++) {
      if (!(item = PySequence_GetItem(seq, i))) goto fail;
      if (!PyInt_Check(item))
	ENTITY_ERROR(PyExc_TypeError,"dimension sizes must be ints");
      self->dim_sizes[i] = PyInt_AsLong(item);
    }
    return 0;
  fail:
    softpy_entity_error = 1;
    if (item) Py_DECREF(item);
    return 1;
  }

  /* Help function. Calls `callable` and return the string returned by it */
  const char *get_string(const softpy_entity_t *self, PyObject *callable, char *s)
  {
    int n;
    const char *string;
    PyObject *args=NULL, *o=NULL;
    if (!(args = Py_BuildValue("(O)", self->this))) goto fail;
    assert(PyTuple_Check(args));
    if (!(o = PyObject_CallObject(callable, args))) goto fail;
    if (!(string = pystring(o))) goto fail;
    n = strlen(string);
    CK_ALLOC(s = realloc(s, n + 1));
    memcpy(s, string, n);
    s[n] = '\0';
    Py_DECREF(o);
    Py_DECREF(args);
    return s;
 fail:
    softpy_entity_error = 1;
    if (o) Py_DECREF(o);
    if (args) Py_DECREF(args);
    return NULL;
  }


  static const char *softpy_get_meta_name(const softc_entity_t *ptr) {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    if (self->get_meta_name)
      return get_string(self, self->get_meta_name, (char *)self->name);
    else
      return self->name;
  }

  static const char *softpy_get_meta_version(const softc_entity_t *ptr) {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    if (self->get_meta_version)
      return get_string(self, self->get_meta_version, (char *)self->version);
    else
      return self->version;
  }

  static const char *softpy_get_meta_namespace(const softc_entity_t *ptr) {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    if (self->get_meta_namespace)
      return get_string(self, self->get_meta_namespace, (char *)self->namespace);
    else
      return self->namespace;
  }


  /* Returns a pointer to a array of pointers to dimension labels.  On
   * return `size` is set to the length of the array. The returned
   * array should not be free'ed. 
   * On error NULL is returned. */
  static const char **softpy_get_dimensions(const softc_entity_t *ptr, 
					    size_t *size) 
  {
    softpy_entity_t *self = (softpy_entity_t *)ptr;
    PyObject *args=NULL, *seq=NULL;

    if (self->get_dimensions) {
      assert(PyCallable_Check(self->get_dimensions));
      
      if (!(args = Py_BuildValue("(O)", self->this))) goto fail;
      assert(PyTuple_Check(args));
      
      if (!(seq = PyObject_CallObject(self->get_dimensions, args))) goto fail;
      if (!PySequence_Check(seq))
	ENTITY_ERROR(PyExc_TypeError, "callable must return a sequence "
		     "of dimension labels");
      softpy_set_dimensions(self, seq);
      Py_DECREF(seq);
      Py_DECREF(args);
    }

    *size = self->ndims;
    return self->dims;

 fail:
    softpy_entity_error = 1;
    if (seq) Py_DECREF(seq);
    if (args) Py_DECREF(args);
    return NULL;
  }


  /* Returns the size of dimension `label` or -1 on error. */
  static int softpy_get_dimension_size(const softc_entity_t *ptr, 
				       const char *label) 
  {
    int i, size;
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    PyObject *args=NULL, *pysize=NULL;
    
    if (self->get_dimension_size) {
      assert(PyCallable_Check(self->get_dimension_size));

      if (!(args = Py_BuildValue("Os", self->this, label))) goto fail;
      assert(PyTuple_Check(args));

      if (!(pysize = PyObject_CallObject(self->get_dimension_size, args))) 
	goto fail;
      if (!PyInt_Check(pysize))
	ENTITY_ERROR(PyExc_TypeError, "callable must return an int");
      size = PyInt_AsLong(pysize);
      Py_DECREF(pysize);
      Py_DECREF(args);
      return size;
    } else {
      size_t ndims;
      const char **dims = softpy_get_dimensions(ptr, &ndims);

      assert(self->dim_sizes);
      for (i=0; i<ndims; i++)
	if (strcmp(dims[i], label) == 0) return self->dim_sizes[i];
      return -1;
    }

  fail:
    softpy_entity_error = 1;
    if (pysize) Py_DECREF(pysize);
    if (args) Py_DECREF(args);
    return -1;
  }
  

  static void softpy_store_or_load(const softc_entity_t *ptr,
				   softc_datamodel_t *datamodel, int storing) 
  {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    PyObject *args=NULL, *pymodel=NULL, *retval=NULL;

    if (!(pymodel = SWIG_NewPointerObj(SWIG_as_voidptr(datamodel),
				       SWIGTYPE_p_softc_datamodel_t, 0))) 
      ENTITY_ERROR(PyExc_TypeError, 
		   "Cannot create new reference to `datamodel`");
    if (!(args = Py_BuildValue("OO", self->this, pymodel))) goto fail;
    assert(PyTuple_Check(args));

    if (storing) {
      if (!(retval = (PyObject_CallObject(self->store, args)))) goto fail;
    } else {
      if (!(retval = (PyObject_CallObject(self->load, args)))) goto fail;
    }
    Py_DECREF(retval);
    Py_DECREF(args);
    Py_DECREF(pymodel);
    return;

  fail:
    softpy_entity_error = 1;
    if (pymodel) Py_DECREF(pymodel);
    if (args) Py_DECREF(args);
    if (retval) Py_DECREF(pymodel);
  }


  static void softpy_store(const softc_entity_t *ptr, 
			   softc_datamodel_t *datamodel)
  {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    assert(self->store);
    if (!PyCallable_Check(self->store))
      ENTITY_ERROR(PyExc_TypeError,"`store` must be callable");
    softpy_store_or_load(ptr, datamodel, 1);
    return;
  fail:
    softpy_entity_error = 1;
  }

  static void softpy_load(softc_entity_t *ptr, 
			  const softc_datamodel_t *datamodel)
  {
    const softpy_entity_t *self = (const softpy_entity_t *)ptr;
    assert(self->load);
    if (!PyCallable_Check(self->load))
      ENTITY_ERROR(PyExc_TypeError,"`load` must be callable");
    softpy_store_or_load(ptr, (softc_datamodel_t *)datamodel, 0);
    return;
  fail:
    softpy_entity_error = 1;
  }


  /*
   * Functions for the extended API
   */
  static void clear_softpy_entity_t(softpy_entity_t *self)
  {
    if (self->get_meta_name) Py_DECREF(self->get_meta_name);
    if (self->get_meta_version) Py_DECREF(self->get_meta_version);
    if (self->get_meta_namespace) Py_DECREF(self->get_meta_namespace);
    if (self->get_dimensions) Py_DECREF(self->get_dimensions);
    if (self->get_dimension_size) Py_DECREF(self->get_dimension_size);
    if (self->store) Py_DECREF(self->store);
    if (self->load) Py_DECREF(self->load);

    if (self->vtable_) free(self->vtable_);
    if (self->id) free((char *)self->id);
    if (self->this) Py_DECREF(self->this);
    if (self->user_data) Py_DECREF(self->user_data);

    if (self->name) free((char *)self->name);
    if (self->version) free((char *)self->version);
    if (self->namespace) free((char *)self->namespace);
    if (self->dims) {
      int i;
      for (i=0; i<self->ndims; i++) free((char *)self->dims[i]);
      free(self->dims);
    }
    if (self->dim_sizes) free(self->dim_sizes);
  }

  static void delete_softc_entity_t(softc_entity_t *ptr) {
    softpy_entity_t *self = (softpy_entity_t *)ptr;
    clear_softpy_entity_t(self);
    free(self);
  }

  static void init_softpy_entity_t(softpy_entity_t *self,
			    PyObject *meta_name,
			    PyObject *meta_version,
			    PyObject *meta_namespace,
			    PyObject *dimensions, 
			    PyObject *dimension_size, 
			    PyObject *store, 
			    PyObject *load,
			    const char *id,
			    PyObject *user_data)
  {
    CK_ALLOC(self->vtable_ = calloc(1, sizeof(softc_entity_vtable)));
    self->vtable_->get_meta_name = softpy_get_meta_name;
    self->vtable_->get_meta_version = softpy_get_meta_version;
    self->vtable_->get_meta_namespace = softpy_get_meta_namespace;
    self->vtable_->get_dimensions = softpy_get_dimensions;
    self->vtable_->get_dimension_size = softpy_get_dimension_size;
    self->vtable_->store = softpy_store;
    self->vtable_->load = softpy_load;

    self->id = (id) ? strdup(id) : softc_uuidgen();

    self->this = SWIG_NewPointerObj(SWIG_as_voidptr(self), 
    				    SWIGTYPE_p_softc_entity_t, 0);
    if (user_data) {
      Py_INCREF(user_data);
      self->user_data = user_data;
    }

    if (PyCallable_Check(meta_name)) {
      self->get_meta_name = meta_name;
      Py_INCREF(meta_name);
    } else {
      if (!(self->name = pystring(meta_name))) goto fail;
    }

    if (PyCallable_Check(meta_version)) {
      self->get_meta_version = meta_version;
      Py_INCREF(meta_version);
    } else {
      if (!(self->version = pystring(meta_version))) goto fail;
    }

    if (PyCallable_Check(meta_namespace)) {
      self->get_meta_namespace = meta_namespace;
      Py_INCREF(meta_namespace);
    } else {
      if (!(self->namespace = pystring(meta_namespace))) goto fail;
    }

    if (PyCallable_Check(dimensions)) {
      self->get_dimensions = dimensions;
      Py_INCREF(dimensions);
    } else if (PySequence_Check(dimensions)) {
      if (softpy_set_dimensions(self, dimensions)) goto fail;
    } else {
      ENTITY_ERROR(PyExc_TypeError, 
		   "`dimensions` must be a callable or sequence");
    }

    if (PyCallable_Check(dimension_size)) {
      self->get_dimension_size = dimension_size;
      Py_INCREF(dimension_size);
    } else if (PySequence_Check(dimension_size)) {
      if (softpy_set_dimension_size(self, dimension_size)) goto fail;
    } else {
      ENTITY_ERROR(PyExc_TypeError, 
		      "`dimension_size` must be a callable or sequence");
    }

    if (PyCallable_Check(store)) {
      self->store = store;
      Py_INCREF(store);
    } else {
      ENTITY_ERROR(PyExc_TypeError, "`store` must be callable");
    }

    if (PyCallable_Check(load)) {
      self->load = load;
      Py_INCREF(load);
    } else {
      ENTITY_ERROR(PyExc_TypeError, "`load` must be callable");
    }

    return;

  fail:
    softpy_entity_error = 1;
    if (self) delete_softc_entity_t((softc_entity_t *)self);
  }

 
  softc_entity_t *new_softc_entity_t(PyObject *meta_name,
				     PyObject *meta_version,
				     PyObject *meta_namespace,
				     PyObject *dimensions, 
				     PyObject *dimension_size, 
				     PyObject *store, 
				     PyObject *load,
				     const char *id,
				     PyObject *user_data)
  {
    softpy_entity_t *self=NULL;
    CK_ALLOC(self = calloc(1, sizeof(softpy_entity_t)));
    init_softpy_entity_t(self, meta_name, meta_version, meta_namespace,
			 dimensions, dimension_size, store, load, id,
			 user_data);
    if (softpy_entity_error) goto fail;
    return (softc_entity_t *)self;
  fail:
    if (self) delete_softc_entity_t((softc_entity_t *)self);
    return NULL;
  }

 
  static const char *softc_entity_t_id_get(softc_entity_t *self) {
    return softc_entity_get_id(self);
  }

  static const char *softc_entity_t_name_get(softc_entity_t *self) {
    return softc_entity_get_meta_name(self);
  }

  static const char *softc_entity_t_version_get(softc_entity_t *self) {
    return softc_entity_get_meta_version(self);
  }

  static const char *softc_entity_t_namespace_get(softc_entity_t *self) {
    return softc_entity_get_meta_namespace(self);
  }

  PyObject *softc_entity_t_user_data_get(softc_entity_t *self) {
    PyObject *user_data = ((softpy_entity_t *)self)->user_data;
    Py_INCREF(user_data);
    return user_data;
  }

  /* Wrapper for softc_entity_get_dimensions() that returns a NULL-terminates
   * string list. */
  const char **entity_get_dimensions(const softc_entity_t *ptr)
  {
    int i;
    size_t size;
    const char **new=NULL, **labels=softc_entity_get_dimensions(ptr, &size);
    if (!labels) goto fail;
    CK_ALLOC(new = calloc(size + 1, sizeof(char *)));
    for (i=0; i<size; i++) CK_ALLOC(new[i] = strdup(labels[i]));
    //const char **new = malloc((size + 1) * sizeof(char *));
    //for (i=0; i<size; i++) new[i] = strdup(s[i]);
    new[size] = NULL;
    return new;
  fail:
    softpy_entity_error = 1;
    for (i=0; i<size; i++) if (new[i]) free((char *)new[i]);
    if (new) free((char **)new);
    return NULL;
  }

  static const char **softc_entity_t_dimensions_get(softc_entity_t *self) {
    return entity_get_dimensions(self);
  }

  /* A convinience function returning the dimension sizes as a newly
   * allocated array (INT_LIST) of int's with size as the first element. */
  static int *softc_entity_t_dimension_sizes_get(softc_entity_t *self) {
    int i, *list=NULL;
    size_t size;
    const char **labels = softc_entity_get_dimensions(self, &size);
    if (!labels) goto fail;
    CK_ALLOC(list = calloc(size + 1, sizeof(int)));
    list[0] = size;
    for (i=0; i<size; i++) 
      list[i+1] = softc_entity_get_dimension_size(self, labels[i]);
    return list;
  fail:
    softpy_entity_error = 1;
    if (list) free(list);
    return NULL;
  }

  /* For supporting pickle */
  static PyObject *softpy_getstate(softpy_entity_t *self)
  {
    PyObject *state=NULL;
    int i;
    
    if (!(state = PyDict_New())) goto fail;

    if (self->get_meta_name) {
      PyDict_SetItemString(state, "meta_name", self->get_meta_name);
    } else {
      PyObject *meta_name = PyString_FromString(self->name);
      PyDict_SetItemString(state, "meta_name", meta_name);
      Py_DECREF(meta_name);
    }

    if (self->get_meta_version) {
      PyDict_SetItemString(state, "meta_version", self->get_meta_version);
    } else {
      PyObject *meta_version = PyString_FromString(self->version);
      PyDict_SetItemString(state, "meta_version", meta_version);
      Py_DECREF(meta_version);
    }

    if (self->get_meta_namespace) {
      PyDict_SetItemString(state, "meta_namespace", self->get_meta_namespace);
    } else {
      PyObject *meta_namespace = PyString_FromString(self->namespace);
      PyDict_SetItemString(state, "meta_namespace", meta_namespace);
      Py_DECREF(meta_namespace);
    }

    if (self->get_dimensions) {
      PyDict_SetItemString(state, "dimensions", self->get_dimensions);
    } else {
      PyObject *dims = PyTuple_New(self->ndims);
      for (i=0; i<self->ndims; i++)
	PyTuple_SetItem(dims, i, PyString_FromString(self->dims[i]));
      PyDict_SetItemString(state, "dimensions", dims);
      Py_DECREF(dims);
    }

    if (self->get_dimension_size) {
      PyDict_SetItemString(state, "dimension_size", self->get_dimension_size);
    } else {
      PyObject *dim_sizes = PyTuple_New(self->ndims);
      for (i=0; i<self->ndims; i++)
	PyTuple_SetItem(dim_sizes, i, PyInt_FromSize_t(self->dim_sizes[i]));
      PyDict_SetItemString(state, "dimension_size", dim_sizes);
      Py_DECREF(dim_sizes);
    }

    PyDict_SetItemString(state, "store", self->store);
    PyDict_SetItemString(state, "load", self->load);
    PyDict_SetItemString(state, "id", PyString_FromString(self->id));
    PyDict_SetItemString(state, "user_data", self->user_data);
    return state;
  fail:
    if (state) Py_DECREF(state);
    return NULL;
  }

  /* 
  static void softpy_setstate(softpy_entity_t *self, PyObject *state)
  {
    PyObject *meta_name, *meta_version, *meta_namespace,
      *dimensions, *dimension_size, *store, *load, *pyid, *user_data;
    char *id=NULL;
    if (!PyDict_Check(state)) goto fail;
    if (!(meta_name =
	  PyDict_GetItemString(state, "meta_name"))) goto fail;
    if (!(meta_version =
	  PyDict_GetItemString(state, "meta_version"))) goto fail;
    if (!(meta_namespace =
	  PyDict_GetItemString(state, "meta_namespace"))) goto fail;
    if (!(dimensions =
	  PyDict_GetItemString(state, "dimensions"))) goto fail;
    if (!(dimension_size =
	  PyDict_GetItemString(state, "dimension_size"))) goto fail;
    if (!(store = PyDict_GetItemString(state, "store"))) goto fail;
    if (!(load = PyDict_GetItemString(state, "load"))) goto fail;
    if (!(pyid = PyDict_GetItemString(state, "id"))) goto fail;
    if (!(user_data = PyDict_GetItemString(state, "user_data"))) goto fail;
    if (!(id = pystring(pyid))) goto fail;
    init_softpy_entity_t(self, meta_name, meta_version, meta_namespace,
			 dimensions, dimension_size, store, load, id,
			 user_data);
    return;
  fail:
    softpy_entity_error = 1;
    if (id) free(id);
  }
  */
  
%}


/*
 * SWIG declarations
 */    

%exception {
  softpy_entity_error = 0;
  $action
  if (softpy_entity_error) SWIG_fail;
}
%exception softc_entity_t {
  $action
  if (!result) SWIG_fail;
}

%feature("docstring", "\
Creates a SOFT entity.") softc_entity_t;
%typemap("doc") (PyObject *) \
  "$1_name : string | callable(self) -> string" 
%typemap("doc") (PyObject *get_dimensions) \
  "$1_name : sequence | callable(self) -> sequence\n" \
  "        Dimension labels."
%typemap("doc") (PyObject *get_dimension_size) \
  "$1_name : sequence | callable(self, label) -> int\n" \
  "        Dimension size(s)."
%typemap("doc") (PyObject *store) \
  "$1_name : callable(self, datamodel)\n" \
  "        Stores entity data to `datamodel`."
%typemap("doc") (PyObject *load) \
  "$1_name : callable(self, datamodel)\n" \
  "        Loads entity data from `datamodel`."
%typemap("doc") (const char *id) \
  "$1_name : None | string\n" \
  "        Entity id.  Generated if None."
%typemap("doc") (PyObject *user_data) \
  "$1_name : any object\n" \
  "        Optional user object typically containing the entity data."

/* Keep softc_entity_t opaque, but add a set of specialised attributes. */
typedef struct {
  %extend {
    softc_entity_t(PyObject *get_meta_name,
		   PyObject *get_meta_version,
		   PyObject *get_meta_namespace,
		   PyObject *get_dimensions, 
		   PyObject *get_dimension_size, 
		   PyObject *store, 
		   PyObject *load,
		   const char *id=NULL,
		   PyObject *user_data=NULL);
    ~softc_entity_t();

    %immutable;
    const char *id;
    const char *name;
    const char *version;
    const char *namespace;
    const char **dimensions;
    INT_LIST dimension_sizes;
    PyObject *user_data;
    %mutable;

    PyObject *__getstate__() {
      return softpy_getstate((softpy_entity_t *)$self);
    }
    /*
    void __setstate__(PyObject *state) {
      softpy_setstate((softpy_entity_t *)$self, state);
    }
    */

    %pythoncode {
        def __reduce__(self):
            state = self.__getstate__()
            args = tuple(state[k] for k in (
                'meta_name', 'meta_version', 'meta_namespace',
                'dimensions', 'dimension_size', 'store', 'load',
                'id', 'user_data'))
            return self.__class__, args

        def __eq__(self, other):
            state1 = self.__getstate__()
            state2 = other.__getstate__()
            for k, v1 in state1.items():
                v2 = state2[k]
                if hasattr(v1, 'im_func') and hasattr(v2, 'im_func'):
                    if v1.im_func.func_code != v2.im_func.func_code:
                       return False
                elif hasattr(v1, '__func__') and hasattr(v2, '__func__'):
                    if v1.__func__ != v2.__func__:
                       return False
                elif v1 != v2:
                    return False
            return True
    }
  }
} softc_entity_t;


%typemap("doc") (PyObject *) ""


//softc_entity_t *softc_entity_new(const char *uri);
const char  *softc_entity_get_id(const softc_entity_t *self);
const char  *softc_entity_get_meta_type(const softc_entity_t *self);
const char  *softc_entity_get_meta_name(const softc_entity_t *self);
const char  *softc_entity_get_meta_namespace(const softc_entity_t *self);
const char  *softc_entity_get_meta_version(const softc_entity_t *self);  
int          softc_entity_get_dimension_size(const softc_entity_t *self, 
					     const char *label);
void         softc_entity_store(const softc_entity_t *self, 
				softc_datamodel_t *datamodel);
void         softc_entity_load(softc_entity_t *self, 
			       const softc_datamodel_t *datamodel);

/* Special handeling of softc_entity_get_dimensions() */
const char **entity_get_dimensions(const softc_entity_t *self);
