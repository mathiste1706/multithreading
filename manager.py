from multiprocessing.managers import BaseManager
from queue import Queue

# Shared queues
task_queue = Queue()
result_queue = Queue()


# QueueClient exposes proxies to remote processes
class QueueClient:
    task_queue = None
    result_queue = None

    def __init__(self):
        class QueueManager(BaseManager):
            pass

        # Register queues
        QueueManager.register("task_queue")
        QueueManager.register("result_queue")

        # Connect to the manager (manager itself must be running)
        manager = QueueManager(address=("localhost", 50000), authkey=b"abc")
        manager.connect()

        # These are proxies; all processes see the same queue
        self.task_queue = manager.task_queue()
        self.result_queue = manager.result_queue()


# Manager server
class QueueManagerServer(BaseManager):
    pass


QueueManagerServer.register("task_queue", callable=lambda: task_queue)
QueueManagerServer.register("result_queue", callable=lambda: result_queue)


def run_manager():
    manager = QueueManagerServer(address=("", 50000), authkey=b"abc")
    server = manager.get_server()
    print("Manager running on port 50000...")
    server.serve_forever()


if __name__ == "__main__":
    run_manager()
