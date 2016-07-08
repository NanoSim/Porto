#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Example demonstrating how to SOFTify an Atoms object from ASE
(https://wiki.fysik.dtu.dk/ase/).

In this example we do the needed modifications to a subclass, but they
could equally well be applied to the original class.
"""
from __future__ import print_function

import warnings
import pickle
import ast
import base64

import numpy as np
import ase
import ase.constraints
from ase.calculators.emt import EMT
from ase.lattice.spacegroup import Spacegroup, crystal
from ase.visualize import view
import softpy
import dill


BaseSoftAtoms = softpy.entity(open('atoms.json'))


class SoftAtoms(BaseSoftAtoms, ase.Atoms):
    """An ase.Atoms subclass behaving as a SOFT entity.

    Takes the same arguments as ase.Atoms with the following additions:

    symbols - same meaning as in Atoms, except that it also can be an
              existing Atoms object
    uuid    - optional, id of instance to load from driver
    driver  - optional, driver to use for loading data: "json", "hdf5", "mongo"
    uri     - optional, from where to load data
    options - optional, additional options passed to the driver
    """
    def __new__(cls, symbols=None, uuid=None, driver=None, uri=None,
                options=None, **kwargs):
        if isinstance(symbols, ase.Atoms):
            atoms = symbols.copy()
            # Note: calculator is not copied!  (bug in ASE?)
            atoms.set_calculator(symbols.get_calculator())  
        elif driver:
            b = BaseSoftAtoms(
                uuid=uuid, driver=driver, uri=uri, options=options)
            atoms = ase.Atoms(positions=b.positions, numbers=b.numbers,
                              **kwargs)
        else:
            atoms = ase.Atoms(symbols, **kwargs)
        atoms.__class__ = cls
        return atoms
    
    def __init__(self, symbols=None, uuid=None, driver=None, uri=None,
                 options=None, **kwargs):
        super(SoftAtoms, self).__init__(uuid=uuid, driver=driver,
                                        uri=uri, options=options,
                                        uninitialize=False)

    def get_info(self):
        """Returns the `info` attribute as a list of "key=value" strings."""
        # wraps info to a SOFT string_list
        return ['%s=%r' % (k, v) for k, v in self.info.items()]

    def set_info(self, value):
        """Sets `info` from a list of "key=value" strings."""
        # unwraps info from a SOFT string_list
        for val in value:
            k, v = val.split('=', 1)
            self.info[k] = _unwrap(v, functions=(Spacegroup, ))

    def get_adsorbate_info(self):
        """Returns the `adsorbate_info` attribute as a list of "key=value"
        strings."""
        # wraps assorbate_info to a SOFT string_list (incl. workaround for
        # empty string_list)
        if self.adsorbate_info:
            return ['%s=%r' % (k, v) for k, v in self.adsorbate_info.items()]
        else:
            return ['']

    def set_adsorbate_info(self, value):
        """Sets `adsorbate_info` from a list of "key=value" strings."""
        # unwraps assorbate_info from a SOFT string_list
        if value and value[0]:
            for val in value:
                k, v = val.split('=', 1)
                self.adsorbate_info[k] = _unwrap(v, functions=(np.array, ))
                    
    def get_constraints(self):
        """Returns constraints as a list of strings."""
        return [
            repr(c) for c in self.constraints] if self.constraints else ['']

    def set_constraints(self, value):
        """Sets constraints from a list of strings."""
        if value and value[0]:
            for c in value:
                self.set_constraint(_unwrap(c, functions=(
                    ase.constraints.FixAtoms,
                    ase.constraints.FixBondLength,
                    ase.constraints.FixBondLengths,
                    ase.constraints.FixCartesian,
                    ase.constraints.FixedLine,
                    ase.constraints.FixedMode,
                    ase.constraints.FixedPlane,
                    ase.constraints.FixInternals,
                    ase.constraints.FixScaled,
                    ase.constraints.Hookean,
                )))

    def get_calculator_pickle(self):
        s = pickle.dumps(self.get_calculator())
        return softpy.asStr(base64.b64encode(s))

    def set_calculator_pickle(self, value):
        s = softpy.asBytes(base64.b64decode(value))
        self.set_calculator(pickle.loads(s))

    def get_numbers(self):
        # needed because SOFT is missing int64 array
        return super(SoftAtoms, self)._get_atomic_numbers().astype('int32')

    def get_tags(self):
        # needed because SOFT is missing int64 array
        return super(SoftAtoms, self).get_tags().astype('int32')

    def get_celldisp(self):
        # needed because Atoms.get_celldisp() returns an array of shape
        # (1, 3) - probably a bug in ASE
        return super(SoftAtoms, self).get_celldisp().squeeze()


def _unwrap(s, constants=None, functions=()):
    """Returns unwrapped object from its string representation `s`."""
    try:
        return softpy.arithmetic_eval(s, constants, functions)
    except softpy.ArithmeticError:
        try:
            return ast.literal_eval(s)
        except ValueError:
            warnings.warn('converted to string: %r' % s)
    return s


    
a = 4.6
c = 2.95
rutile = crystal(['Ti', 'O'], basis=[(0, 0, 0), (0.3, 0.3, 0.0)],
                 spacegroup=136, cellpar=[a, a, c, 90, 90, 90])
rutile.set_constraint(ase.constraints.FixAtoms(indices=(1, 3)))


r = SoftAtoms(rutile)

s = softpy.Storage(driver='hdf5', uri='atoms.h5')
s.save(r)
s.close()

rr = SoftAtoms(driver='hdf5', uri='atoms.h5', uuid=r.soft_get_id())
rr.write('rutile.cif')

