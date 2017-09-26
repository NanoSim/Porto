# -*- mode: python -*-
# -*- coding: utf-8 -*-
"""Sample plugin."""
import softpy
from softpy import Translator

E_mtype = 'E', '0.1', 'http://sintef.no/meta/soft'
A_mtype = 'A', '0.1', 'http://sintef.no/meta/soft'


class TransE2A(Translator):
    """Translator plugin for translating instances of type E to A."""

    input_entities = [E_mtype]
    output_entities = [A_mtype]

    @staticmethod
    def translate(instances):
        """Translates instances of type E to A."""
        A = softpy.entity(A_mtype)
        dt = softpy.get_instancedict(instances)
        return A(s=dt[E_mtype].s, length=100.0 * dt[E_mtype].length)
