#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import pickle

import numpy as np
import softpy

try:
    import dill
    HAVE_DILL = True
except ImportError:
    HAVE_DILL = False
    print("You need dill in order to pickle entities")

class Data(object):
    pass


def store(e, datamodel):
    softpy.datamodel_append_string(datamodel, 'name', 'NaCl')
    softpy.datamodel_append_int32(datamodel, 'spacegroup_no', 225)
    softpy.datamodel_append_double(datamodel, 'lattice_parameter', 5.64)
    softpy.datamodel_append_array_double(
        datamodel, 'positions', 
        np.array([(0.0, 0.0, 0.0), (0.5, 0.5, 0.5)]).flatten())

def load(e, datamodel):
    d = e.user_data
    d.name = softpy.datamodel_get_string(datamodel, 'name')
    d.spgr = softpy.datamodel_get_int32(datamodel, 'spacegroup_no')
    d.latt = softpy.datamodel_get_double(datamodel, 'lattice_parameter')
    d.posi = softpy.datamodel_get_array_double(datamodel, 'positions')

e = softpy.entity_t(get_meta_name='MyStructure',
                    #get_meta_version='0.1.1',
                    get_meta_version=lambda self: '0.1.1',
                    get_meta_namespace='http://sintef.no/meta/soft',
                    get_dimensions=['I', 'J'],
                    get_dimension_size=[3, 4],
                    load=load,
                    store=store, 
                    user_data=Data())

assert softpy.entity_get_meta_name(e) == 'MyStructure'
assert softpy.entity_get_meta_version(e) == '0.1.1'
assert softpy.entity_get_meta_namespace(e) == 'http://sintef.no/meta/soft'
assert softpy.entity_get_dimensions(e) == ['I', 'J']
assert softpy.entity_get_dimension_size(e, 'I') == 3
assert softpy.entity_get_dimension_size(e, 'J') == 4
assert softpy.entity_get_id(e) == e.id

assert e.name == 'MyStructure'
assert e.version == '0.1.1'
assert e.namespace == 'http://sintef.no/meta/soft'
assert e.dimensions == ['I', 'J']
assert e.dimension_sizes == [3, 4]

s = softpy.Storage('hdf5', 'softpy-test-entity.h5')
s.save(e)

s.load(e)
d = e.user_data
assert d.name == 'NaCl'
assert d.spgr == 225
assert d.latt == 5.64
assert np.allclose(
    d.posi, np.array([(0.0, 0.0, 0.0), (0.5, 0.5, 0.5)]).flatten())
                   
del e


class Person(object):
    def __init__(self, name='', age=0.0, distances=(), uuid=None):
        self.name = name
        self.age = age
        self.distances = distances  # km walked the last n days
        self.__soft_entity__ = softpy.entity_t(
            get_meta_name='Person',
            get_meta_version='0.1',
            get_meta_namespace='http://sintef.no/meta/soft',
            get_dimensions=['ndays'],
            get_dimension_size=[len(distances)],
            load=self.load,
            store=self.store, 
            id=uuid)

    def store(self, e, datamodel):
        softpy.datamodel_append_string(datamodel, 'name', self.name)
        softpy.datamodel_append_int32(datamodel, 'age', self.age)
        softpy.datamodel_append_array_double(
            datamodel, 'distances', self.distances)
        
    def load(self, e, datamodel):
        self.name = softpy.datamodel_get_string(datamodel, 'name')
        self.age = softpy.datamodel_get_int32(datamodel, 'age')
        self.distances = softpy.datamodel_get_array_double(
            datamodel, 'distances')
        
person = Person('Jack', 42, [5.4, 7.6, 1.1])

with softpy.Storage('hdf5', 'x.h5') as s:
    s.save(person)


p = Person(uuid=person.__soft_entity__.id)
with softpy.Storage('hdf5', 'x.h5') as s:
    s.load(p)

assert p.name == 'Jack'
assert p.age == 42
assert np.all(p.distances == person.distances)


# Ensure that entities are pickleable
if HAVE_DILL:
    dump = pickle.dumps(person)
    person2 = pickle.loads(dump)
    for k in person.__dict__:
        assert getattr(person2, k) == getattr(person, k)
