import os
import random
import signal
import string
import subprocess
import threading
import datetime

SERVER_ID = None
LOGFILE = datetime.datetime.now().strftime("./logs/%Y-%m-%d-%H-%M-%S.log")
TEMP_LOG = "./logs/temp.log"

def start_server(server_id_event):
    global SERVER_ID
    with open(TEMP_LOG, "a") as logfile:
        global SERVER_ID
        proc = subprocess.Popen("./server", stdout=logfile, stderr=None, text=True,
                bufsize=1, close_fds=True)
        SERVER_ID = proc.pid
        server_id_event.set()
        proc.wait()

def start_client(msg="test"):
    output = subprocess.run(["./client", str(SERVER_ID), str(msg)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    #print(output)

def test_one_server_one_client(msg="test"):
    start = datetime.datetime.now().timestamp()
    to_print = f'Testing "{msg}"\n'
    server_id_event = threading.Event()

    server = threading.Thread(target=start_server, args=(server_id_event,))
    server.start()

    server_id_event.wait()

    if SERVER_ID:
        print(SERVER_ID)

        client = threading.Thread(target=start_client, args=(msg,))
        client.start()

        client.join()

    os.kill(SERVER_ID, signal.SIGINT)
    server.join()
    elapsed = datetime.datetime.now().timestamp() - start
    with open(TEMP_LOG, "r") as temp_log:
        contents = temp_log.read()
        if (contents != msg):
            to_print += f"ERROR!\nReceived: {contents}\nExpected: {msg}\n"
        else:
            to_print += "OK!"
        to_print += f"\nRuntime: {elapsed} [{len(msg)/elapsed} c/s]"
        to_print += '\n\n'
        with open(LOGFILE, "a") as logfile:
            print(to_print);
            logfile.write(to_print)
    if os.path.exists(TEMP_LOG):
        os.remove(TEMP_LOG)


if __name__ == "__main__":
    if not os.path.isdir("./logs"):
        os.mkdir("logs")
    if os.path.exists(TEMP_LOG):
        os.remove(TEMP_LOG)

    signal.signal(signal.SIGUSR1, signal.SIG_IGN)
   
    letters = string.ascii_letters + string.digits + string.punctuation
    test_string = ''.join(random.choice(letters) for i in range(100))
    
    test_one_server_one_client("hello\n")
    test_one_server_one_client(test_string)
    test_one_server_one_client()
