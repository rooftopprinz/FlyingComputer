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
    v = math.sin(2*math.pi*F*t)
    print "send: " + str(v) + " " + str(t)
    sock.sendto(struct.pack("dd", v, t), addr)

