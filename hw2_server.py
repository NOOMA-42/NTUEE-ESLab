#!/usr/bin/env python3
import socket
import numpy as np
import json
import time
import random
import ast # string to dict
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
from matplotlib.ticker import FuncFormatter
from multiprocessing import Process, Queue

def Animate(i):  
    new_data = q.get()
    data.append(new_data['x'])
    line.set_ydata(data)
    return line,

def init():
    line.set_ydata([np.nan] * len(x))
    return line,

def SensorDataParser(q):
    # Socket config
    HOST_ENV = {"dev": "192.168.0.11", "local-test": "127.0.0.1"}
    HOST = HOST_ENV["dev"] # Standard loopback interface address
    PORT = 65431 # Port to listen on (use ports > 1023)

    # Run server and data renderer
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen() # why 5: https://ask.csdn.net/questions/684321
        conn, addr = s.accept()

        with conn:
            print('Connected by', addr)
            while True:
                sensor_data = conn.recv(1024).decode('utf8')
                if not sensor_data: # client disconnects
                    break
                print('Received from socket server : ', sensor_data)
                    
                # print received data
                try:
                    sensor_data = ast.literal_eval(sensor_data)
                except:
                    print("Fail to parse data")
                
                # Add next value
                q.put(sensor_data)
            
            conn.close()
            print('client closed connection')

if __name__ == '__main__':
    # Socket as child process asymchronously catching data
    q = Queue()
    p = Process(target=SensorDataParser, args=(q,))
    p.start()

    # Matpolib config
    max_sample_num = 10
    x = np.arange(0, max_sample_num)
    fig, ax = plt.subplots()
    ax.set_ylim(-300, 300)
    ax.set_xlim(0, max_sample_num)
    ax.xaxis.set_major_formatter(FuncFormatter(lambda x, pos: '{:.0f}s'.format(max_sample_num - x - 1))) #axis format
    plt.xlabel('Seconds ago')
    line, = ax.plot(np.arange(max_sample_num), np.zeros(max_sample_num))
    ax.plot(np.arange(max_sample_num), np.zeros(max_sample_num))
    data = deque(np.zeros(max_sample_num), maxlen=max_sample_num)  # hold the last 10 values
    
    # List of Animation objects for tracking
    ani = animation.FuncAnimation(fig, Animate, init_func=init, interval=50, blit=True, save_count=10)
    plt.show()

    p.join() # block and wait for child process end
    print("server end. close")





