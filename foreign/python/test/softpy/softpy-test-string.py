#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import softpy

s = softpy.string_create('Hello world!')
assert s == 'Hello world!'

#softpy.string_assign(s, 'A new value...')
#assert softpy.from_softc_string(s) == 'A new value...'
#
#softpy.string_destroy(s)
