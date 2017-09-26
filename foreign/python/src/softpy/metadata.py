# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import print_function

import sys
import os
import json
import warnings
import re
import fnmatch
from glob import glob

from . import softpy
from .softpy import asStr, asBytes
from .errors import SoftError


class SoftMissingMetadataError(SoftError):
    """Raised when metadata cannot be found in the database."""
    pass


class Metadata(dict):
    """A class representing SOFT metadata.

    Parameters
    ----------
    name : str | Metadata | Entity | Entity instance | file-like | dict | tuple
        If `version` and `namespace` are given, this is the metadata name.

        Otherwise, this is a full description of the metadata in one of
        following forms:
          - Metadata object or dict
          - SOFT entity or entity instance
          - file-like object with a read() method with the metadata in json-
            format.
          - string with the metadata in json-format
          - tuple with (name, version, namespace). The full description is
            looked up in the metadata database.
    version : None | str
        Metadata version or None if `name` provides full description
        of the metadata.
    namespace : None | str
        Metadata namespace or None if `name` provides full description
        of the metadata."""
    def __init__(self, name, version=None, namespace=None):
        if version is None or namespace is None:
            if hasattr(name, 'soft_metadata'):
                d = name.soft_metadata
            elif hasattr(name, 'read'):
                d = json.load(name)
            elif isinstance(name, str):
                d = json.loads(name)
            elif sys.version_info.major == 2 and isinstance(name, unicode):
                d = json.loads(name.decode('utf8'))
            elif isinstance(name, dict):
                d = name
            elif isinstance(name, tuple):
                name, version, namespace = name
                d = find_metadata(name, version, namespace)
            else:
                raise TypeError(
                    'Cannot convert %s to metadata' % (type(name),))
        else:
            d = find_metadata(name, version, namespace)
        self.update(d)
        # A kind of hack, that automatically adds all metadata to the
        # metadata database
        if not self in metaDB:
            dict.__setitem__(metaDB, self.get_uuid(), self)

    def __str__(self):
        return self.get_json()

    def __repr__(self):
        return 'Metadata(%s)' % self.get_json()

    def __hash__(self):
        return hash(self.mtype)

    def __eq__(self, other):
        return self.mtype == other.mtype

    name = property(lambda self: asStr(self['name']),
                    doc='Metadata name.')
    version = property(lambda self: asStr(self['version']),
                       doc='Metadata version.')
    namespace = property(lambda self: asStr(self['namespace']),
                         doc='Metadata namespace.')
    description = property(lambda self: asStr(self['description']),
                           doc='Description of this metadata.')
    mtype = property(lambda self: (self.name, self.version, self.namespace),
                     doc='A (name, version, namespace)-tuple uniquely '
                         'identifying the metadata.')
    dimensions = property(lambda self: [
        str(asStr(d['name'])) for d in self['dimensions']],
                          doc='List of dimension labels.')
    property_names = property(lambda self: [
            asStr(p['name']) for p in self['properties']],
                              doc='List of property names.')

    def get_uuid(self):
        """Returns an UUID generated from a MD5 hash of the metadata name,
        version and namespace."""
        return softpy.uuid_from_entity(self.name, self.version, self.namespace)

    def get_json(self, indent=2, sort_keys=True):
        """Returns a json string representing this metadata."""
        return json.dumps(self, indent=indent, sort_keys=sort_keys)



#
# Metadata database interface
# ===========================

# FIXME: functionality should be implemented in C++?
#
# FIXME2: improve the design
#         A better design would be to create a build-in entity for metadata
#         (i.e. an entity that encode the entity schema).  Then we could
#         save the metadata using any of the already supported storage
#         backends.


