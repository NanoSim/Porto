# -*- coding: utf-8 -*-
"""
Exceptions common to the softpy modules.
"""

class SoftError(Exception):
    """Base class for SOFT exceptions."""
    pass

class SoftMetadataError(SoftError):
    """Raised on malformed metadata description."""
    pass
