import socket, time, re

HOST = "127.0.0.1"
PORT = 4444

s = socket.create_connection((HOST, PORT))
s.settimeout(0.2)

def send(cmd):
    s.sendall((cmd + "\n").encode())

buf = b""
unlocked = False

for i in range(50000):
    send("AUTH a")
    # read whatever is available
    try:
        data = s.recv(4096)
        if data:
            buf += data
            if b"Mu'adhin mode enabled" in buf:
                unlocked = True
                break
    except socket.timeout:
        pass

if not unlocked:
    print("Did not unlock, try again / increase loops")
    exit()

send("IFTAR")
time.sleep(0.05)
try:
    print(s.recv(4096).decode(errors="ignore"))
except socket.timeout:
    pass
