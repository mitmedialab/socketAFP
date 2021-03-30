import sys
import serial
import time
import threading
import multiprocessing
import json
import typing
from guiMain import GuiMain
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication
from ast import literal_eval
from queue import Queue
from dataQueues import DataQueue
from messages import Messages
from enums import Board

inputQ = multiprocessing.Queue()
serQ = multiprocessing.Queue()
serShutdown = multiprocessing.Queue()


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


def serial_sender(SEA_port: serial.Serial, general_port: serial.Serial, outgoing_queue: DataQueue):
	if SEA_port.is_open or general_port.is_open:
		while True:
			if not outgoing_queue.data.empty():
				print("got a message")
				message_to_send = Messages()
				message_to_send = outgoing_queue.data.get()
				board = message_to_send.destination
				if board == Board.SEA:
					print("this is going to the SEA")
					message_as_bytes = message_to_send.pack_byte_array()
					print(message_as_bytes)
					SEA_port.write(message_as_bytes)
				time.sleep(0.01)



# def main(argv):
def main():

	gui_send_queue = DataQueue()
	gui_response_queue = DataQueue()

	afp_gui = GuiMain(gui_send_queue, gui_response_queue)

	print("gui object created")
	# file_name = str(sys.argv[1]) + str(time.time()) + '.csv'
	# f = open(file_name, "w+")

	# for instructions on opening the serial port check
	# https://pythonhosted.org/pyserial/shortintro.html#opening-serial-ports

	SEA_serial = serial.Serial('COM22', 115200)  # uncomment for windows, check com port with arduino or device manager
	# ser = serial.Serial('/dev/cu.usbmodem1101', 115200) # uncomment for mac and check the port

	general_serial = serial.Serial('COM15', 115200)


	##
	# create threads
	##

	input_thread = threading.Thread(target=input_listener)
	input_thread.setDaemon(True)
	input_thread.start()

	print("input thread created")
	SEA_serial_thread = threading.Thread(target=ser_handler, args=(SEA_serial,))
	SEA_serial_thread.setDaemon(True)
	SEA_serial_thread.start()

	gui_thread = threading.Thread(target=afp_gui.run, args=sys.argv)
	gui_thread.setDaemon(True)
	gui_thread.start()

	send_thread = threading.Thread(target=serial_sender, args=(SEA_serial, general_serial, gui_response_queue))
	send_thread.setDaemon(True)
	send_thread.start()

	# can't switch to multiprocessing, because of my classes - would have to follow these instructions
	# https://stackoverflow.com/questions/8804830/python-multiprocessing-picklingerror-cant-pickle-type-function
	# input_thread = multiprocessing.Process(target=input_listener)
	# input_thread.daemon = True
	# input_thread.start()
	#
	# print("input thread created")
	# SEA_serial_thread = multiprocessing.Process(target=ser_handler, args=SEA_serial)
	# SEA_serial_thread.daemon = True
	# SEA_serial_thread.start()
	#
	# gui_thread = multiprocessing.Process(target=afp_gui.run, args=(sys.argv))
	# gui_thread.daemon = True
	# gui_thread.start()
	#
	# send_thread = multiprocessing.Process(target=serial_sender, args=(SEA_serial, general_serial, gui_response_queue))
	# send_thread.daemon = True
	# send_thread.start()

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


	# f.close()
	ser.close()


# main(sys.argv)
main()
