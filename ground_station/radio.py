#!/usr/bin/python3

import serial
import time


def print_line():
    print(ser.read())

if __name__ == "__main__":
    ser = serial.Serial(
        port='/dev/ttyS6',
        baudrate=57600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout = 5
    )
    print('Opening serial port...') // temp
    print(ser)

    ser.write('+++\r')
    time.sleep(1)
    print_line()

    ser.write('AT\r')
    time.sleep(1)
    print_line()

    ser.close()
    print('Closing serial port...') // temp
