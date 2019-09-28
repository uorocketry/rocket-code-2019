import serial
import time


def sim_flight(state):
	alt = state["alt"]
	prev_alt = alt
	alt = alt+80*(1-alt/3000)
	vel = (alt - prev_alt)
	state["alt"] = alt
	state["vel"] = vel
	state["lat"] = state["lat"]+1
	state["lon"] = state["lon"]+1
	state["pch"] = state["pch"]+1
	state["yaw"] = state["yaw"]+1
	state["rol"] = state["rol"]+1
	state["acx"] = state["acx"]+1
	state["acy"] = state["acy"]+1
	state["acz"] = state["acz"]+1



def repr_to_serial(state):
	return str(state["alt"])+", "+str(state["vel"])+", "+str(state["lat"])+", "+str(state["lon"])+", "+str(state["pch"])+", "+str(state["yaw"])+", "+str(state["rol"])+", "+str(state["acx"])+", "+str(state["acy"])+", "+str(state["acz"])

def serial_decoder(s):
	newS = ""

	for c in s:
		if(c in "0123456789"):
			newS= newS+c
	return newS


#constant variables
com = '/dev/ttyS8' #can and should be changed(to COM for ex) for different devices


#struct(dictionary)
state = {
	"alt":0,
	"vel":0,
	"lat":0,
	"lon":0,
	"pch":0,
	"yaw":0,
	"rol":0,
	"acx":0,
	"acy":0,
	"acz":0

}


#serial init
ser = serial.Serial(port = com, baudrate = 9600, timeout = 2)
ser.close()
ser.open()


#main loop
while(True):
	ser.flush()
	message_to_send = repr_to_serial(state)
	print ("Python value sent: ")
	print (message_to_send)
	ser.write(message_to_send.encode("utf-8"))

	time.sleep(1)

	message_received = ser.readline()
	print ("Message from arduino: ")
	print (serial_decoder(str(message_received)))
	sim_flight(state)