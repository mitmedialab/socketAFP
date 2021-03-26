import sys
import serial
import time
import threading
import json
import typing
from guiMain import GuiMain
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication
from ast import literal_eval
from queue import Queue
from dataQueues import DataQueue

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
		print("input added to Queue")


####
# Grab serial data and put into q
# ###
def ser_handler(ser: serial.Serial):
	"""
	grab grab the serial data and put it into a q
	:param ser:
	:return:
	"""
	print("serial q")
	is_open = ser.is_open
	if is_open:
		print(f'Serial is open (is_open={is_open}')
		while True:
			# this is faster than serial.readline
			# reads however many bytes are waiting in buffer
			if 0 < ser.in_waiting:
				incoming = ser.read(ser.in_waiting)
				serQ.put(incoming)
				"""
				incoming serial needs to be converted from b'' to string
				then string needs to be broken into a list of string to convert it to a json string. 
				"""

			if 0 == ser.in_waiting:
				if not serShutdown.empty():
					break


# def main(argv):
def main():

	gui_send_queue = DataQueue()
	afp_gui = GuiMain(gui_send_queue)

	print("gui object created")
	# file_name = str(sys.argv[1]) + str(time.time()) + '.csv'
	# f = open(file_name, "w+")

	# for instructions on opening the serial port check
	# https://pythonhosted.org/pyserial/shortintro.html#opening-serial-ports
	ser = serial.Serial('COM22', 115200)  # uncomment for windows, check com port with arduino or device manager
	ser.flush()
	# ser = serial.Serial('/dev/cu.usbmodem1101', 115200) # uncomment for mac and check the port

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

	guiThread = threading.Thread(target=afp_gui.run, args=sys.argv)
	guiThread.setDaemon(True)
	guiThread.start()

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
			new_ser = serQ.get()
			# gui_send_queue.data.put(new_ser)
			# gui_send_queue.queue_update()
			# print(new_ser)
			try:
				new_serial_string = new_ser.decode("utf-8")
				gui_send_queue.data.put(new_serial_string)
				gui_send_queue.data.put(new_serial_string)
				# gui_send_queue.queue_update()
			except Exception as e:
				print(e)
				# print("couldnt decode line")
				print(new_ser)
			# afp_gui.update_terminal(new_serial_string)
			# guiThread.update_terminal(newSer)
			# print(newSer)




	# f.close()
	ser.close()


# main(sys.argv)
main()
