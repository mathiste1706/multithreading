from manager import QueueClient
import time

def main():
    client = QueueClient()
    print("[MINION] Connected to manager.")

    while True:
        try:
            task = client.task_queue.get()
            print(f"[MINION] Working on task {task.identifier} (size={task.size})")

            task.work()
            print(f"[MINION] Finished task {task.identifier} in {task.time:.3e}s")
            
            client.result_queue.put(task)

        except Exception as e:
            print(f"[MINION] Error: {e}")
            time.sleep(1)  # prevent busy loop

if __name__ == "__main__":
    main()
