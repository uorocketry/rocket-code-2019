#!/usr/bin/python3

import serial, time, re
from datetime import datetime

SLEEP_INTERVAL = 2


def send_command(command):
    flush_ser()

    ser.write(command)
    print('Sent command: ', command.decode())
    time.sleep(SLEEP_INTERVAL)
    return ser.readline()

def exit_AT_mode(opt):
    flush_ser()

    command = b'\r\n'        # the ATO command must start on a newline
    ser.write(command)
    print('Sent command: (newline and return)', command.decode())
    time.sleep(SLEEP_INTERVAL/2)
    if (opt == 'AT'):
        command = b'ATO\r\n'     # exit AT command mode if we are in it
    else:
        command = b'RTO\r\n'
    ser.write(command)
    print('Sent command:', command.decode())
    time.sleep(SLEEP_INTERVAL/2)

    return ser.readline()


ser = serial.Serial(
    port='/dev/ttyS4',
    baudrate=57600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout = 5
)



def flush_ser():
    ser.flushOutput()
    ser.flushInput()
    time.sleep(SLEEP_INTERVAL)           # give the flush a second

def setup():
    flush_ser()


def loop(f):
    inBuffer = ser.inWaiting()

    while inBuffer > 0 :
            response = ser.readline()
            print(response)
            f.write(str(response) + '\n')
            time.sleep(0.1)

    response = ser.readline()
    print(response)

    time.sleep(SLEEP_INTERVAL/8)


if __name__ == '__main__':
    FILE_FORMAT = datetime.now()

    f= open ("data_radio{}.rocketlog".format(FILE_FORMAT), "w+")

    print("=== Setup ===")
    setup()
    print("=== Loop ===")
    while True:
        try:
            loop(f)
        except Exception as e:
            print(e)

    f.close()
