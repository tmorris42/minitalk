import os
import signal
import subprocess
import threading

SERVER_ID = None

def start_server(server_id_event):
    global SERVER_ID
    with open("logfile2", "a") as logfile:
        global SERVER_ID
        proc = subprocess.Popen("./server", stdout=logfile, stderr=None, text=True,
                bufsize=1, close_fds=True)
        SERVER_ID = proc.pid
        server_id_event.set()
        proc.wait()

def start_client():
    output = subprocess.run(["./client", f"{SERVER_ID}", "test"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    print(output)

if __name__ == "__main__":
    server_id_event = threading.Event()

    server = threading.Thread(target=start_server, args=(server_id_event,))
    server.start()

    server_id_event.wait()

    if SERVER_ID:
        print(SERVER_ID)

        client = threading.Thread(target=start_client)
        client.start()

        client.join()

    os.kill(SERVER_ID, signal.SIGINT)
    server.join()
