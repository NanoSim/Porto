#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

import os

import softpy


thisdir = os.path.dirname(__file__)

A = softpy.load_entity(os.path.join(thisdir, 'A.json'))
B = softpy.load_entity(os.path.join(thisdir, 'B.json'))
C = softpy.load_entity(os.path.join(thisdir, 'C.json'))
D = softpy.load_entity(os.path.join(thisdir, 'D.json'))
E = softpy.load_entity(os.path.join(thisdir, 'E.json'))
ma = A.soft_metadata.mtype
mb = B.soft_metadata.mtype
mc = C.soft_metadata.mtype
md = D.soft_metadata.mtype
me = E.soft_metadata.mtype

#
# define translations according to the following graph:
#
#   A<--(B,C)
#   ^      ^
#    \     |
#     E    D
#
def transD2C(d):
    """Translates entities of type D to C."""
    dt = softpy.get_instancedict(d)
    return C(s=dt[md].s)

#def transE2A(e):
#    """Translates entities of type E to A."""
#    dt = softpy.get_instancedict(e)
#    return A(s=dt[me].s, length=100.0 * dt[me].length)
#
def transBC2A(bc):
    """Translates entities of type B, C to A."""
    dt = softpy.get_instancedict(bc)
    return A(s=dt[mc].s, length=100.0 * dt[mb].length)

softpy.register_translator(transD2C, [md], [mc])
#softpy.register_translator(transE2A, [me], [ma])
softpy.register_translator(transBC2A, [mb, mc], [ma])

b = B(length=1.2)
c = C(s='c')
d = D(s='d', length=1.4)
e = E(s='e', length=1.6)

a = softpy.translate(ma, [b, c])
assert a.s == 'c'
assert a.length == 120.0

a = softpy.translate(ma, [b, d])
assert a.s == 'd'
assert a.length == 120.0

a = softpy.translate(ma, [e])
assert a.s == 'e'
assert a.length == 160.0

a2 = softpy.translate(ma, [a])
assert a2.s == a.s
assert a2.length == a.length
