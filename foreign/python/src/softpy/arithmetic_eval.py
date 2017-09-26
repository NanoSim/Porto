# -*- coding: utf-8 -*-
"""
Module implementing safe evaluation of arithmetic expression.
"""
import ast
import operator


class ArithmeticError(Exception):
    """Raised by arithmetic_eval() on errors in the evaluated expression."""
    pass


def arithmetic_eval(expr, constants=None, functions=()):
    """Returns the result of arithmetic evaluation of `expr`.  This function
    is much safer to use than eval() and should be preferred for simple
    arithmetic evaluation.

    If given, `constants` should be a dict mapping constant names to values,
    e.g. ``{'pi': math.pi, 'e': math.e}``.

    The `functions` argument should be a sequence of available functions,
    e.g. ``(abs, min, max, math.sin)``.

    This implementation is inspired by
    http://bingbots.com/questions/148864/valueerror-malformed-string-when-using-ast-literal-eval
    """
    binOps = {
        ast.Add: operator.add,
        ast.Sub: operator.sub,
        ast.Mult: operator.mul,
        ast.Div: operator.truediv,
        ast.Mod: operator.mod,
        ast.Pow: operator.pow,
    }
    consts = constants if constants else {}
    funcs = {f.__name__: f for f in functions}
    node = ast.parse(expr, mode='eval')

    def _eval(node):
        if isinstance(node, ast.Expression):
            return _eval(node.body)
        elif isinstance(node, ast.Str):
            return node.s
        elif isinstance(node, ast.Num):
            return node.n
        elif isinstance(node, ast.BinOp):
            return binOps[type(node.op)](_eval(node.left), _eval(node.right))
        elif isinstance(node, ast.Call):  # function call
            if not node.func.id in funcs:
                raise ArithmeticError(
                    'Invalid function %s() in expr: %r' % (node.func.id, expr))
            func = funcs[node.func.id]
            args = tuple(_eval(arg) for arg in node.args)
            keywords = {kw.arg: _eval(kw.value) for kw in node.keywords}
            return func(*args, **keywords)
        elif isinstance(node, ast.Name):  # constant
            if not node.id in consts:
                raise ArithmeticError(
                    'Invalid constant %r in expr: %r' % (node.id, expr))
            return consts[node.id]
        # -- indexing...
        elif isinstance(node, ast.Subscript):
            return _eval(node.value)[_eval(node.slice)]
        elif isinstance(node, ast.Index):
            return _eval(node.value)
        elif isinstance(node, ast.Slice):
            lower = _eval(node.lower) if node.lower else None
            upper = _eval(node.upper) if node.upper else None
            step = _eval(node.step) if node.step else None
            return slice(lower, upper, step)
        # -- some Python containers, not really arithmetic, but useful...
        elif isinstance(node, ast.List):
            return [_eval(v) for v in node.elts]
        elif isinstance(node, ast.Tuple):
            return tuple(_eval(v) for v in node.elts)
        elif isinstance(node, ast.Set):
            return {_eval(v) for v in node.elts}
        elif isinstance(node, ast.Dict):
            return {_eval(k): _eval(v) for k, v in zip(node.keys, node.values)}
        else:
            raise ArithmeticError(
                'Unsupported type %s in expr: %r' % (node, expr))

    return _eval(node.body)
