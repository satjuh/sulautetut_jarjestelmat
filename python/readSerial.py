#!/usr/bin/python

import serial
import time
import signal
import sys

def signal_handler(sig, frame):
    print("ctr+c quitting...")
    sys.exit(0)
# read serial input every second and print output
if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    ser = serial.Serial('/dev/ttyACM0')
    while True:
        line = ser.readline().decode("utf-8").strip("\n")
        print(line)
        time.sleep(1)
