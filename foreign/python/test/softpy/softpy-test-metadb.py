#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os

import softpy


thisdir = os.path.dirname(__file__)


jsondb = softpy.JSONMetaDB('metadb.json')
jsondb.clear()
assert jsondb.has('Person', '0.2.4', 'http://sintef.no/meta/soft') is False
jsondb.insert(open('person.json'))
assert jsondb.has('Person', '0.2.4', 'http://sintef.no/meta/soft') is True
types = jsondb.types()
assert types[0] == ('Person', '0.2.4', 'http://sintef.no/meta/soft')

softpy.register_metadb(jsondb)

meta = softpy.Metadata(name='Person', version='0.2.4',
                       namespace='http://sintef.no/meta/soft')

Person = softpy.entity(name='Person', version='0.2.4',
                       namespace='http://sintef.no/meta/soft')
jack = Person(name='Jack', age=42, skills=['foo', 'bar'])
assert jack.soft_metadata == meta
jsondb.close()


try:
    import pymongo
except ImportError:
    import warnings
    warnings.warn('pymongo not installed - skipping mongodb test...')
else:
    mongodb = softpy.MongoMetaDB('mongodb://localhost:27017/', 'metadb_test',
                                 'metadata')
    mongodb.clear()
    assert mongodb.has(
        'Person', '0.2.4', 'http://sintef.no/meta/soft') is False
    mongodb.insert(open('person.json'))
    assert mongodb.has(
        'Person', '0.2.4', 'http://sintef.no/meta/soft') is True

    types = mongodb.types()
    assert types[0] == ('Person', '0.2.4', 'http://sintef.no/meta/soft')

    softpy.register_metadb(mongodb)

    meta = softpy.Metadata(name='Person', version='0.2.4',
                           namespace='http://sintef.no/meta/soft')

    Person = softpy.entity(name='Person', version='0.2.4',
                           namespace='http://sintef.no/meta/soft')
    jack = Person(name='Jack', age=42, skills=['foo', 'bar'])
    assert jack.soft_metadata == meta
    mongodb.close()
