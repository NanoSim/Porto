# -*- python -*-

%pythoncode %{

import sys
import json
import ast
import operator

import numpy as np


class SoftError(Exception):
    """Base class for SOFT exceptions."""
    pass

class SoftUninitializedError(SoftError):
    """Raised when working with uninitialized data."""
    pass

class SoftStorageClosedError(SoftError):
    """Raised when working on a closed storage."""
    pass

class SoftMissingDimensionsError(SoftError):
    """Raised if dimensions are missing or cannot be derived."""
    pass

class SoftInvalidDimensionsError(SoftError):
    """Raised if an invalid dimension label is used."""
    pass

class SoftInvalidPropertyError(SoftError):
    """Raised if an invalid property name is used."""
    pass

class SoftMetadataError(SoftError):
    """Raised on malformed metadata description."""
    pass

class SettingDerivedPropertyError(Exception):
    """Users of softpy can raise this exception in setters for derived
    properties.  This exception will signal to the storage loader, of all
    classes inheriting from a class created with the entity factory, that
    the property should not be set.  Example:

        myprop = property(
            lambda self: "hello world",
            lambda self, value: derived_property_exception('myprop'))
    """
    pass

class ArithmeticError(Exception):
    """Raised by arithmetic_eval() on errors in the evaluated expression."""
    pass


class Uninitialized(object):
    """Class representing uninitialized values. Not intended to be
    instanciated..."""


class Storage(object):
    """Class for connecting to a storage.

    Parameters
    ----------
    driver : "json" | "hdf5" | "mongo"...
        The storage driver to use.  Available drivers can be obtained from
        softpy.get_storage_drivers().
    uri : string
        Storage uri.  For json or hdf5 this might be a file name, but might
        be a url for database connections.
    options : None | string
        Additional options passed to the driver.
    """
    def __init__(self, driver, uri, options=None):
        self.driver = driver
        self.uri = uri
        self.options = options
        self.storage = storage_create(driver, uri, options)
        self.strategy = storage_get_storage_strategy(self.storage)
        self._closed = False

    def save(self, entity, uuid=None):
        """Saves entity in this storage.  By default `entity` is stored
        under its own id, but by providing `uuid` you can store it under this
        id instead."""
        if self.closed:
            raise SoftClosedStorageError('Storage %r is closed.' % self.uri)
        e = get_c_entity(entity)
        datamodel = storage_strategy_get_datamodel(self.strategy)
        datamodel_set_id(datamodel, asStr(uuid if uuid else e.id))
        datamodel_set_meta_name(datamodel, asStr(e.name))
        datamodel_set_meta_version(datamodel, asStr(e.version))
        datamodel_set_meta_namespace(datamodel, asStr(e.namespace))
        entity_store(e, datamodel)   # Ask the entity to fill out the datamodel
        storage_strategy_store(self.strategy, datamodel)
        storage_strategy_free_datamodel(datamodel)

    def load(self, entity, uuid=None):
        """Fills `entity` with data loaded from this storage.  By default
        `entity` is loaded from its own id, but by providing `uuid` can load
        it from this id instead."""
        if self.closed:
            raise SoftClosedStorageError('Storage %r is closed.' % self.uri)
        e = get_c_entity(entity)
        datamodel = storage_strategy_get_datamodel(self.strategy)
        datamodel_set_id(datamodel, asStr(uuid if uuid else e.id))
        datamodel_set_meta_name(datamodel, asStr(e.name))
        datamodel_set_meta_version(datamodel, asStr(e.version))
        datamodel_set_meta_namespace(datamodel, asStr(e.namespace))
        storage_strategy_start_retrieve(self.strategy, datamodel)
        entity_load(e, datamodel)  # Ask the entity to fill out itself
        storage_strategy_end_retrieve(self.strategy, datamodel)
        storage_strategy_free_datamodel(datamodel)

    def close(self):
        """Closes current storage."""
        if self.closed:
            raise SoftClosedStorageError('Storage %r is closed.' % self.uri)
        storage_free_storage_strategy(self.strategy)
        storage_free(self.storage)
        self._closed = True

    closed = property(lambda self: self._closed,
                      doc='Whether current storage is closed.')

    def __delete__(self):
        if not self.closed:
            self.close()
        object.__del__(self)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
       self.close()

    def __repr__(self):
        if self.options:
            return "Storage(%r, %r, %r)" % (
                self.driver, self.uri, self.options)
        else:
            return "Storage(%r, %r)" % (self.driver, self.uri)


class Collection(object):
    """A collection of entities."""
    def __init__(self, id=None):
        self.__soft_entity__ = collection_create(id)

    def __delete__(self):
        collection_free(self.__softpy_entity__)
        object.__del__(self)

    def __len__(self):
        return collection_num_entities(self.__soft_entity__)

    def __setitem__(self, label, value):
        if isinstance(value, entity_t):
            e = value
        elif hasattr(value, '__soft_entity__'):
            e = value.__soft_entity__
        else:
            raise TypeError('Value must be an softpy entity')
        collection_register_entity(self.__soft_entity__, label, e)

    def __getitem__(self, label):
        raise NotImplementedError

    #def __setattr__(self, label, value):
    #    self.__setitem__(label, value)

    #def __getattr__(self, label):
    #    return self.__getitem__(label)

    def register_entity(self, label, value):
        self.__setitem__(label, value)

    def add_dim(self, label, description=''):
        collection_add_dim(self.__soft_entity__, label, description)

    def add_connection(self, subject, predicate, object_):
        collection_connection(self.__soft_entity__, subject,
                              predicate, object_)

    def get_num_entities(self):
         return len(self)

    def get_num_dims(self):
         return collection_num_dims(self.__soft_entity__)

    def get_num_relations(self):
         return collection_num_relations(self.__soft_entity__)

    def get_num_dim_maps(self):
         return collection_num_dim_maps(self.__soft_entity__)

    def get_dimensions(self):
         return collection_get_dimensions(self.__soft_entity__)


def get_c_entity(entity):
    """Returns a reference to the underlying C-level entity_t."""
    if hasattr(entity, '__soft_entity__'):
        e = entity.__soft_entity__
    else:
        e = entity
    if not isinstance(e, entity_t):
        raise TypeError('Not a proper entity')
    return e


def arithmetic_eval(expr, constants=None, functions=()):
    """Returns the result of arithmetic evaluation of `expr`.  This function
    is much safer to use than eval() and should be preferred for simple
    arithmetic evaluation.

    If given, `constants` should be a dict mapping constant names to values,
    e.g. ``{'pi': math.pi, 'e': math.e}``.

    The `functions` argument should be a sequence of available functions,
    e.g. ``(abs, min, max, math.sin)``.

    This implementation is inspired by
    http://bingbots.com/questions/148864/valueerror-malformed-string-when-using-ast-literal-eval
    """
    binOps = {
        ast.Add: operator.add,
        ast.Sub: operator.sub,
        ast.Mult: operator.mul,
        ast.Div: operator.truediv,
        ast.Mod: operator.mod,
        ast.Pow: operator.pow,
    }
    consts = constants if constants else {}
    funcs = {f.__name__: f for f in functions}
    node = ast.parse(expr, mode='eval')

    def _eval(node):
        if isinstance(node, ast.Expression):
            return _eval(node.body)
        elif isinstance(node, ast.Str):
            return node.s
        elif isinstance(node, ast.Num):
            return node.n
        elif isinstance(node, ast.BinOp):
            return binOps[type(node.op)](_eval(node.left), _eval(node.right))
        elif isinstance(node, ast.Call):  # function call
            if not node.func.id in funcs:
                raise ArithmeticError(
                    'Invalid function %s() in expr: %r' % (node.func.id, expr))
            func = funcs[node.func.id]
            args = tuple(_eval(arg) for arg in node.args)
            keywords = {kw.arg: _eval(kw.value) for kw in node.keywords}
            return func(*args, **keywords)
        elif isinstance(node, ast.Name):  # constant
            if not node.id in consts:
                raise ArithmeticError(
                    'Invalid constant %r in expr: %r' % (node.id, expr))
            return consts[node.id]
        # -- indexing...
        elif isinstance(node, ast.Subscript):
            return _eval(node.value)[_eval(node.slice)]
        elif isinstance(node, ast.Index):
            return _eval(node.value)
        elif isinstance(node, ast.Slice):
            lower = _eval(node.lower) if node.lower else None
            upper = _eval(node.upper) if node.upper else None
            step = _eval(node.step) if node.step else None
            return slice(lower, upper, step)
        # -- some Python containers, not really arithmetic, but useful...
        elif isinstance(node, ast.List):
            return [_eval(v) for v in node.elts]
        elif isinstance(node, ast.Tuple):
            return tuple(_eval(v) for v in node.elts)
        elif isinstance(node, ast.Set):
            return {_eval(v) for v in node.elts}
        elif isinstance(node, ast.Dict):
            return {_eval(k): _eval(v) for k, v in zip(node.keys, node.values)}
        else:
            raise ArithmeticError(
                'Unsupported type %s in expr: %r' % (node, expr))

    return _eval(node.body)


def asBytes(s):
    """Returns `s` as bytes using the default encoding - works for Python
    2.7 and 3.x."""
    if hasattr(s, 'encode'):
        return s.encode()
    return s

def asStr(s):
    """Returns `s` as string using the default encoding - works for Python
    2.7 and 3.x."""
    if isinstance(s, str):
        return s
    if sys.version_info.major == 2:
        if hasattr(s, 'encode'):
            return s.encode()
    else:
        if hasattr(s, 'decode'):
            return s.decode()
    raise TypeError('Cannot convert %r to string' % (s, ))


def _get_entity_method(entity, name):
    """Returns a reference to method `name` in `entity`.
    """
    if hasattr(entity, name):
        attr = getattr(entity, name)
        if hasattr(attr, '__call__'):
            method = attr
    method = getattr(entity, '_' + name)
    assert hasattr(method, '__call__')
    return method


class MetaEntity(type):
    """Metaclass for BaseEntity providing some functionality to entity
    classes that is not inherited by their instances."""
    def __str__(self):
        return json.dumps(self.soft_metadata, indent=2, sort_keys=True)

    def __repr__(self):
        return '<class %s version=%r, namespace=%r)>' % (
            self.soft_metadata['name'], self.soft_metadata['version'],
            self.soft_metadata['namespace'])

    def __len__(self):
        return len(self.soft_metadata['properties'])

    name = property(lambda self: str(self.soft_metadata['name']),
                    doc='Entity name.')
    version = property(lambda self: str(self.soft_metadata['version']),
                       doc='Entity version string.')
    namespace = property(lambda self: str(self.soft_metadata['namespace']),
                         doc='Entity namespace.')
    description = property(lambda self: str(self.soft_metadata['description']),
                           doc='Entity description.')
    dimensions = property(lambda self: [
        str(d['name']) for d in self.soft_metadata['dimensions']],
                          doc='List of dimension labels.')
    property_names = property(lambda self: [
        str(p['name']) for p in self.soft_metadata['properties']],
                              doc='List of property names.')


class BaseEntity(object):
    """Base class for entities created with the entity() factory function.

    To avoid name conflict with properties and making it easy to subclass
    the entity class created with entity(), all attributes of this class
    are starts with "soft_".  The only exception is the special
    "__soft_entity__" attribute, which provides direct access to the c-level
    object.

    Sometimes the automatic derivation of dimension sizes doesn't work
    (e.g. if the properties has dimension "N+1", softpy is not smart enough
    to determine the size of dimension "N").  In these cases you can either
    provide the argument `dimension_sizes` to the constructor, but it is
    often more convenient to override soft_internal_dimension_size() in
    your subclass.
    """
    __metaclass__ = MetaEntity

    def __init__(self, uuid=None, driver=None, uri=None, options=None,
                 dimension_sizes=None, uninitialize=True, **kwargs):
        """Initialize a new entity.

        If `uuid`, `driver` and `uri` are given, initial property
        values of the new entity will be loaded from dataset `uuid` is
        storage `uri`.  Otherwise a new empty entity will be created.

        Parameters
        ----------
        uuid : None | string
            Id of the instance.  If `driver` is given, it should match the
            id of the dataset to load.  The default is to generate a
            new unique id.
        driver : None | "json" | "hdf5" | "mongo" | ...
            The driver to use for loading initial values.
        uri : None | string
            Where the initial values are stored.  Required if `driver` is
            given.
        options : None | string
            Additional options passed to the driver.
        dimension_sizes : None | sequence | dict | callable

            By default the dimension sizes are derived from the
            property sizes (unless the subclass overrides
            soft_internal_dimension_size()).  This requires that for
            each dimensions at least one property has a dimension of
            exactly this size.  If that is not the case (bad design?),
            you have to provide this argument.

            For entities with static dimensions, it can be given as a
            sequence or dict, like ``[3, 5]`` or ``{'I'=3, 'J'=5}``
            provided that the entity has dimensions I and J.

            For entities with dynamic dimensions, `dimension_sizes` must
            be a callable taking two arguments; a softpy.entity_t and
            a dimension label and should return the size of the dimension
            corresponding to the label.
        uninitialize : bool
            If this is true and `driver` is None, all attributes
            representing SOFT properties not initialized with `kwargs`
            will be set to ``softpy.Uninitialized``.
        kwargs :
            Initial property label-value pairs.  These will overwrite
            initial values read from `uri`.
        """
        meta = self.soft_metadata
        dims = [str(d['name']) for d in meta['dimensions']]
        if isinstance(dimension_sizes, dict):
            dimension_sizes = [dimension_sizes[label] for label in dims]
        self.soft_internal_dimension_info = dimension_sizes

        self.__soft_entity__ = entity_t(
            get_meta_name=meta['name'],
            get_meta_version=meta['version'],
            get_meta_namespace=meta['namespace'],
            get_dimensions=dims,
            get_dimension_size=self.soft_internal_dimension_size,
            load=self.soft_internal_load,
            store=self.soft_internal_store,
            id=uuid,
            user_data=self)

        if driver:
            if uuid is None:
                raise TypeError('`uuid` must be provided when initialising '
                                'an entity from storage')
            if uri is None:
                raise TypeError('`uri` must be provided when initialising '
                                'an entity from storage')
            with Storage(driver, uri, options) as s:
                s.load(self)

        propnames = set(self.soft_get_property_names())
        for name, value in kwargs.items():
            if name not in propnames:
                raise SoftInvalidPropertyError(name)
            self.soft_set_property(name, value)

        if uninitialize and not driver:
            for name in propnames:
                if not hasattr(self, name):
                    try:
                        self.soft_set_property(name, Uninitialized)
                    except SettingDerivedPropertyError:
                        pass

    def soft_internal_dimension_size(self, e, label):
        """Returns the size of dimension `label`.

        Used internally by softpy but may be overridded by subclasses.
        The argument `e` corresponds to __soft_entity__ and should
        normally be used when overriding this method.
        """
        if hasattr(self.soft_internal_dimension_info, '__call__'):
            return self.soft_internal_dimension_info(e, label)
        elif isinstance(self.soft_internal_dimension_info, dict):
            # Dynamic size from property, using cached property name
            # and dimension index
            name, ind = self.soft_internal_dimension_info[label]
            prop = self.soft_get_property(name)
            if prop is Uninitialized:
                raise SoftUninitializedError(
                    'cannot determine dimension size from uninitialized '
                    'property: %r' % name)
            if isinstance(prop, np.ndarray):
                return prop.shape[ind]
            else:
                for i in range(ind):
                    prop = prop[0]
                return len(prop)
        elif hasattr(self.soft_internal_dimension_info, '__getitem__'):
            # Static sizes
            dims = entity_get_dimensions(self.__soft_entity__)
            ind = dims.index(label)
            return self.soft_internal_dimension_info[ind]
        else:
            # Associate property name and dimension index with dimensions,
            # for fast retrival of dynamic sizes
            assert self.soft_internal_dimension_info is None
            dimensions = entity_get_dimensions(self.__soft_entity__)
            d = {}
            for lab in dimensions:
                 for name in self.soft_get_property_names():
                     value = self.soft_get_property(name)
                     for i, dim in enumerate(self.soft_get_property_dims(name)):
                         if dim == lab:
                             d[lab] = (name, i)
                 if not lab in d:
                     raise SoftMissingDimensionsError(
                         'cannot determine size of dimension "%s"' % lab)
            assert set(d) == set(dimensions)
            self.soft_internal_dimension_info = d
            return self.soft_internal_dimension_size(e, label)

    def soft_internal_check_dimension_sizes(self):
        """Checks that the dimension sizes of properties is consistent with
        the metadata definitions.  Raises SoftInvalidDimensionsError for
        inconsistencies."""
        for d in self.soft_metadata['properties']:
             name = asStr(d['name'])
             value = getattr(self, name)
             if value is not Uninitialized and 'dims' in d:
                 for label in d['dims']:
                     dimsize = self.soft_get_dimension_size(asStr(label))
                     if len(value) != dimsize:
                         raise SoftInvalidDimensionsError(
                             'length of dimension %r of property %r is %d, '
                             'expected %d' % (label, name, len(value), dimsize))
                     if len(value) == 0:  # can't continue for zero-sized dims
                         break
                     value = value[0]

    def soft_internal_store(self, e, datamodel):
        """Stores property values to `datamodel`, raising SoftUnitializedError
        if any property is uninitialized.

        Normally you would not call this function directly, but
        instead through Storage.save()."""
        for label in self.soft_get_dimensions():
            value = self.soft_get_dimension_size(label)
            datamodel_append_dimension(datamodel, label, value)

        for name in self.soft_get_property_names():
            value = self.soft_get_property(name)
            if value is Uninitialized:
                raise SoftUninitializedError(
                    'Uninitialized data for "%s.%s"' % (
                        self.__class__.__name__, name))
            vtype = self.soft_get_property_type(name)
            dims = self.soft_get_property_dims(name)
            if not dims:
                setter = getattr(_softpy, 'datamodel_append_' + vtype)
            elif vtype == 'string' or vtype == 'string_list':
                assert len(dims) == 1
                setter = getattr(_softpy, 'datamodel_append_string_list')
            elif len(dims) == 1:
                setter = getattr(_softpy, 'datamodel_append_array_' + vtype)
            else:
                setter = getattr(_softpy, 'datamodel_append_array_%s_%dd' % (
                    vtype, len(dims)))
            #print('*** name=%r -> %r' % (name, setter))
            try:
                setter(datamodel, asStr(name), value)
            except Exception as ex:
                ex.args = ('%s.%s: %s' % (
                    self.__class__.__name__, name, ex), ) + ex.args[1:]
                raise  # reraise exception with property prepended to message

    def soft_internal_load(self, e, datamodel):
        """Loads property values from `datamodel` into self.  Normally you
        would not call this function directly, but instead through
        Storage.load()."""
        for name in self.soft_get_property_names():
            vtype = self.soft_get_property_type(name)
            dims = self.soft_get_property_dims(name)
            if not dims:
                getter = getattr(_softpy, 'datamodel_get_' + vtype)
            elif vtype == 'string_list' or vtype == 'string':
                assert len(dims) == 1
                getter = getattr(_softpy, 'datamodel_get_string_list')
            elif len(dims) == 1:
                getter = getattr(_softpy, 'datamodel_get_array_' + vtype)
            else:
                getter = getattr(_softpy, 'datamodel_get_array_%s_%dd' % (
                    vtype, len(dims)))
            #print('*** name=%r <- %r' % (name, getter))
            value = getter(datamodel, str(name))
            #print('    self=%r, value=%r' % (self, value))
            try:
                self.soft_set_property(asStr(name), value)
            except SettingDerivedPropertyError:
                pass
            except Exception as ex:
                ex.args = ('%s.%s: %s' % (
                    self.__class__.__name__, name, ex), ) + ex.args[1:]
                raise  # reraise exception with property prepended to message

    def soft_initialized(self):
        """Returns true if all properties are initialized. False is returned
        otherwise."""
        return all(self.soft_get_property(name) is not Uninitialized
                   for name in self.soft_get_property_names())

    def soft_get_property(self, name):
         """Returns the value of property `name`.  The default implementation
         checks if there exists a method get_`name`().  If so, the the
         result of calling get_`name`() with no argument is returned.
         Otherwise the value of attribute `name` is returned."""
         getter = 'get_' + name
         if hasattr(self, getter):
             return getattr(self, getter)()
         elif hasattr(self, name):
             return getattr(self, name)
         else:
             raise SoftInvalidPropertyError(name)

    def soft_set_property(self, name, value):
         """Sets property `name` to value.  The default implementation checks
         if there exists a method set_`name`().  If so, set_`name`() is called
         with `value` as argument.  Otherwise the attribute `name` is set to
         `value`."""
         setter = 'set_' + name
         if hasattr(self, setter):
             getattr(self, setter)(value)
         else:
             setattr(self, name, value)

    def soft_get_id(self):
        """Returns entity id."""
        return entity_get_id(self.__soft_entity__)

    def soft_get_meta_name(self):
        """Returns entity name."""
        return entity_get_name(self.__soft_entity__)

    def soft_get_meta_version(self):
        """Returns entity version."""
        return entity_get_name(self.__soft_entity__)

    def soft_get_meta_namespace(self):
        """Returns entity name space."""
        return entity_get_namespace(self.__soft_entity__)

    @classmethod
    def soft_get_meta_description(cls):
        """Returns description of entity."""
        return cls.soft_metadata.get('description', '')

    def soft_get_dimensions(self):
        """Returns a list of dimension labels."""
        return entity_get_dimensions(self.__soft_entity__)

    def soft_get_dimension_size(self, label):
        """Returns size of dimension `label`.  If `label` is not a valid
        dimension label -1 is returned."""
        return entity_get_dimension_size(self.__soft_entity__, label)

    @classmethod
    def soft_get_property_names(cls):
        """Returns a list with the name of all properties."""
        return [p['name'] for p in cls.soft_metadata['properties']]

    @classmethod
    def soft_get_property_unit(cls, name):
        """Returns unit for property `name`, or and empty string if
        `property_name` has no unit."""
        return _get_prop_info(cls, asStr(name), 'unit', '')

    @classmethod
    def soft_get_property_type(cls, name):
        """Returns the type of property `name`."""
        ptype = _get_prop_info(cls, asStr(name), 'type')
        if not ptype:
            raise SoftMetadataError(
                'property "%s" has no type information' % name)
        return ptype

    @classmethod
    def soft_get_property_description(cls, name):
        """Returns description of property `name`."""
        return _get_prop_info(cls, asStr(name), 'description', '')

    @classmethod
    def soft_get_property_dims(cls, name):
        """Returns a list with the dimensions of property `name`."""
        return _get_prop_info(cls, asStr(name), 'dims', [])

    def soft_get_property_dim_sizes(self, name):
        """Returns a list with the dimensions of property `name` evaluated
        to integers."""
        sizes = [(label, get_dimension_size(self.__soft_entity__, label))
                 for label in get_dimensions(self.__soft_entity__)]
        dim_sizes = []
        for sizeexpr in self.soft_get_property_dims(asStr(name)):
             for label, size in sizes:
                 sizeexpr = sizeexpr.replace(label, str(size))
             dim_sizes.append(arithmetic_eval(sizeexpr))
        return dim_sizes


def _get_prop_info(cls, name, field, default=None):
    """Help function used by BaseEntity class methods."""
    for p in cls.soft_metadata['properties']:
        if p['name'] == name:
            return p.get(field, default)
    raise SoftInvalidPropertyError('%s has no property named "%s"' % (
        cls.soft_metadata['name'], field))



def entity(metadata):
    """Factory fuction for creating an Entity class object for `metadata`.

    Here `metadata` is a metadata description (in json format) of the
    entity to create.  It should either be a string or a file-like
    object."""
    if hasattr(metadata, 'read'):
        meta = json.load(metadata)
    else:
        meta = json.loads(metadata)
    attr = dict(soft_metadata=meta)
    return type(str(meta['name']), (BaseEntity,), attr)


# Convinience functions for returning entity info
# -----------------------------------------------
def _get_entity_info(e, field, *args):
    """Help function for returning info about entities."""
    if isinstance(e, entity_t):
        return globals()['entity_get_' + field](e, *args)
    elif hasattr(e, '__soft_entity__'):
        return globals()['entity_get_' + field](e.__soft_entity__, *args)
    else:
        raise TypeError('not an entity: %r' % e)

def get_id(e):
    """Returns the id of entity `e`."""
    return _get_entity_info(e, 'id')

def get_meta_name(e):
    """Returns the name of entity `e`."""
    return _get_entity_info(e, 'meta_name')

def get_meta_version(e):
    """Returns the version of entity `e`."""
    return _get_entity_info(e, 'meta_version')

def get_meta_namespace(e):
    """Returns the namespace of entity `e`."""
    return _get_entity_info(e, 'meta_namespace')

def get_dimensions(e):
    """Returns list of dimension lables for entity `e`."""
    return _get_entity_info(e, 'dimensions')

def get_dimension_size(e, label):
    """Returns size of dimension `label` for entity `e`.  If `label` is
    not a valid dimension label -1 is returned."""
    return _get_entity_info(e, 'dimension_size', label)

def derived_property_exception(msg=''):
    """Convinient function for raising SettingDerivedPropertyError
    within lambdas."""
    raise SettingDerivedPropertyError(msg)

%}
