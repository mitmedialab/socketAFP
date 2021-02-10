import sys
import serial
import time
import threading
import typing
import msgpack
from queue import Queue

global inputQ
global serQ
global serShutdown

inputQ = Queue()
serQ = Queue()
serShutdown = Queue()


def input_listener():
	"""
	get command line input
	:return:
	"""
	while True:
		clInput = input(" ")
		print(input)

		if "q" == clInput:
			print("want to quit")

		inputQ.put(clInput)
		# qInput.put(input)
		print("input added to Queue")


####
# Grab serial data and put into q
# ###
def ser_handler(ser: serial.Serial):
	print("serial q")
	is_open = ser.is_open
	if is_open:
		print(f'Serial is open (is_open={is_open}')
		while True:
			# this is faster than serial.readline
			# reads however many bytes are waiting in buffer
			if 0 < ser.in_waiting:
				serQ.put(ser.read(ser.in_waiting))

			if 0 == ser.in_waiting:
				if not serShutdown.empty():
					break


# def main(argv):
def main():
	# file_name = str(sys.argv[1]) + str(time.time()) + '.csv'
	# f = open(file_name, "w+")

	# for instructions on opening the serial port check
	# https://pythonhosted.org/pyserial/shortintro.html#opening-serial-ports
	ser = serial.Serial('COM23', 115200) ## uncomment for windows, check com port with arduino or device manager
	print("serialOpened")
	# ser = serial.Serial('/dev/cu.usbmodem1101', 500000)

	##
	# create threads
	##
	input_thread = threading.Thread(target=input_listener)
	input_thread.setDaemon(True)
	input_thread.start()

	print("input thread created")
	ser_thread = threading.Thread(target=ser_handler, args=(ser,))
	ser_thread.setDaemon(True)
	ser_thread.start()

	print("threads created")
	####
	# handle all the inputs continuously
	####
	while True:

		# handle inputs from commandline
		if not inputQ.empty():
			new_input = inputQ.get()
			print('received input')
			# print (newInput)
			# break
			if "q" == new_input:
				serShutdown.put("q")
				time.sleep(1)
				ser.close()
				break
			else:
				print(new_input)

		# handle inputs serial
		if not serQ.empty():
			newSer = serQ.get()

			unpacked = msgpack.unpackb(newSer, raw=False)
			print(unpacked)

			print(newSer)


	# f.close()
	ser.close()


# main(sys.argv)
main()
