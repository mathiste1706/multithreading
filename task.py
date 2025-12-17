import time
import json
import numpy as np


class Task:
    def __init__(self, identifier=0, size=None):
        self.identifier = identifier
        # choosee the size of the problem
        self.size = size or np.random.randint(300, 3_000)
        # Generate the input of the problem
        self.a = np.random.rand(self.size, self.size)
        self.b = np.random.rand(self.size)
        # prepare room for the results
        self.x = np.zeros((self.size))
        self.time = 0

    def work(self):
        start = time.perf_counter()
        self.x = np.linalg.solve(self.a, self.b)
        self.time = time.perf_counter() - start

    def to_json(self) -> str:
        return json.dumps(
            {
                "identifier": self.identifier,
                "size": self.size,
                "time": self.time,
                "a": self.a.tolist(),
                "b": self.b.tolist(),
                "x": self.x.tolist(),
            }
        )

    @staticmethod
    def from_json(text: str) -> "Task":
        data = json.loads(text)

        task = Task(
            identifier=data["identifier"],
            size=data["size"],
        )

        task.time = data["time"]
        task.a = np.array(data["a"])
        task.b = np.array(data["b"])
        task.x = np.array(data["x"])

        return task

    def __eq__(self, other: "Task") -> bool:
        if self.identifier != other.identifier:
            return False
        if self.size != other.size:
            return False

        if self.a != other.a:
            return False

        if self.b != other.b:
            return False

        if self.x != other.x:
            return False

        if self.x != other.x:
            return False

        if self.time != other.time:
            return False

        return True
