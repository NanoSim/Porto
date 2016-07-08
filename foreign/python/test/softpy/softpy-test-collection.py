#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os

import softpy


thisdir = os.path.dirname(__file__)


Person = softpy.entity(open(os.path.join(thisdir, 'person.json')))

jack = Person()


coll = softpy.Collection()
assert len(coll) == 0
coll['jack'] = jack
#assert len(coll) == 1
