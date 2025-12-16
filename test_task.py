import unittest
from numpy.testing import assert_allclose
from task import Task


class TestTask(unittest.TestCase):
    def test_Ax_equals_B(self):
        task = Task(1, 10)

        Task.work(task)

        # Vérification numérique avec tolérance
        assert_allclose(task.a @ task.x, task.b, rtol=1e-7, atol=1e-9)

    def test_a_equals_b(self):
        a = Task()
        json_dump = a.to_json()
        Task.from_json(json_dump)


if __name__ == "__main__":
    unittest.main()
