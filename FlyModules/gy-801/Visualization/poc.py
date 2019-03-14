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

fig, (ax0, ax1) = plt.subplots(2,1)
gyroxdata = []
gyroydata = []
gyrozdata = []
accelxdata = []
accelydata = []
accelzdata = []
tdata = []

gyrox, = ax0.plot([], [], 'r-')
gyroy, = ax0.plot([], [], 'g-')
gyroz, = ax0.plot([], [], 'b-')

accelx, = ax1.plot([], [], 'r-')
accely, = ax1.plot([], [], 'g-')
accelz, = ax1.plot([], [], 'b-')


def update(frame):
    try:
        sock.sendto("xx", ("192.168.43.212",9999))
        data, addr = sock.recvfrom(1024)

        t = time.time()
        x, y, z = struct.unpack("fff", data);
        print "(" + str(x) + "," + str(y) + "," + str(z) + ","+ str(t) +")"
        gyroxdata.append(x)
        gyroydata.append(y)
        gyrozdata.append(z)
        tdata.append(t)

        accelxdata.append(-x)
        accelydata.append(-y)
        accelzdata.append(-z)

        if (200==len(gyroxdata)):
            gyroxdata.pop(0)
            gyroydata.pop(0)
            gyrozdata.pop(0)
            accelxdata.pop(0)
            accelydata.pop(0)
            accelzdata.pop(0)
            tdata.pop(0)

        ax0.relim()
        ax0.autoscale_view()
        gyrox.set_data(tdata, gyroxdata)
        gyroy.set_data(tdata, gyroydata)
        gyroz.set_data(tdata, gyrozdata)

        ax1.relim()
        ax1.autoscale_view()
        accelx.set_data(tdata, accelxdata)
        accely.set_data(tdata, accelydata)
        accelz.set_data(tdata, accelzdata)
        return [gyrox,gyroy,gyroz,accelx,accely,accelz]
    except:
        print "timeout"
    return [gyrox,gyroy,gyroz,accelx,accely,accelz]

ani = FuncAnimation(fig, update, frames=range(10000), repeat=False, interval=100)
plt.show()