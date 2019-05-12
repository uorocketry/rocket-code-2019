import serial

def talk():
    ser = serial.Serial()
    ser.baudrate = 9600
    ser.port = '/dev/ttyS5'
    ser.timeout = 5
    ser.open()
    text = ser.readline()
    ser.close()
    return(text)

if __name__ == '__main__':
    print(talk())
