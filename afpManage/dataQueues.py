from queue import Queue
from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot

# inputQ = Queue()
# serQ = Queue()
# serShutdown = Queue()


class DataQueue(QObject):

    queue_updated = pyqtSignal()

    def __init__(self):
        QObject.__init__(self)
        self.data = Queue()

    def queue_update(self):
        self.queue_updated.emit()



