# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import print_function

from . import softpy
from .softpy import asStr, asBytes
from .errors import SoftError
from .entity import entity
from .storage import Storage, json_hack_load, json_hack_save
from .metadata import find_metadata_uuid
from .utils import json_loads, json_dumps, json_load, json_dump


class SoftLabelError(SoftError):
    """Raised when a label cannot be found in a collection."""
    pass


class Collection(object):
    """A collection of entity instances with relations between them.

    With default arguments, it returns a new empty collection.

    If `uuid` is given, this will be the uuid of the new collection.
    This is normally used in combination with `driver`, `uri` and `options`
    to load the collection from a storage.

    See also: Storage
    """
    def __init__(self, uuid=None, driver=None, uri=None, options="append=yes"):
        if uuid:
            c = softpy.collection_create(uuid)
        else:
            c = softpy.collection_create_new()
        self.__soft_entity__ = c

        # FIXME - the C/C++ implementation of
        # softc_collection_get_name()/softc_collection_set_name()/+++
        # functions seems to store name/version/namespace separate
        # from the underlying entity...
        softpy.collection_set_name(c, softpy.get_meta_name(c))
        softpy.collection_set_version(c, softpy.get_meta_version(c))
        softpy.collection_set_namespace(c, softpy.get_meta_namespace(c))

        self._driver = driver
        self._uri = uri
        self._options = options

        # We store a Python-level references to all entity instances/collections
        # added to this collection.  This makes it possible to:
        #   - store the content of the collection together with the collection
        #   - retrieve elements added in this session without having their
        #     metadata in a in a metadata database
        self._cache = {}

        if driver:
            with Storage(driver=driver, uri=uri, options=options) as s:
                if driver == 'json':
                    # FIXME: hack
                    with open(uri, 'r') as f:
                        d = json_load(f)
                    if not self.uuid in d:
                        raise SoftError(
                            'storage %r has no uuid %r' % (uri, self.uuid))
                    self.soft_from_dict(d[self.uuid])
                else:
                    softpy.collection_load(s.storage, self.__soft_entity__)
                #softpy.collection_load(s.storage, self.__soft_entity__)

    def __delete__(self):
        collection_free(self.__softpy_entity__)
        object.__del__(self)

    def add(self, label, entity):
        """Register a new entity associated with the given label."""
        e = softpy.get_c_entity(entity)
        if isinstance(e, softpy.entity_t):
            softpy.collection_register_entity(
                self.__soft_entity__, asStr(label), e)
        elif isinstance(e, softpy.collection_s):
            softpy.collection_register_collection(
                self.__soft_entity__, asStr(label), e)
        else:
            raise SoftError('Only instances, collections and metadata '
                            'can be added to collections.')
        self._cache[label] = entity

    #def pop(self, label):
    #    return collection_pop(self.__soft_entity__, label)

    #def remove(self, label):
    #    collection_remove(self.__soft_entity__, label)

    #def add_dim(self, label, description=''):
    #    collection_add_dim(self.__soft_entity__, label, description)

    def add_relation(self, subject, predicate, object_):
        softpy.collection_add_relation(self.__soft_entity__, asStr(subject),
                                       asStr(predicate), asStr(object_))

    #def connect(self, subject, predicate, object_):
    #    collection_connect(self.__soft_entity__,
    #                       subject, predicate, object_)

    # FIXME: change name to get_num_instances() for consistency
    def get_num_entities(self):
        """Returns the number of instances."""
        return softpy.collection_num_entities(self.__soft_entity__)

    def get_num_relations(self):
        """Returns the total number of relations associated with this
        collection.

        Note: A collection will typically contain many more relations
        than the number of relations you have added with add_relation()
        because relations are also used to associate entity labels with
        the entity type, name, version , namespace and uuid."""
        return softpy.collection_num_relations(self.__soft_entity__)

    #def get_num_dim_maps(self):
    #     return collection_num_dim_maps(self.__soft_entity__)
    #
    #def get_dimensions(self):
    #     return collection_get_dimensions(self.__soft_entity__)

    def get_labels(self):
        """Returns a set with all registered entity labels."""
        return self.find_relations('Entity', '^is-a')

    def _find_attr(self, label, attr):
        """Returns the attribute `attr` of instance with given label."""
        attrs = self.find_relations(label, attr)
        if len(attrs) == 0:
            raise SoftLabelError(
                'Label %r is not found in Collection %r' % (label, self.uuid))
        assert len(attrs) == 1, 'max one instance with the same label'
        attr, = attrs
        return attr

    def get_uuid(self, label):
        """Returns the uuid of the entity instance with the given label."""
        return self._find_attr(label, 'id')

    def get_name(self, label):
        """Returns the name of the entity instance with the given label."""
        return self._find_attr(label, 'name')

    def get_version(self, label):
        """Returns the version of the entity instance with the given label."""
        return self._find_attr(label, 'version')

    def get_namespace(self, label):
        """Returns the namespace of the entity instance with the given
        label."""
        return self._find_attr(label, 'namespace')

    def get_instance(self, label, driver=None, uri=None, options=None,
                     expected_entity=None):
        """Returns the the entity instance associated with `label`
        from the storage specified with `driver`, `uri` and `options`.

        Default values for `driver`, `uri` and `options` can be set
        with the corresponding properties.  If this collection was
        loaded from a storage, these properties be initialised to the
        same storage.

        If `expected_entity` is given, the returned instance will be
        of this type.  If the stored instance is of another type, it
        will be translated.  If no matching translator is found, a
        SoftMissingTranslatorError will be raised.

        This method depends on that the metadata for `label` exists in
        a metadata database registered with softpy.register_metadb().
        """
        if label in self._cache:
            return self._cache[label]
        if label not in self.get_labels():
            raise SoftLabelError(
                'Label %r not found in Collection %r' % (label, self.uuid))
        driver = driver if driver else self._driver
        uri = uri if uri else self._uri
        options = options if options else self._options
        if not driver or not uri:
            raise SoftError('`driver` and `uri` must be given when no '
                             'defaults are set')
        name = self.get_name(label)
        version = self.get_version(label)
        namespace = self.get_namespace(label)
        uuid = self.get_uuid(label)
        if (name, namespace) == ('Collection', 'org.sintef.soft'):
            instance = Collection(uuid=uuid, driver=driver, uri=uri,
                                  options=options)
        elif (name, namespace) == ('entity_schema', 'org.sintef.soft'):
            meta = find_metadata_uuid(uuid)
            instance = entity(meta)
        else:
            if not expected_entity:
                expected_entity = (name, version, namespace)
            cls = entity(expected_entity)
            instance = cls(uuid=uuid, driver=driver, uri=uri, options=options)
        self._cache[label] = instance
        return instance

    name = property(
        lambda self: softpy.collection_get_name(self.__soft_entity__),
        lambda self, name: softpy.collection_set_name(
            self.__soft_entity__, name),
        doc='Collection name.'
    )

    version = property(
        lambda self: softpy.collection_get_version(self.__soft_entity__),
        lambda self, version: softpy.collection_set_version(
            self.__soft_entity__, version),
        doc='Collection version.'
    )

    namespace = property(
        lambda self: softpy.collection_get_namespace(self.__soft_entity__),
        lambda self, namespace: softpy.collection_set_namespace(
            self.__soft_entity__, namespace),
        doc='Collection name space.'
    )

    uuid = property(
        lambda self: self.__soft_entity__.id,
        doc='The uuid of this collection.')

    driver = property(
        lambda self: self._driver,
        lambda self, value: setattr(self, '_driver', value),
        doc='Default driver used by get_instances().')

    uri = property(
        lambda self: self._uri,
        lambda self, value: setattr(self, '_uri', value),
        doc='Default uri used by get_instances().')

    options = property(
        lambda self: self._options,
        lambda self, value: setattr(self, '_options', value),
        doc='Default options used by get_instances().')

    def find_relations(self, subject, predicate):
        """Returns a set with all relations matching the given `subject` and
        `predicate`.

        If `predicate` is preceded with "^", the match is inverted, i.e.
        all relations whos object matches `subject` and predicate matches
        the remaining of `predicate` are returned."""
        strlst = softpy.collection_find_relations(self.__soft_entity__,
                                                  subject, predicate)
        relations = set()
        for i in range(softpy.string_list_count(strlst)):
            relations.add(softpy.string_at(strlst, i))
        softpy.string_list_free(strlst)
        return relations

    def save(self, storage=None, store_content=True):
        """A convenient method for storing a collection.

        If `storage` is not given, the default storage will be used.

        If `store_content` is true, the content of the collection will
        also be stored.

        Note: if `storage` is given and `store_content` is true, the
        storage should have been created with option "append=yes".
        """
        if storage:
            pass  # ok, proceed...
        elif self.driver and self.uri:
            with Storage(self.driver, self.uri, self.options) as s:
                if self.driver == 'json':  # FIXME: hack
                    return json_hack_save(s, self, self.uuid)
                else:
                    return self.save(s, store_content)
                #return self.save(s, store_content)
        else:
            raise SoftError('`storage` must be given when no default storage')

        if store_content:
            for label, e in self._cache.items():
                if isinstance(e, Collection):
                    e.save(storage, store_content=store_content)
                else:
                    storage.save(e)
        storage.save(self)

    #-----------------------------------------------------------------------
    # FIXME - use the json storage when that is implemented
    #
    # These methods are implemented because of urgent need and lack of
    # a working json storage...
    def soft_as_dict(self):
        """Returns a dict representation of this instance."""
        triplets = []
        for label in self.get_labels():
            name = self.get_name(label)
            version = self.get_version(label)
            namespace = self.get_namespace(label)
            if (name, namespace) == ('Collection', 'org.sintef.soft'):
                metatype = 'Collection'
            elif (name, namespace) == ('entity_schema', 'org.sintef.soft'):
                metatype = 'Schema'
            else:
                metatype = 'Entity'
            triplets.append([label, 'name', name])
            triplets.append([label, 'version', version])
            triplets.append([label, 'namespace', namespace])
            triplets.append([label, 'is-a', metatype])
            triplets.append([label, 'id', self.get_uuid(label)])
        return {
            'meta': {
                'name': self.name,
                'version': self.version,
                'namespace': self.namespace,
            },
            'dimensions': {
                #'n-instances': self.get_num_entities(),
            },
            'properties': {
                #'__name__' = self.name,
                #'__version__' = self.version,
                'triplets': triplets,
            },
        }

    def soft_from_dict(self, d, driver=None, uri=None, options=None):
        """Populate self from a dict."""
        driver = driver if driver else self._driver
        uri = uri if uri else self._uri
        options = options if options else self._options
        if not driver or not uri:
            raise SoftError('`driver` and `uri` must be given when no '
                             'defaults are set')
        meta = d['meta']
        if     (meta['name'] != self.name or
                meta['version'] != self.version or
                meta['namespace'] != self.namespace):
            raise SoftError('expect instance of type %s/%s-%s got %s/%s-%s' % (
                self.namespace(), self.name(), self.verson(),
                meta['namespace'], meta['name'], meta['version']))
        triplets = d['properties']['triplets']
        for subj, pred, obj in triplets:
            self.add_relation(subj, pred, obj)

    def soft_to_json(self, indent=2, sort_keys=True):
        """Returns a json representation of this string."""
        return json_dumps(self.soft_as_dict(), indent=indent, sort_keys=sort_keys)

    def soft_from_json(self, s):
        """Initialise self from a json string."""
        d = json_loads(s)
        self.soft_from_dict(d)
    #-----------------------------------------------------------------------

    def __repr__(self):
        triplets = self.soft_as_dict()['properties']['triplets']
        tripletdict = {}
        for label, attr, value in triplets:
            if not label in tripletdict:
                tripletdict[label] = {}
            tripletdict[label][attr] = value
        return '%s(%s)' % (self.__class__.__name__,
                           json_dumps(tripletdict, indent=2, sort_keys=True))
