# -*- python -*-

%pythoncode %{
import sys


def get_c_entity(entity):
    """Returns a reference to the underlying C-level entity_t or
    collection_s."""
    if hasattr(entity, '__soft_entity__'):
        e = entity.__soft_entity__
    else:
        e = entity
    if not isinstance(e, (entity_t, collection_s)):
        raise TypeError('Not a proper entity or collection')
    return e

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


#def _get_entity_method(entity, name):
#    """Returns a reference to method `name` in `entity`.
#    """
#    if hasattr(entity, name):
#        attr = getattr(entity, name)
#        if hasattr(attr, '__call__'):
#            method = attr
#    method = getattr(entity, '_' + name)
#    assert hasattr(method, '__call__')
#    return method








#
# Convinience functions for returning entity info
# ===============================================
def _get_entity_info(e, field, *args):
    """Help function for returning info about entities."""
    if hasattr(e, '__soft_entity__'):
        e = e.__soft_entity__
    if isinstance(e, collection_s):
        return globals()['collection_get_' + field](e, *args)
    elif isinstance(e, entity_t):
        return globals()['entity_get_' + field](e, *args)
    else:
        raise TypeError('not an entity or collection: %r' % e)

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

def get_meta_mtype(e):
    """Returns a (name, version, namespace) tuple for entity `e`."""
    return (_get_entity_info(e, 'meta_name'),
            _get_entity_info(e, 'meta_version'),
            _get_entity_info(e, 'meta_namespace'))

def get_dimensions(e):
    """Returns list of dimension lables for entity `e`."""
    return _get_entity_info(e, 'dimensions')

def get_dimension_size(e, label):
    """Returns size of dimension `label` for entity `e`.  If `label` is
    not a valid dimension label -1 is returned."""
    return _get_entity_info(e, 'dimension_size', label)



%}
