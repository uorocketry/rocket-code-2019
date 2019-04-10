#!/usr/bin/python3

import serial, time, re


file_name = 'rocket_data.csv'
total_len = 22
write_len = [8, 12]
write_len.extend([total_len-write_len[0]-write_len[1]])
write_len_dict = {
    'time': 8,
    'GPS': 12
}

## BS code
def print_line():
    read_data = ser.readline()
    print(read_data.decode() + ', \r\n')

def gen_rand_bytes():
    import random
    max_val = int(str(9)*total_len)
    rand_num = random.randint(1,max_val)
    return bytearray(str(rand_num), 'utf-8')

## Radio code
def test_baud():
    for test_baud in reversed(serial_speeds.keys()):
        ser.baudrate = test_baud
        ser.open()
        print('Testing serial port at', test_baud, 'baud.')
        if command_mode():
            print('Test passed at', test_baud, 'baud.')
            return(test_baud)   # return the baud value and leave serial port open
        print('Test failed at', test_baud, 'baud.')
        ser.close()
    print('Could not determine baud rate!  Exiting.')
    exit(100)

def enter_AT_mode():
    flush_ser()
    exit_AT_mode()

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

    print(get_response())

def flush_ser():
    ser.flushOutput()
    ser.flushInput()
    time.sleep(1)           # give the flush a second

def send_command(command):
    flush_ser()

    ser.write(command)
    print('Sent command: ', command.decode())
    time.sleep(1)

    response = get_response()

def get_response():
    sleep_time_after_buffer_read = 2
    inBuffer = ser.inWaiting()
    print("Characters in receive buffer before reading:", str(inBuffer))
    response = b''
    while inBuffer > 0:
        print("Reading serial port buffer.")
        # response = response + ser.readline(inBuffer)
        response = ser.readline(inBuffer)
        print("Response:", str(response))
        time.sleep(sleep_time_after_buffer_read)
        inBuffer = ser.inWaiting()
        #print("Characters in receive buffer after reading and waiting %d seconds:" % sleep_time_after_buffer_read, str(ser.inWaiting()))
    print("No more characters in serial port buffer.")
    return response

def exit_AT_mode(opt):
    flush_ser()

    command = b'\r\n'        # the ATO command must start on a newline
    ser.write(command)
    print('Sent command: (newline and return)', command.decode())
    time.sleep(0.5)
    if (opt == 'RT'):
        command = b'ATO\r\n'     # exit AT command mode if we are in it
    else:
        command = b'RTO\r\n'
    ser.write(command)
    print('Sent command:', command.decode())
    time.sleep(1)

    print(get_response())

## Write to file code
def write2file (data_in):
    data_in = data_in.decode()
    write_data = ",\t".join([data_in[sum(write_len[:i]) : sum(write_len[:i+1])] for i in range(len(write_len))  ])
    print(write_data)
    f.write(str(write_data) + '\r\n')

## =========================================

if __name__ == '__main__':
    ser = serial.Serial(
        port='/dev/ttyS4',
        baudrate=57600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout = 5
    )
    print('Opening serial port.') ## temp
    #print(ser)

    f = open(file_name, 'w+')
    f.write('Time,\tGPS Coordinates,\tAltitude\r\n')
    for x in range(0, 5):
        write2file(gen_rand_bytes())

    # test_baud()
    at_mode = input('Enter AT/RT mode? (1 for yes, else for no): ')
    if (at_mode == '1'):
        enter_AT_mode()

        command = b'ATI\r\n'
        print(send_command(command))

        cmd = '1'
        while (cmd == '1'):
            command = input('Enter AT/RT command: ')
            ## ie. ATSn? , register number n= 0 to 18
            ## For example: use RTI,1 to get RTI value for node 1 or simply use
            ## RTI to get the RTI value for the node set in the local NODEDESTINATION.
            ## RTI[,x], RTI3[,x]
            command = command.encode()

            rt = send_command(command)
            print(rt)
            time.sleep(0.2)
            rt = send_command(b'\r\n')
            print(rt)
            time.sleep(0.2)

            cmd = input('Exit command mode? \r\n\t(integers only, 1 to stay, others to exit)')
            cmd = cmd.rstrip()
        exit_AT_mode('RT')
        time.sleep(0.1)
        exit_AT_mode()

    ## TODO: Temp
    else:
        read_en = True;
        while read_en:
            rd = ser.readline()
            rd = rd.decode()
            rd = rd.zfill(total_len)

            write2file(rd)
            print(rd)

            time.sleep(0.1)
            if rd.rstrip() == b'':
                read_en = False
                time.sleep(0.5)

    ser.close()
    f.close()
    print('Closing serial port.') ## temp
