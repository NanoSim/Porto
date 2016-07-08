#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os
import softpy

import numpy as np

thisdir = os.path.abspath(os.path.dirname(__file__))

metadata = """
{
    "name": "Structure",
    "version": "0.1",

    "namespace": "http://sintef.no/meta/soft",
    "description": "Incomplete description of an atomic structure",
    "dimensions": [
        {
            "name": "N",
            "description": "Number of atoms"
        }
    ],
    "properties": [
        {
            "name": "structure",
            "type": "string",
            "description": "Name of structure"
        },
        {
            "name": "symbols",
            "type": "string",
            "dims": [
                "N"
            ],
            "description": "List of atomic symbols"
        },
        {
            "name": "spacegroup",
            "type": "int32",
            "description": "Spacegroup number in International Tables"
        },
        {
            "name": "total_energy",
            "type": "double",
            "unit": "eV",
            "description": "Total energy of the system"
        },
        {
            "name": "masses",
            "type": "double",
            "unit": "u",
            "dims": [
                "N"
            ],
            "description": "Mass of each atom"
        }
    ]
}
"""

# Create new Structure entity based on the metadata
Structure = softpy.entity(metadata)

# Create a empty Structure instance
s = Structure()
s.structure = 'water'
s.symbols = ['H', 'H', 'O']
s.spacegroup = 1
s.total_energy = 446.32
s.masses = np.array([1., 1., 16.])

s.soft_internal_check_dimension_sizes()

# Write the structure to file
with softpy.Storage('hdf5', 'softpy-test-factory.h5') as storage:
    storage.save(s)


# Create new Structure instance initiated from file
s2 = Structure(uuid=s.soft_get_id(), driver='hdf5',
               uri='softpy-test-factory.h5')


# Check that the entities are equals
for name in 'id', 'meta_name', 'meta_version', 'meta_namespace':
    f = getattr(softpy, 'get_' + name)
    assert f(s) == f(s2)
    
assert s.soft_get_property_names() == s2.soft_get_property_names()

for name in s.soft_get_property_names():
    v = getattr(s, name)
    v2 = getattr(s2, name)
    if isinstance(v, np.ndarray):
        assert v.all() == v2.all()
    else:
        assert v == v2


Structure3 = softpy.entity(metadata)
s3 = Structure3()
s3.structure = 'water'
s3.symbols = ['H', 'H', 'O']
s3.spacegroup = 1
s3.total_energy = 446.32
s3.masses = np.array([1., 1., 16.])

s4 = Structure3(uuid=s.soft_get_id(), driver='hdf5',
                uri='softpy-test-factory.h5')


Person = softpy.entity(open(os.path.join(thisdir, 'person.json')))
jack = Person(name='Jack Daniels', age=42, skills=['tasting', 'Python', 'C'])
