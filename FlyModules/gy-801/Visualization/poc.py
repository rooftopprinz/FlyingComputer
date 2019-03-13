#   #!/usr/bin/python
import socket
import time
import struct
import math

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.settimeout(1)

fig, ax = plt.subplots()
xdata = []
ydata = []
zdata = []
tdata = []

lnx, = plt.plot([], [], 'r-')
lny, = plt.plot([], [], 'g-')
lnz, = plt.plot([], [], 'b-')

def update(frame):
    try:
        sock.sendto("xx", ("192.168.43.212",9999))
        data, addr = sock.recvfrom(1024)

        t = time.time()
        x, y, z = struct.unpack("fff", data);
        print "(" + str(x) + "," + str(y) + "," + str(z) + ","+ str(t) +")"
        xdata.append(x)
        ydata.append(y)
        zdata.append(z)
        tdata.append(t)

        if (200==len(xdata)):
            xdata.pop(0)
            ydata.pop(0)
            zdata.pop(0)
            tdata.pop(0)

        ax.relim()
        ax.autoscale_view()
        lnx.set_data(tdata, xdata)
        lny.set_data(tdata, ydata)
        lnz.set_data(tdata, zdata)
        return [lnx,lny,lnz]
    except:
        print "timeout"
    return [lnx,lny,lnz]

ani = FuncAnimation(fig, update, frames=range(10000), repeat=False, interval=100)
plt.show()