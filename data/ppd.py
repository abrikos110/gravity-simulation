from math import *
from random import *

def rc():
    x = (random()*2-1)*radius
    y = (random()*2-1)*radius
    z = (random()*2-1)*radius
    while x*x+y*y+z*z > radius*radius:
        x = (random()*2-1)*radius
        y = (random()*2-1)*radius
        z = (random()*2-1)*radius
    return x, y, z

G = 6.674e-11

n = int(input())
mass = float(input())
radius = float(input())
rr = (mass/n/1000*3/4/pi) ** (1/3)  # v = m/p = m/1000*3/4/pi = r^3
vel = sqrt(G*mass/(radius/1))

for i in range(n):
    x, y, z = rc()
    absvel = vel * sqrt(x*x+y*y) / radius + 0.05*(random()*2-1)*vel
    vx, vy = y/sqrt(x*x+y*y) * absvel, -x/sqrt(x*x+y*y) * absvel
    vz = 0
    print(x, y, z, vx, vy, vz, mass/n, rr)
