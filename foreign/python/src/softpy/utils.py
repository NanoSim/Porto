# -*- coding: utf-8 -*-
"""
Some generic utility functions.
"""
from __future__ import absolute_import

import json

import numpy as np



class JSONSoftEncoder(json.JSONEncoder):
    """A subclass of JSONEncoder that handles all soft types."""
    def default(self, obj):
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        return json.JSONEncoder(self, obj)


#def json_soft_obj_hook(obj):
#    """Decodes a previously encoded soft data."""
#    return obj


def json_loads(s, **kw):
    """Soft wrapper around json.loads()."""
    #return json.loads(s, object_hook=json_soft_obj_hook, **kw)
    return json.loads(s, **kw)

def json_dumps(obj, **kw):
    """Soft wrapper around json.load()."""
    return json.dumps(obj, cls=JSONSoftEncoder, **kw)


def json_load(fp, **kw):
    """Soft wrapper around json.loads()."""
    #return json.load(fp, object_hook=json_soft_obj_hook, **kw)
    return json.load(fp, **kw)

def json_dump(obj, fp, **kw):
    """Soft wrapper around json.load()."""
    return json.dump(obj, fp, cls=JSONSoftEncoder, **kw)
