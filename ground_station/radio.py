#!/usr/bin/python3

import serial, time, re

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
    if (opt == 'RT'):
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

    command = b'\r\n'        # the ATO command must start on a newline
    ser.write(command)
    print("Sent command: (\r\n)", command)

    command = b'ATI\r\n'     # test to see if we are stuck in AT command mode.  If so, we see a response from this.
    ser.write(command)
    print('Sent command:', command.decode())
    time.sleep(2)           # minimum 1 second wait needed before +++

    command = b'+++'         # +++ enters AT command mode
    ser.write(command)
    print('Sent command:', command.decode())
    time.sleep(3)           # minimum 1 second wait after +++

    command = b'ATI\r\n'
    print(send_command(command))

    time.sleep(3)
    cmds = ['ATS4=27', 'ATS4?']
    for item in cmds:
        rt = send_command(item.encode())
        print(rt)
        time.sleep(0.2)
        rt = send_command(b'\r\n')
        print(rt)
        time.sleep(0.2)

#    exit_AT_mode('RT')
#    time.sleep(0.1)
    exit_AT_mode('AT')


def loop():
    inBuffer = ser.inWaiting()
    response = ser.readline(inBuffer)

#    response = ser.readline()
    print(response)
    time.sleep(SLEEP_INTERVAL/2)


if __name__ == '__main__':
    print("=== Setup ===")
    setup()
    print("=== Loop ===")
    while True:
        try:
            loop()
        except Exception as e:
            print(e)
