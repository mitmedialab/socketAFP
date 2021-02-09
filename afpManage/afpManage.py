import sys
import serial
import time
import threading
from queue import Queue

global inputQ
global serQ
global serShutdown

inputQ = Queue()
serQ = Queue()
serShutdown  = Queue()


#######
# get command line input
#######
def input_listener():
  while True:
    clInput = input(" ")
    print (input)

    if "q" == clInput:
    	print ("want to quit")

    inputQ.put(clInput)
    # qInput.put(input)
    print ("input added to Queue")



####
# Grab serial data and put into q
# ###
def ser_handler(ser):
	isOpen = ser.is_open
	if isOpen:
		print('Serial is open')
		while True:
			# this is faster than serial.readline
			# reads however many bytes are waiting in buffer
			if (0 < ser.in_waiting):
				serQ.put(ser.read(ser.in_waiting))

			if(0 == ser.in_waiting):
				if (not serShutdown.empty()):
					break
	# else:
	# 	delay(100)
	# 	ser.close()

def main(argv):
	fileName  = str(sys.argv[1]) + str(time.time()) + '.csv'
	f = open(fileName, "w+")

	# for instructions on opening the serial port check https://pythonhosted.org/pyserial/shortintro.html#opening-serial-ports
	# ser = serial.Serial('COM5', 500000) ##uncomment for windows, check com port with arduino or device manager
	ser = serial.Serial('/dev/cu.usbmodem1101', 500000)

	##
	# create threads
	##
	threads = []
	inputThread = threading.Thread(target = input_listener())
	inputThread.setDaemon(True)
	inputThread.start()

	serThread = threading.Thread(target = ser_handler(), args =(ser,))
	serThread.setDaemon(True)
	serThread.start()


	####
	# handle all the inputs continuously
	####
	while True:

		#handle inputs from commandline
		if (not inputQ.empty()):
			newInput = inputQ.get()
			print ('received input')
			# print (newInput)
			# break
			if "q" == newInput:
				serShutdown.put("q")
				time.sleep(1)
				ser.close()
				break
			else:
				print (newInput)

		#handle inputs serial
		if(not serQ.empty()):
			newSer = str(serQ.get())

			# print (newSer)
			length  = len(newSer)
			removeStart = newSer[2:length]
			removeR = removeStart.replace ("\\r" , "")
			removePos = removeR.replace ("\'" , "")
			newLines = removePos.replace("\\n" , (", " +  str(time.time()) + " \n"))
			f.write(newLines)
			print (newLines, end = '' )


	f.close()
	ser.close()

main(sys.argv)
