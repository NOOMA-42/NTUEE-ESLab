import socket
import time

HOST_ENV = {"dev": "192.168.0.11", "local_test": "127.0.0.1"}
HOST = HOST_ENV["local-test"] # Standard loopback interface address
PORT = 65431
clientMessage = "{\"x\":2, \"y\":5, \"z\":5, \"s\":10}"
x_val = 10
s_val = 0

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))
while(True):
    x_val = x_val + 0.1
    s_val = s_val + 1
    clientMessage = "{\"x\":" + str(x_val) + ", \"y\":10, \"z\":10, \"s\":" + str(s_val) + "}"
    client.sendall(clientMessage.encode())
    time.sleep(1)

serverMessage = str(client.recv(1024), encoding='utf-8')
print('Server:', serverMessage)

client.close()