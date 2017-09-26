# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import print_function

import sys

import numpy as np

from . import softpy
from .softpy import asStr, asBytes, _softpy
from .arithmetic_eval import arithmetic_eval
from .errors import SoftError, SoftMetadataError
from .storage import Storage
from .metadata import Metadata
from .utils import json_loads, json_dumps, json_load, json_dump


__all__ = ['entity', 'load_entity', 'derived_property_exception']


class SoftMissingDimensionsError(SoftError):
    """Raised if dimensions are missing or cannot be derived."""
    pass

class SoftInvalidDimensionsError(SoftError):
    """Raised if an invalid dimension label is used."""
    pass

class SoftInvalidPropertyError(SoftError):
    """Raised if an invalid property name is used."""
    pass

class SoftUninitializedError(SoftError):
    """Raised when working with uninitialized data."""
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


class Uninitialized(object):
    """Class representing uninitialized values. Not intended to be
    instanciated..."""
    pass



class MetaEntity(type):
    """Metaclass for BaseEntity providing some functionality to entity
    classes that is not inherited by their instances."""
    def __str__(self):
        return json_dumps(self.soft_metadata, indent=2, sort_keys=True)

    def __repr__(self):
        return '<class %s version=%r, namespace=%r)>' % (
            self.soft_metadata['name'], self.soft_metadata['version'],
            self.soft_metadata['namespace'])

    def __len__(self):
        return len(self.soft_metadata['properties'])

    def __eq__(self, other):
        return self.soft_metadata == other.soft_metadata

    def __hash__(self):
        # The IPython pretty-printer calls this hash method (for
        # caching) from a object with no soft_metadata attribute while
        # it looks for formatters. This annoying exceptions in
        # interactive sessions.
        # To work around, we ask for forgiveness...
        try:
            return hash(self.soft_metadata.mtype)
        except AttributeError:  # only reached by IPython pretty-printer, I hope..
            return id(self)

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


# Python 2/3-compatibility trick copied from six
def with_metaclass(meta, *bases):
    """Create a base class with a metaclass."""
    # This requires a bit of explanation: the basic idea is to make a dummy
    # metaclass for one level of class instantiation that replaces itself with
    # the actual metaclass.
    class metaclass(meta):
        def __new__(cls, name, this_bases, d):
            return meta(name, bases, d)
    return type.__new__(metaclass, 'temporary_class', (), {})


