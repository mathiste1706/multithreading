from multiprocessing.managers import BaseManager
from task import Task


class QueueManager(BaseManager):
    pass


QueueManager.register("task_queue")
QueueManager.register("result_queue")

manager = QueueManager(address=("localhost", 50000), authkey=b"abc")
manager.connect()

task_queue = manager.task_queue()
result_queue = manager.result_queue()

# Enqueue tasks
for i in range(3):
    t = Task(identifier=i, size=6000)
    print(f"[BOSS] Enqueue task {i}")
    task_queue.put(t)

# Collect results
for i in range(3):
    result = result_queue.get()
    print(f"[BOSS] Got result {result.identifier} time={result.time:.3f}s")
