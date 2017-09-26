#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os

import softpy


thisdir = os.path.dirname(__file__)


coll = softpy.Collection()
assert coll.get_num_entities() == 0
assert coll.get_num_relations() == 0

coll.name = 'foo'
assert coll.name == 'foo'

coll.version = '1.0'
assert coll.version == '1.0'

with open(os.path.join(thisdir, 'person.json')) as f:
    Person = softpy.entity(f)

jack = Person(name='Jack', age=42, skills=[])
alda = Person(name='Alda', age=39, skills=['foo'])
fred = Person(name='Fred', age=12, skills=['bar'])
coll.add('Jack', jack)
coll.add('Alda', alda)
coll.add('Fred', fred)
assert coll.get_num_entities() == 3

coll.add_relation('Jack', 'loves', 'Alda')
coll.add_relation('Jack', 'loves', 'Fred')
coll.add_relation('Jack', 'husband_of', 'Alda')
coll.add_relation('Jack', 'father_of', 'Fred')

assert coll.find_relations('Jack', 'loves') == {'Fred', 'Alda'}
assert coll.find_relations('Alda', '^loves') == {'Jack'}
assert coll.find_relations('Jack', 'husband_of') == {'Alda'}
assert coll.find_relations('Fred', '^father_of') == {'Jack'}

assert coll.get_labels() == {'Alda', 'Fred', 'Jack'}
assert coll.get_uuid('Fred') == fred.soft_get_id()
assert coll.get_name('Fred') == 'Person'
assert coll.get_version('Fred') == '0.2.4'
assert coll.get_namespace('Fred') == 'http://sintef.no/meta/soft'

# Test to add another collection to coll
subcoll = softpy.Collection()
anna = Person(name='Anna', age=78, skills=['piano'])
subcoll.add('Anna', anna)
coll.add('relatives', subcoll)

# Test to add metadata to coll
coll.add('Person', Person)

# Save the collection to hdf5
# Note that we must set append=yes to also store the instances. This will
# cause trouble with existing names the second time we run the test, so
# we remove the h5 file first.
if os.path.exists('softpy-test-collection.h5'):
    os.remove('softpy-test-collection.h5')
with softpy.Storage('hdf5', 'softpy-test-collection.h5', 'append=yes') as s:
    coll.save(s)


# Create a metadata database with all metadata defined in json files
# in the current directory - needed in order to instanciate entities
# and metadata read from the stored hdf5 file
#softpy.register_metadb(softpy.JSONDirMetaDB(thisdir))
softpy.register_jsondir(thisdir)


# Create a new collection, loaded from hdf5
coll2 = softpy.Collection(uuid=coll.uuid, driver='hdf5',
                          uri='softpy-test-collection.h5')

assert coll2.uuid == coll.uuid
assert coll2.get_num_entities() == coll.get_num_entities()
assert coll2.get_num_relations() == coll.get_num_relations()
assert coll2.find_relations('Jack', 'loves') == coll.find_relations(
    'Jack', 'loves')
fred2 = coll2.get_instance('Fred')
assert fred2.name == 'Fred'
assert fred2.age == 12
assert fred2.skills == ['bar']

subcoll2 = coll2.get_instance('relatives')
assert subcoll2.get_labels() == {'Anna'}

Person2 = coll2.get_instance('Person')
assert Person2.property_names == Person.property_names
assert Person2 == Person
