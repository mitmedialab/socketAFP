from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout
from PyQt5.QtWidgets import (QApplication, QCheckBox, QComboBox, QDateTimeEdit,
                             QDial, QDialog, QGridLayout, QGroupBox, QHBoxLayout, QLabel, QLineEdit,
                             QProgressBar, QPushButton, QRadioButton, QScrollBar, QSizePolicy,
                             QSlider, QSpinBox, QStyleFactory, QTableWidget, QTabWidget, QTextEdit,
                             QVBoxLayout, QWidget, QFormLayout, QListWidget, QListWidgetItem)
from PyQt5.QtCore import QTextStream, QTimer
import  PyQt5.QtCore

from messages import Messages
from enums import (Board, State)
from threading import Thread
from dataQueues import DataQueue


class GuiMain:
    # def __init__(self, parent=None):
    #     super(GuiMain, self).__init__(parent)
    def __init__(self, incoming_data: DataQueue, out_data: DataQueue):
        print("here we go")
        self.new_data = incoming_data
        self.out_data = out_data
        print("Created newdata attribute")
        print("connected slot to the signal")
        self.line_count = 0

    def run(self, argv):
        # Thread.__init__(self)
        # super(GuiMain, self).__init__(parent)

        # body = QGridLayout()

        app = QApplication([argv])
        window = QWidget()
        self.gains_input = QGroupBox("Group1")
        self.position_input = QGroupBox("group 2")
        self.terminal_box = QGroupBox("group 3")
        self.send_box = QGroupBox("group 4")

        self.create_gains_input()
        self.create_position_input()
        self.create_send()
        self.create_terminal()

        layout = QHBoxLayout()
        main_layout = QGridLayout()

        layout.addWidget(QPushButton('Top'))
        layout.addWidget(QPushButton('Bottom'))

        # main_layout.addWidget(layout, 0, 0)
        main_layout.addWidget(self.gains_input, 0, 0,)
        main_layout.addWidget(self.position_input, 1, 0)
        main_layout.addWidget(self.terminal_box, 0, 1, 3, 1)
        main_layout.addWidget(self.send_box, 2, 0)

        self.terminal_update_timer = QTimer()
        # self.terminal_update_timer.singleShot(10000, self.update_terminal)
        self.terminal_update_timer.timeout.connect(self.update_terminal)
        self.terminal_update_timer.start(200)
        window.setLayout(main_layout)
        window.show()
        app.exec()

    def create_gains_input(self):
        self.p_gain_input = QLineEdit('')
        self.i_gain_input = QLineEdit('')
        self.d_gain_input = QLineEdit('')

        layout = QFormLayout()
        layout.addRow("p gain", self.p_gain_input)
        layout.addRow("i gain", self.i_gain_input)
        layout.addRow("d gain", self.d_gain_input)

        self.gains_input.setLayout(layout)

    def create_position_input(self):
        self.y_position_input = QLineEdit('')
        self.z_position_input = QLineEdit('')
        self.z_rotation_input = QLineEdit('')
        self.alpha_rotation_input = QLineEdit('')

        layout = QFormLayout()
        layout.addRow("y position", self.y_position_input)
        layout.addRow("z position", self.z_position_input)
        layout.addRow("z rotation", self.z_rotation_input)
        layout.addRow("alpha rotation", self.alpha_rotation_input)

        self.position_input.setLayout(layout)

    def create_terminal(self):
        self.terminal_window = QListWidget()
        self.terminal_window.setMinimumWidth(1000)
        self.terminal_window.setMinimumHeight(1000)
        self.terminal_window.verticalScrollBar().setSingleStep(2)

        self.send_summary = QTextEdit()
        self.send_summary.setFixedHeight(200)

        clear_windows = QPushButton("clear terminals")
        clear_windows.clicked.connect(self.on_clear_terminal)

        connect_sea = QCheckBox("Connect SEA")
        connect_general = QCheckBox("connect general")
        log = QCheckBox("log data")

        layout = QGridLayout()
        layout.addWidget(self.terminal_window)
        layout.addWidget(self.send_summary)
        layout.addWidget(clear_windows)
        layout.addWidget(connect_sea)
        layout.addWidget(connect_general)
        layout.addWidget(log)

        self.terminal_box.setLayout(layout)

    def create_send(self):
        layout = QGridLayout()
        send_sea = QPushButton("send to SEA")
        send_general = QPushButton("send General")
        send_all = QPushButton("send all")

        # connect button callbacks
        send_sea.clicked.connect(self.on_send_sea_click)
        send_general.clicked.connect(self.on_general_click)
        send_all.clicked.connect(self.on_all_click)


        layout.addWidget(send_sea)
        layout.addWidget(send_general)
        layout.addWidget(send_all)

        self.send_box.setLayout(layout)

    def read_sea_inputs(self) -> Messages:
        y_position = self.y_position_input.text()
        p_gain = self.p_gain_input.text()
        i_gain = self.i_gain_input.text()
        d_gain = self.d_gain_input.text()

        SEA_message = Messages()
        SEA_message.destination = Board.SEA
        SEA_message.gui_set_gain(p_gain, d_gain, i_gain)
        SEA_message.gui_update_y_position(y_position)

        return SEA_message

    def read_general_inputs(self) -> Messages:
        z_rot = self.z_rotation_input.text()
        z_pos = self.z_position_input.text()
        alpha_rot = self.alpha_rotation_input.text()

        general_message = Messages()
        general_message.destination = Board.general
        general_message.gui_update_z_rotation(z_rot)
        general_message.gui_update_z_position(z_pos)
        general_message.gui_update_alpha_rotation(alpha_rot)

        return general_message

    def on_send_sea_click(self):
        SEA_send = self.read_sea_inputs()
        self.send_summary.append(SEA_send.pack_json())
        self.out_data.data.put(SEA_send)

    def on_general_click(self):
        general_send = self.read_general_inputs()
        self.send_summary.append(general_send.pack_json())
        self.out_data.data.put(general_send)

    def on_all_click(self):
        self.read_sea_inputs()
        self.read_general_inputs()

    def on_clear_terminal(self):
        self.terminal_window.clear()

    """
        update_terminal is called on a timer
        New items are added to data queue faster than can be added to the list view 
        this will cause cause the program to hang, and also hang the microcontroller (which is weird) 
        So this runs a loop for a portion of the timeout adding a bunch of items to the output display
        it is faster to put items in list and then add whole list to listWidget in one go. 
    """
    def update_terminal(self):

        try:
            update_items = list()
            while self.terminal_update_timer.remainingTime() > 70:
                if not self.new_data.data.empty():
                    update_items.append(str(self.line_count) + ' ' + self.new_data.data.get())
                    self.new_data.data.task_done()
                    self.line_count += 1
            if update_items:
                self.terminal_window.addItems(update_items)
                self.terminal_window.scrollToBottom()

        except Exception as e:
            print(e)
            print("couldnt add line")




