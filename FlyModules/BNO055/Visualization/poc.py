#!/usr/bin/python
import socket
import time
import struct
import math
import sys
import os

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
tstart = time.time()
tlast  = time.time()

print "tstart=", tstart
print "tlast=", tlast

def update(frame):
    global sock
    global gyroxdata
    global gyroydata
    global gyrozdata
    global accelxdata
    global accelydata
    global accelzdata
    global tdata
    global gyrox
    global gyroy
    global gyroz
    global accelx
    global accely
    global accelz
    global tstart
    global tlast
    try:
        sock.sendto("xx", ("192.168.43.212",9999))
        data, addr = sock.recvfrom(1024)

        t = time.time()
        t0 = t - tstart
        ox,oy,oz,ax,ay,az = struct.unpack("ffffff", data);
        print "G (" + str(ox) + "," + str(oy) + "," + str(oz) + ","+ str(t0) +")"
        print "A (" + str(ax) + "," + str(ay) + "," + str(az) + ","+ str(t0) +")"
        gyroxdata.append(ox)
        gyroydata.append(oy)
        gyrozdata.append(oz)
        accelxdata.append(ax)
        accelydata.append(ay)
        accelzdata.append(az)
        tdata.append(t0)

        if (600==len(gyroxdata)):
            gyroxdata.pop(0)
            gyroydata.pop(0)
            gyrozdata.pop(0)
            accelxdata.pop(0)
            accelydata.pop(0)
            accelzdata.pop(0)
            tdata.pop(0)

        if ((t-tlast)>3):
            print "reset"
            tstart = t
            gyroxdata   = []
            gyroydata   = []
            gyrozdata   = []
            accelxdata  = []
            accelydata  = []
            accelzdata  = []
            tdata       = []
        
        tlast = t

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
    except Exception as e:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(exc_type, fname, exc_tb.tb_lineno)

    return [gyrox,gyroy,gyroz,accelx,accely,accelz]

ani = FuncAnimation(fig, update, frames=range(10000), repeat=False, interval=100)
plt.show()