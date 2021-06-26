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

GREEN = "\033[0;32m"
RED = "\033[0;31m"
WHITE = "\033[0;37m"
RESET = "\033[0;0m"

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

def generate_report(test_name, expected, results, elapsed):
    test_string = f"Test: {repr(test_name)}"
    if (results == expected):
        status = f"{GREEN}OK!{RESET}"
    else:
        status= f"{RED}KO!{RESET}"
    diff = f"Received: {repr(results)}\nExpected: {repr(expected)}\n"
    runtime = f"{WHITE}Runtime: {elapsed} [{len(results)/elapsed} c/s]{RESET}\n\n"

    normal = f"{status}\t{test_string}\n"
    medium = normal + runtime
    verbose = normal + diff + runtime
    return (normal, medium, verbose)

def test_one_server_one_client(msg="test"):
    start = datetime.datetime.now().timestamp()
    server_id_event = threading.Event()
    server = threading.Thread(target=start_server, args=(server_id_event,))
    server.start()
    server_id_event.wait()

    if SERVER_ID:
        client = threading.Thread(target=start_client, args=(msg,))
        client.start()
        client.join()
    os.kill(SERVER_ID, signal.SIGINT)
    server.join()

    elapsed = datetime.datetime.now().timestamp() - start
    contents = ""
    with open(TEMP_LOG, "r") as temp_log:
        contents = temp_log.read()
        expected = str(SERVER_ID) + "\n" + msg
    if (contents != expected):
        ret = -1
    else:
        ret = 0
    if os.path.exists(TEMP_LOG):
        os.remove(TEMP_LOG)
    normal, medium, verbose = generate_report(msg, expected, contents, elapsed)
    with open(LOGFILE, "a") as logfile:
        logfile.write(verbose)
    print(medium, end="")
    return ret

def test_one_server_two_clients(msg="test"):
    start = datetime.datetime.now().timestamp()
    server_id_event = threading.Event()
    server = threading.Thread(target=start_server, args=(server_id_event,))
    server.start()
    server_id_event.wait()

    if SERVER_ID:
        expected = str(SERVER_ID) + "\n"
        client = threading.Thread(target=start_client, args=(msg + "(#1)",))
        client.start()
        client.join()
        expected += msg + "(#1)"
        
        client = threading.Thread(target=start_client, args=(msg + "(#2)",))
        client.start()
        client.join()
        expected += msg + "(#2)"

    os.kill(SERVER_ID, signal.SIGINT)
    server.join()
    elapsed = datetime.datetime.now().timestamp() - start
    with open(TEMP_LOG, "r") as temp_log:
        contents = temp_log.read()
     #   expected = str(SERVER_ID) + "\n" + msg
        if (contents != expected):
            ret = -1
        else:
            ret = 0
    normal, medium, verbose = generate_report(msg, expected, contents, elapsed)
    print(medium, end="")
    with open(LOGFILE, "a") as logfile:
        logfile.write(verbose)
    if os.path.exists(TEMP_LOG):
        os.remove(TEMP_LOG)
    return ret

if __name__ == "__main__":
    if not os.path.isdir("./logs"):
        os.mkdir("logs")
    if os.path.exists(TEMP_LOG):
        os.remove(TEMP_LOG)

    signal.signal(signal.SIGUSR1, signal.SIG_IGN)
   
    total = 0
    err = 0

    total += 1
    err += test_one_server_one_client("hello\n")
    total += 1
    err += test_one_server_one_client()
    total += 1
    err += test_one_server_one_client("\n")
    total += 1
    err += test_one_server_two_clients("hello ")

    letters = string.ascii_letters + string.digits + string.punctuation
 #   test_string = ''.join(random.choice(letters) for i in range(100))
  #  total += 1
   # err += test_one_server_one_client(test_string)
    
  #  test_string = ''.join(random.choice(letters) for i in range(500))
  #  total += 1
  #  err += test_one_server_one_client(test_string)

    print(f"Passed {total + err}/{total}\n")