class BaseEntity(with_metaclass(MetaEntity)):
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
        if uuid:
            uuid = asStr(uuid)
        dims = [asStr(d['name']) for d in meta['dimensions']]
        if isinstance(dimension_sizes, dict):
            dimension_sizes = [dimension_sizes[label] for label in dims]
        self.soft_internal_dimension_info = dimension_sizes

        self.__soft_entity__ = softpy.entity_t(
            meta.name,                          # get_meta_name
            meta.version,                       # get_meta_version
            meta.namespace,                     # get_meta_namespace
            dims,                               # get_dimensions
            self.soft_internal_dimension_size,  # get_dimension_size
            self.soft_internal_store,           # store
            self.soft_internal_load,            # load
            uuid,                               # id
            self,                               # user_data
        )

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

    def __repr__(self):
        return '%s(%s)' % (self.soft_get_meta_name(), self.soft_to_json())

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
            dims = softpy.entity_get_dimensions(self.__soft_entity__)
            ind = dims.index(label)
            return self.soft_internal_dimension_info[ind]
        else:
            # Associate property name and dimension index with dimensions,
            # for fast retrival of dynamic sizes
            assert self.soft_internal_dimension_info is None
            dimensions = softpy.entity_get_dimensions(self.__soft_entity__)
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
            softpy.datamodel_append_dimension(datamodel, label, value)

        for name in self.soft_get_property_names():
            typename = self.soft_get_property_typename(name)
            ptype = self.soft_get_property_type(name)
            dims = self.soft_get_property_dims(name)
            value = self.soft_get_property(name)
            if value is Uninitialized:
                raise SoftUninitializedError(
                    'Uninitialized data for "%s.%s"' % (
                        self.__class__.__name__, name))
            elif not dims and not isinstance(value, ptype):
                value = ptype(value)

            if not dims:
                setter = getattr(_softpy, 'datamodel_append_' + typename)
            elif typename == 'string' or typename == 'string_list':
                assert len(dims) == 1
                setter = getattr(_softpy, 'datamodel_append_string_list')
            elif len(dims) == 1:
                setter = getattr(_softpy, 'datamodel_append_array_' + typename)
            else:
                setter = getattr(_softpy, 'datamodel_append_array_%s_%dd' % (
                    typename, len(dims)))
            #print('*** name=%r: %r -> %r' % (name, type(value), setter))
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
            typename = self.soft_get_property_typename(name)
            dims = self.soft_get_property_dims(name)
            if not dims:
                getter = getattr(_softpy, 'datamodel_get_' + typename)
            elif typename == 'string_list' or typename == 'string':
                assert len(dims) == 1
                getter = getattr(_softpy, 'datamodel_get_string_list')
            elif len(dims) == 1:
                getter = getattr(_softpy, 'datamodel_get_array_' + typename)
            else:
                getter = getattr(_softpy, 'datamodel_get_array_%s_%dd' % (
                    typename, len(dims)))
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
            ptype = self.soft_get_property_type(name)
            if   (not isinstance(value, ptype) and
                  not value is Uninitialized and
                  not ptype == str):
                value = ptype(value)
            setattr(self, name, value)

    def soft_get_id(self):
        """Returns entity id."""
        return softpy.entity_get_id(self.__soft_entity__)

    def soft_get_meta_name(self):
        """Returns entity name."""
        return softpy.entity_get_meta_name(self.__soft_entity__)

    def soft_get_meta_version(self):
        """Returns entity version."""
        return softpy.entity_get_meta_version(self.__soft_entity__)

    def soft_get_meta_namespace(self):
        """Returns entity name space."""
        return softpy.entity_get_meta_namespace(self.__soft_entity__)

    @classmethod
    def soft_get_meta_description(cls):
        """Returns description of entity."""
        return cls.soft_metadata.get('description', '')

    def soft_get_dimensions(self):
        """Returns a list of dimension labels."""
        return softpy.entity_get_dimensions(self.__soft_entity__)

    def soft_get_dimension_size(self, label):
        """Returns size of dimension `label`.  If `label` is not a valid
        dimension label -1 is returned."""
        return softpy.entity_get_dimension_size(self.__soft_entity__, label)

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
    def soft_get_property_typename(cls, name):
        """Returns the type name of property `name`."""
        typename = _get_prop_info(cls, asStr(name), 'type')
        if not typename:
            raise SoftMetadataError(
                'property "%s" has no type information' % name)
        return asStr(typename)

    @classmethod
    def soft_get_property_type(cls, name):
        """Returns the type of property `name`."""
        typemaps = dict(string=str, string_list=str)
        if not cls.soft_get_property_dims(name):
            # Should uint8 map to bytes?
            # Issue: datamodel_append_uint8() does only accept ints not bytes
            typemaps.update(dict(int8=int, uint8=int, int16=int, uint16=int,
                                 int32=int, uint32=int, blob=bytes))
            if sys.version_info.major == 2:
                typemaps.update(dict(int64=long, uint64=long))
            else:
                typemaps.update(dict(int64=int, uint64=int))
        typename = cls.soft_get_property_typename(name)
        if typename in typemaps:
            return typemaps[typename]
        else:
            return getattr(np, typename)

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
        sizes = [(label, softpy.get_dimension_size(self.__soft_entity__, label))
                 for label in softpy.get_dimensions(self.__soft_entity__)]
        dim_sizes = []
        for sizeexpr in self.soft_get_property_dims(asStr(name)):
             for label, size in sizes:
                 sizeexpr = sizeexpr.replace(label, str(size))
             dim_sizes.append(arithmetic_eval(sizeexpr))
        return dim_sizes

    def soft_update(self, other):
        """Populate this instance from `other`, where `other` must be an
        instance of the same type as this one."""
        if self.soft_metadata.mtype != other.soft_metadata.mtype:
            raise SoftError(
                'cannot update instance of type %r from an instance of '
                'type %r' % (self.soft_metadata.mtype,
                             other.soft_metadata.mtype))
        for pname in self.soft_get_property_names():
            self.soft_set_property(pname, other.soft_get_property(pname))



    #-----------------------------------------------------------------------
    # FIXME - use the json storage when that is implemented
    #
    # These methods are implemented because of urgent need and lack of
    # a working json storage...
    def soft_as_dict(self):
        """Returns a dict representation of this instance."""
        return {
            'meta': {
                'name': self.soft_get_meta_name(),
                'version': self.soft_get_meta_version(),
                'namespace': self.soft_get_meta_namespace(),
            },
            'dimensions': {dname: self.soft_get_dimension_size(dname)
                           for dname in self.soft_get_dimensions()},
            'properties': {pname: self.soft_get_property(pname)
                           for pname in self.soft_get_property_names()},
        }

    def soft_from_dict(self, d):
        """Populate self from a dict."""
        meta = d['meta']
        # FIXME - check if translator exists... if so apply it
        if     (meta['name'] != self.soft_get_meta_name() or
                meta['version'] != self.soft_get_meta_version() or
                meta['namespace'] != self.soft_get_meta_namespace()):
            raise SoftError('expect instance of type %s/%s-%s got %s/%s-%s' % (
                self.soft_get_meta_namespace(),
                self.soft_get_meta_name(),
                self.soft_get_meta_verson(),
                meta['namespace'], meta['name'], meta['version']))
        for pname, value in d['properties'].items():
            self.soft_set_property(pname, value)
        self.soft_internal_check_dimension_sizes()

    def soft_to_json(self, indent=2, sort_keys=True):
        """Returns a json representation of this string."""
        return json_dumps(self.soft_as_dict(), indent=indent, sort_keys=sort_keys)

    def soft_from_json(self, s):
        """Initialise self from a json string."""
        d = json_loads(s)
        self.soft_from_dict(d)

    #-----------------------------------------------------------------------


