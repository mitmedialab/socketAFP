from queue import Queue
import multiprocessing
from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot

# inputQ = Queue()
# serQ = Queue()
# serShutdown = Queue()

"""
    building a class around the queue
    in theory this lets me create a signal in the gui when the queue is updated
    for the Raw serial data coming out of the devices this is too much and
    just hangs the app...
"""


class DataQueue(QObject):

    queue_updated = pyqtSignal()

    def __init__(self):
        QObject.__init__(self)
        # multiprocessing queue seems to work better, but I could also just be imagining it...
        self.data = Queue()

    def queue_update(self):
        self.queue_updated.emit()



