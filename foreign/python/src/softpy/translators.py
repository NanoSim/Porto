# -*- coding: utf-8 -*-
"""Translators are used to translate an instance of one entity to an
instance of another entity.

Translators can be registered as plugins and invoked transparently by
SOFT.  For this to work, softpy search the current directory and all
directories listed in the module `path` variable for *.pyt files.

When this module is imported, the plugin search path is initialised
from the environment variable SOFT_TRANSLATORS.  Like SOFTSTORAGE,
this search path is colon-separated.

A plugin *.pyt file should provide one or more sub-classes of Translator.
For details, see the docstring for Translator.  When a
"""
from __future__ import absolute_import
from __future__ import print_function

import os
import sys
import glob
import inspect
if sys.version_info < (3, 4):
    import imp
else:
    import importlib.machinery
    import importlib.util

from . import softpy
from .errors import SoftError

# FIXME: should be implemented in C++?



class SoftTranslatorError(SoftError):
    """Raised for translation error."""
    pass

class SoftMissingTranslatorError(SoftTranslatorError):
    """Raised when a translator cannot be found."""
    pass


# Search paths for plugin *.pyt files
path = [os.getcwd()]

def _parse_pathstring(paths):
    """Adds `paths` to the module path variable."""
    s = set(path)
    for p in paths.split(':'):
        if p and p not in s:
            path.append(p)

# Initialize path from environment
_parse_pathstring(os.environ.get('SOFT_TRANSLATORS', ''))


class Translator(object):
    """A base class that translator plugins should inherit from.

    The plugin subclass should:

      - Overwrite the translate() method.

      - Set the class attribute `inputs_entities` to a sequence of
        entity types that the translate() method should get as input.
        These should be given as (name, version, namespace)-tuples.

      - Set the class attribute `output_entities` to a sequence of
        entity types that the translate() method will return.
        These should be given as (name, version, namespace)-tuples.

    Note that this class is not intented to be instansiated, we just
    use the class to keep the definitions of input and output types
    together with the translate() method.  This makes it easy to
    define several translators in a single file.
    """
    input_entities = NotImplemented
    output_entities = NotImplemented

    @staticmethod
    def translate(input_instances):
        """Translates `input_instances` to a sequence of output instances.

        In the common case of a single output instance, it may be returned
        by itself.

        This method should be overwritten in the plugin subclass.
        """
        raise NotImplementedError

    # List of registered translators
    _translators = []  # (translator, inputs, outputs)-tuples
    _visited_paths = set()  # lists visited paths, for efficiency
    _plugins = []  # references to loaded plugin name spaces, to avoid
                   # that they are garbage collected

    @classmethod
    def get_translators(cls):
        """Returns a list of (translator, inputs, outputs)-tuples."""
        for p in path:
            if p not in cls._visited_paths:
                for pytfile in glob.glob(os.path.join(p, '*.pyt')):
                    cls.load_plugin(pytfile)
                cls._visited_paths.add(p)
        return cls._translators

    @classmethod
    def load_plugin(cls, filename):
        name = 'soft_translator_plugin%d' % len(cls._plugins)
        if sys.version_info < (3, 4):
            with open(filename, 'U') as f:
                mod = imp.load_module(name, f, filename,
                                      ('.py', 'U', imp.PY_SOURCE))
        else:
            loader = importlib.machinery.SourceFileLoader(name, filename)
            spec = importlib.util.spec_from_loader(loader.name, loader)
            mod = importlib.util.module_from_spec(spec)
            loader.exec_module(mod)
        cls._plugins.append(mod)

        for t in mod.__dict__.values():
            if (inspect.isclass(t) and
                issubclass(t, Translator) and
                t is not Translator):
                cls._translators.append(
                    (t.translate, t.input_entities, t.output_entities))



def register_translator(translator, inputs, outputs):
    """Registers a translator.

    Parameters
    ----------
    translator : callable
        A translator. It should take a sequence of input instances as
        argument and should return a sequence of output instances.
    inputs : sequence
        A sequence (name, version, namespace) tuples describing the
        entities the translator takes as input.
    outputs : sequence
        A sequence (name, version, namespace) tuples describing the
        entities the translator returns as output.
    """
    Translator._translators.append((translator, inputs, outputs))


def _translation_tree(output, inputs):
    """Returns a nested list structure describing how the entities in
    `inputs` can be translated to `output`.

    `output` is a (name, version, namespace)-tuple and `inputs` is a
    sequence of (name, version, namespace)-tuples.

    The returned value is either an element in `inputs` (if it matches
    `output`) or a nested tuple of the form

        (output, translator, [input, ...])

    where `input` is either an element in `inputs` or a tuple like above.

    Raises SoftMissingTranslatorError if none of the installed
    translators can translate `inputs` to `output`.
    """
    if output in inputs:
        return output
    for trans, ins, outs in Translator.get_translators():
        if output in outs:
            try:
                return (output, trans, [_translation_tree(inp, inputs)
                                       for inp in ins])
            except SoftMissingTranslatorError:
                pass
    raise SoftMissingTranslatorError('Cannot translate to %s' % (output, ))


def translate(output, input_instances):
    """Returns a new instance of type `output` from `input_instances`.

    Args:
        output: A (name, version, namespace)-tuple specifying the desired
            type.
        input_instances: Sequence of input entity instances.

    Returns:
        New instance of the desired type populated from `input_instances`.

    Raises:
        SoftMissingTranslatorError: If none of the installed
            translators can translate `input_instances` to an instance
            of the desired type.
    """
    if hasattr(input_instances, 'soft_get_id'):
        instances = [input_instances]
    inputdict = get_instancedict(input_instances)
    if len(inputdict) != len(input_instances):
        raise SoftTranslatorError(
            'Translating from several instances of the same type is '
            'ambiguous')

    def _translate(tree):
        if isinstance(tree[2], list):
            output, trans, inputs = tree
            assert output not in inputdict
            insts = trans([_translate(t) for t in inputs])
            d = get_instancedict(insts)
            inputdict[output] = d[output]
            return d[output]
        else:
            return inputdict[tree]

    tree = _translation_tree(output, inputdict.keys())
    return _translate(tree)


def get_instancedict(instances):
    """Returns a dict mapping (name, version, namespace) of the metadata
    of each element in `instances` to the instances themselves.
    `instances` may also be a single instance."""
    if hasattr(instances, 'soft_get_id'):
        instances = [instances]
    return {softpy.get_meta_mtype(inst): inst for inst in instances}