def _get_prop_info(cls, name, field, default=None):
    """Help function used by BaseEntity class methods."""
    for p in cls.soft_metadata['properties']:
        if p['name'] == name:
            return p.get(field, default)
    raise SoftInvalidPropertyError('%s has no property named "%s"' % (
        cls.soft_metadata['name'], field))



def entity(name, version=None, namespace=None):
    """Factory fuction for creating an Entity class object.

    See the class docstring for Metadata for supported values for
    `name`, `version` and `namespace`."""
    meta = Metadata(name, version, namespace)

    # Create a dummy C-level entity for the returned metadata.
    # This allows us to add this metadata to a collection.
    # The assigned UUID is generated from a MD5 hash of the metadata
    # name, version and namespace.
    e = softpy.entity_t(
        'entity_schema',                    # get_meta_name
        '0.1'          ,                    # get_meta_version
        'org.sintef.soft',                  # get_meta_namespace
        [],                                 # get_dimensions
        [],                                 # get_dimension_size
        lambda e, d: None,                  # store
        lambda e, d: None,                  # load
        meta.get_uuid(),                    # id
        None,                               # user_data
    )

    attr = dict(
        soft_metadata=meta,
        __soft_entity__=e,
        __reduce__=lambda self: (_instantiate, (meta.get_json(indent=None), )),
    )
    return type(meta.name, (BaseEntity, ), attr)

# Mark the entity() factory as safe for unpickling
entity.__safe_for_unpickling__ = True

def _instantiate(s):
    """A help function that helps pickle instantiating an instance of the
    entity described by `s`."""
    meta = Metadata(s)
    cls = entity(meta)
    return cls()


def load_entity(filename):
    """A convenience function to read an entity from a json file."""
    with open(filename) as f:
        e = entity(f)
    return e

def derived_property_exception(msg=''):
    """Convinient function for raising SettingDerivedPropertyError
    within lambdas.

    See also: SettingDerivedPropertyError."""
    raise SettingDerivedPropertyError(msg)
