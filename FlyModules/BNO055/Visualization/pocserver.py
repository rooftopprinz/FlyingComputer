#!/usr/bin/python
import socket
import time
import struct
import math

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(('', 9999))

while (1):
    data, addr = sock.recvfrom(1024)
    t = time.time()
    F = 1;
    v1 = math.sin(2*math.pi*F*t)
    v2 = math.sin(2*math.pi*F*t+2*math.pi/3)
    v3 = math.sin(2*math.pi*F*t+4*math.pi/3)
    print "send: " + str(v1) + " " + str(v2) + " " + str(v3)
    sock.sendto(struct.pack("fff", v1, v2, v3), addr)

