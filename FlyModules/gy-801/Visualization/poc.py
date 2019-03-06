#   #!/usr/bin/python
import socket
import time
import struct
import math

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.settimeout(0.01)

fig, ax = plt.subplots()
xdata, ydata = [], []
ln, = plt.plot([], [], 'r-')

def update(frame):
    try:
        sock.sendto("xx", ("192.168.43.212",9999))
        data, addr = sock.recvfrom(1024)
        print "data", data
        y, x = struct.unpack("dd", data);
        xdata.append(x)
        ydata.append(y)

        ax.relim()
        ax.autoscale_view()
        ln.set_data(xdata, ydata)
        return ln,
    except:
        print "timeout"
    return ln

ani = FuncAnimation(fig, update, frames=range(10000), repeat=False, interval=10)
plt.show()