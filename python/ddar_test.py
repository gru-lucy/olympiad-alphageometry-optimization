# python/ddar_test.py
from absl.testing import absltest
import graph_cpp  # our pybind11 module
import problem    # our minimal problem module

class DDARTest(absltest.TestCase):

    @classmethod
    def setUpClass(cls):
        # Use our helper function to load definitions.
        cls.defs = problem.load_definitions('defs.txt', to_dict=True)
        cls.rules = problem.Theorem.from_txt_file('rules.txt', to_dict=True)

    def test_orthocenter_should_fail(self):
        txt = 'a b c = triangle a b c; d = on_tline d b a c, on_tline d c a b ? perp a d b c'
        p = problem.Problem.from_txt(txt)
        g, _ = graph_cpp.Graph().build_problem(p, self.defs, False)
        # Use self.defs (not self.rules) as the second argument.
        g, level_times, status, branching, all_added = graph_cpp.DDAR.solve(
            g, self.defs, p, max_level=1000, timeout=600)
        goal_args = g.names2nodes(p.goal.args)
        self.assertFalse(g.check(p.goal.name, goal_args))

    def test_orthocenter_aux_should_succeed(self):
        txt = ('a b c = triangle a b c; '
               'd = on_tline d b a c, on_tline d c a b; '
               'e = on_line e a c, on_line e b d ? perp a d b c')
        p = problem.Problem.from_txt(txt)
        g, _ = graph_cpp.Graph().build_problem(p, self.defs, False)
        g, level_times, status, branching, all_added = graph_cpp.DDAR.solve(
            g, self.defs, p, max_level=1000, timeout=600)
        goal_args = g.names2nodes(p.goal.args)
        self.assertTrue(g.check(p.goal.name, goal_args))

    def test_incenter_excenter_should_succeed(self):
        txt = 'a b c = triangle a b c; d = incenter d a b c; e = excenter e a b c ? perp d c c e'
        p = problem.Problem.from_txt(txt)
        g, _ = graph_cpp.Graph().build_problem(p, self.defs, False)
        g, level_times, status, branching, all_added = graph_cpp.DDAR.solve(
            g, self.defs, p, max_level=1000, timeout=600)
        goal_args = g.names2nodes(p.goal.args)
        self.assertTrue(g.check(p.goal.name, goal_args))


if __name__ == '__main__':
    absltest.main()
