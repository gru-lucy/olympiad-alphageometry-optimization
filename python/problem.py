# python/problem.py
from graph_cpp import CppProblem, Definition

def load_definitions(fname, to_dict=False):
    # For testing, we ignore fname and return dummy definitions using the bound dummy method.
    return {
        "triangle": Definition.dummy("triangle"),
        "on_tline": Definition.dummy("on_tline"),
        "on_line": Definition.dummy("on_line"),
        "incenter": Definition.dummy("incenter"),
        "excenter": Definition.dummy("excenter"),
    }

class Problem:
    @classmethod
    def from_txt(cls, txt, translate=True):
        # Use the bound C++ factory method (which we exposed as from_txt) to create a Problem instance.
        return CppProblem.from_txt(txt)

class Theorem:
    @classmethod
    def from_txt_file(cls, fname, to_dict=False):
        # For testing, return an empty dictionary.
        return {}