class MetaDB(dict):
    """Generic metadata database interface.

    It is implemented as a dict mapping metadata uuid's to Metadata objects.

    Keys may be provided either as (name, version, namespace)-tuples or
    as metadata uuid's.
    """
    def __setitem__(self, key, meta):
         dict.__setitem__(self, self.touuid(key), Metadata(meta))

    def __getitem__(self, key):
         return dict.__getitem__(self, self.touuid(key))

    def __contains__(self, key):
         return dict.__contains__(self, self.touuid(key))

    @staticmethod
    def touuid(key):
        """Returns `key` converted to a metadata uuid. `key` may be either a
        (name, version, namespace)-tuple or an uuid."""
        if isinstance(key, tuple):
            name, version, namespace = key
            return softpy.uuid_from_entity(name, version, namespace)
        elif isinstance(key, str) and re.match(
                '[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
                '[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$', key):
            return key
        elif isinstance(key, Metadata):
            return key.get_uuid()
        raise TypeError(
            'key must be an uuid or (name, version, namespace)-tuple: %r' % (
                key, ))

    def find(self, name='*', version='*', namespace='*'):
        """Returns a list of Metadata objects matching the given `name`,
        `version` and `namespace` shell patterns."""
        values = self.values()
        if name is not '*':
            rname = re.compile(fnmatch.translate(name))
            values = [v for v in values if rname.match(v)]
        if version is not '*':
            rversion = re.compile(fnmatch.translate(version))
            values = [v for v in values if rversion.match(v)]
        if namespace is not '*':
            rnamespace = re.compile(fnmatch.translate(namespace))
            values = [v for v in values if rnamespace.match(v)]
        return list(values)

    def add(self, meta):
        """Add Metadata object `meta` to database."""
        meta = Metadata(meta)
        dict.__setitem__(self, meta.get_uuid(), meta)

    def mtypes(self):
        """Returns a list of (name, version, namespace)-tuples for all
        registered metadata."""
        return [meta.mtype for meta in self.values()]

    def get_json(self, indent=2, soft_keys=True):
        """Returns a JSON representation of the database."""
        return json.dumps(self, indent=indent, sort_keys=soft_keys)


# Exposed instance of MetaDB currently caching all metadata
metaDB = MetaDB()

# List of instances of deprecated metadata databases
_metadbs = []

def register_metadb(metadb):
    """Registers metadata database `metadb`."""
    _metadbs.append(metadb)

def register_jsondir(path):
    """Registers all the entities defined in json-files in the directory
    with the given path."""
    filenames = glob(os.path.join(path, '*.json'))
    for filename in filenames:
        with open(filename) as f:
            d = json.load(f)
            # Check if d looks like an entity
            required = set(['name', 'version', 'namespace', 'properties'])
            optional = set(['description', 'dimensions'])
            allowed = required.union(optional)
            s = set(d.keys())
            if required.issubset(s) and not s.difference(allowed):
                Metadata(d)  # this automatically registers the metadata...

def find_metadata(name, version, namespace):
    """Search through all registered metadata databases and return
    a Metadata object corresponding to `name`, `version`, `namespace`.
    """
    return find_metadata_uuid(softpy.uuid_from_entity(
        asStr(name), asStr(version), asStr(namespace)))

def find_metadata_uuid(uuid):
    """Search through all registered metadata databases and return
    a Metadata object corresponding to `name`, `version`, `namespace`.
    """
    if uuid in metaDB:
        return metaDB[uuid]
    for db in _metadbs:
        try:
            meta = db.find_uuid(uuid)
        except SoftMissingMetadataError:
            pass
        else:
            metaDB[uuid] = meta
            return meta
    raise SoftMissingMetadataError(
        'Cannot find metadata with uuid: ' + uuid)


# When the module is loaded, search the directories in environment
# variable SOFT_ENTITIES for JSON-files and initialise metaDB from
# them.
#
# Since we may want to load the entities from online metadata databases,
# we should allow SOFT_ENTITIES to contain urls. That means that we cannot
# use colon as separator.  Semicolon seems better.
for d in os.environ.get('SOFT_ENTITIES', '').split(';'):
    if d:
        register_jsondir(d)
del d  # clean up the module namespace
