# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import print_function

import os

from . import softpy
from .softpy import asStr, asBytes
from .errors import SoftError
from .translators import translate
from .utils import json_loads, json_dumps, json_load, json_dump


class SoftStorageError(SoftError):
    """Error when storing/loading to/from a storage."""
    pass

class SoftStorageClosedError(SoftStorageError):
    """Raised when working on a closed storage."""
    pass


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

    Notes
    -----
    Options for the default drivers:

    hdf5 : "append=yes"  - append to the hdf5 file, otherwise truncate.
    """
    def __init__(self, driver, uri, options=None):
        self.driver = driver
        self.uri = uri
        self.options = options

        # FIXME - hack for json, remove when it is implemented in core
        if driver == 'json':
            pass
        else:
            self.storage = softpy.storage_create(driver, uri, options)
            self.strategy = softpy.storage_get_storage_strategy(self.storage)

        self._closed = False

    def save(self, instance, uuid=None):
        """Saves instance in this storage.  By default `instance` is stored
        under its own id, but by providing `uuid` you can store it under this
        id instead."""
        if self.closed:
            raise SoftStorageClosedError('Storage %r is closed.' % self.uri)

        # FIXME - hack for json, remove when it is implemented in core
        if self.driver == 'json':
            return json_hack_save(self, instance, uuid)

        e = softpy.get_c_entity(instance)
        if isinstance(e, softpy.collection_s):
            softpy.storage_save(self.storage, e)
        else:
            datamodel = softpy.storage_strategy_get_datamodel(self.strategy)
            softpy.datamodel_set_id(
                datamodel, asStr(uuid if uuid else e.id))
            softpy.datamodel_set_meta_name(
                datamodel, asStr(softpy.get_meta_name(e)))
            softpy.datamodel_set_meta_version(
                datamodel, asStr(softpy.get_meta_version(e)))
            softpy.datamodel_set_meta_namespace(
                datamodel, asStr(softpy.get_meta_namespace(e)))
            softpy.entity_store(e, datamodel)   # Ask the instance to fill out
                                                # the datamodel
            softpy.storage_strategy_store(self.strategy, datamodel)
            softpy.storage_strategy_free_datamodel(datamodel)

    def load(self, instance, uuid=None):
        """Fills `instance` with data loaded from this storage.  By default
        `instance` is loaded from its own id, but by providing `uuid`
        you can load it from this id instead."""
        if self.closed:
            raise SoftStorageClosedError('Storage %r is closed.' % self.uri)

        e = softpy.get_c_entity(instance)
        if uuid is None:
            uuid = e.id

        #
        # Implicit translation
        #
        # FIXME - this is yet another experimental feature that should go
        #         intosoft core if it turns out to work
        #
        # Hmm, we need an API for queuing the storage for the type
        # (name, version, namespace) of an instance with a given UUID
        # in order to know when to translate.  For now we will
        # demonstrate the concept using the json-hack below...

        # FIXME - hack for json, remove when it is implemented in core
        if self.driver == 'json':

            # Get source type, i.e. the type of the instance with this
            # uuid in the storage
            src_mtype = json_hack_get_mtype(self, uuid)
            if not src_mtype:
                raise SoftStorageError(
                    'UUID %r is not found in storage %r' % (uuid, storage.uri))

            # Get target type, i.e.
            target_mtype = (softpy.get_meta_name(e),
                            softpy.get_meta_version(e),
                            softpy.get_meta_namespace(e))

            # Populate the instance.
            # If the source type differ from the target type, we will
            # first create an uninitialised instance of the source type
            # populate it and then use an translator to populate the
            # target instance.
            if src_mtype == target_mtype:
                json_hack_load(self, instance, uuid)
            else:
                cls = entity(src_mtype)
                src_instance = cls()
                json_hack_load(self, src_instance, uuid)
                target_instance = translate(target_mtype, [src_instance])
                instance.soft_update(target_instance)

        else:
            if isinstance(e, softpy.collection_s):
                softpy.collection_load(self.storage, e)
            else:
                datamodel = softpy.storage_strategy_get_datamodel(self.strategy)
                softpy.datamodel_set_id(
                    datamodel, asStr(uuid))
                softpy.datamodel_set_meta_name(
                    datamodel, asStr(softpy.get_meta_name(e)))
                softpy.datamodel_set_meta_version(
                    datamodel, asStr(softpy.get_meta_version(e)))
                softpy.datamodel_set_meta_namespace(
                    datamodel, asStr(softpy.get_meta_namespace(e)))
                softpy.storage_strategy_start_retrieve(self.strategy, datamodel)
                softpy.entity_load(e, datamodel)  # Ask instance to fill out itself
                softpy.storage_strategy_end_retrieve(self.strategy, datamodel)
                softpy.storage_strategy_free_datamodel(datamodel)

    def close(self):
        """Closes current storage."""
        if self.closed:
            raise SoftStorageClosedError('Storage %r is closed.' % self.uri)

        # FIXME - hack for json, remove when it is implemented in core
        if self.driver == 'json':
            pass
        else:
            softpy.storage_free_storage_strategy(self.strategy)
            softpy.storage_free(self.storage)

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



#=======================================================================
# json hacks
#=======================================================================

def json_hack_save(storage, instance, uuid=None):
    """Stores `instance` to `storage`.

    If `uuid` is provided, the instance is stored with this uuid, otherwise
    it is stored with its own uuid.

    Recognised options are:
      append : whether to append to storage instead of overwriting the file [yes]
      indent : non-negative integer, level of indentation [2]
      sort : whether to sort the output by key [no]
    """
    assert storage.driver == 'json'
    optdict = _parse_options(storage.options)
    if istrue(optdict.get('append', 'yes')) and os.path.exists(storage.uri):
        with open(storage.uri, 'r') as f:
            d = json_load(f)
    else:
        d = {}

    if hasattr(instance, 'mtype') and hasattr(instance, 'get_uuid'):
        uuid = uuid if uuid else instance.get_uuid()
        dd = dict(instance)
        dd['meta'] = {'name': 'entity_schema',
                      'version': '0.3',
                      'namespace': 'org.sintef.soft'}
        d[uuid] = dd
    else:
        e = softpy.get_c_entity(instance)
        if isinstance(e, softpy.collection_s):
            uuid = uuid if uuid else instance.uuid
            d[uuid] = instance.soft_as_dict()
            for label in instance.get_labels():
                ent = instance.get_instance(label)
                d[instance.get_uuid(label)] = ent.soft_as_dict()
        elif isinstance(e, softpy.entity_t):
            uuid = uuid if uuid else instance.soft_get_id()
            d[uuid] = instance.soft_as_dict()
        else:
            raise TypeError(
                '`instance` must be an Entity, Collection or Metadata')

    with open(storage.uri, 'w') as f:
        json_dump(d, f, indent=int(optdict.get('indent', 4)),
                  sort_keys=istrue(optdict.get('sort', 'no')))


def json_hack_load(storage, instance, uuid=None):
    """Populates `instance` from `storage`.

    If `uuid` is provided, the instance is loaded from this uuid, otherwise
    it is loaded from its own uuid.
    """
    assert storage.driver == 'json'
    with open(storage.uri) as f:
        d = json_load(f)

    e = softpy.get_c_entity(instance)
    if not uuid:
        uuid = softpy.get_id(e)

    name = softpy.get_meta_name(e)
    version = softpy.get_meta_version(e)
    namespace = softpy.get_meta_namespace(e)
    meta = d[uuid]['meta']
    if (    meta['name'] != name or
            meta['version'] != version or
            meta['namespace'] != namespace):
        raise SoftError('Cannot load %s/%s-%s into a %s/%s-%s' % (
            meta['name'], meta['version'], meta['namespace'],
            name, version, namespace))

    if isinstance(e, softpy.collection_s):
        instance.soft_from_dict(
            d[uuid], storage.driver, storage.uri, storage.options)
    elif isinstance(e, softpy.entity_t):
        instance.soft_from_dict(d[uuid])
    else:
        raise TypeError(
            '`instance` must be an Entity or Collection, got %r' %
            type(instance))


def json_hack_has_uuid(storage, uuid):
    """Returns true if an instance with the given `uuid` exists in
    `storage`."""
    return json_hack_get_mtype() is not None


def json_hack_get_mtype(storage, uuid):
    """Returns the (name, version, namespace) of the instance with the
    given `uuid` in `storage`.

    None is returned if `uuid` is not in `storage`."""
    assert storage.driver == 'json'
    with open(storage.uri) as f:
        d = json_load(f)
    if uuid in d:
        meta = d[uuid]['meta']
        return (asStr(meta['name']),
                asStr(meta['version']),
                asStr(meta['namespace']))
    return None


def json_hack_get_uuids(storage):
    """Returns a generator over all the UUID's in `storage`."""
    assert storage.driver == 'json'
    with open(storage.uri) as f:
        d = json_load(f)
    for uuid in d.keys():
        yield uuid



# FIXME - get rid to these cyclic imports
from .entity import entity


def _parse_options(options):
    """Parses options string and returns is as a dict."""
    d = {}
    if options:
        for opt in options.split('&'):
            k, v = opt.split('=', 1)
            d[k] = v
    return d


def istrue(value):
    """Returns false if the string `value` corresponds to false, otherwise
    true is returned.  The following values are considered false:
    "false", "no", "0", "off", ".false.".
    """
    if value.lower() in ('false', 'no', '0', 'off', '.false.'):
        return False
    else:
        return True


# FIXME: circular import - get rid of this when json is properly
# implemented in soft core
from .entity import entity
