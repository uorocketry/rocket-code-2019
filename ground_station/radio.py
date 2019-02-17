#!/usr/bin/python3

import serial
import time


def print_line():
    read_data = ser.read()
    print(read_data.decode() + ', ')

if __name__ == "__main__":
    ser = serial.Serial(
        port='/dev/ttyS4',
        baudrate=57600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout = 5
    )
    print('Opening serial port...') ## temp
    print(ser)

    ser.write(b'+++\r') ## enter AT command mode
    time.sleep(1)
    print_line()

    ser.write(b'AT\r')
    time.sleep(1)
    print_line()

    commands = [b'ATI\r', b'ATI2\r', b'ATI3\r', b'ATI4\r', b'ATI6\r',b'ATI7\r']

    for i in commands:
        print(i.decode())
        ser.write(i)
        time.sleep(1)

    ser.write(b'ATO\r') ## exit AT command mode
    time.sleep(1)
    print_line()

    ser.close()
    print('Closing serial port...') ## temp
