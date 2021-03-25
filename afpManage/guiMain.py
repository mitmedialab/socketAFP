from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout
from PyQt5.QtWidgets import (QApplication, QCheckBox, QComboBox, QDateTimeEdit,
                             QDial, QDialog, QGridLayout, QGroupBox, QHBoxLayout, QLabel, QLineEdit,
                             QProgressBar, QPushButton, QRadioButton, QScrollBar, QSizePolicy,
                             QSlider, QSpinBox, QStyleFactory, QTableWidget, QTabWidget, QTextEdit,
                             QVBoxLayout, QWidget, QFormLayout)
from messages import Messages
from threading import Thread
class GuiMain:

    def __init__(self):
        print("here we go")

    def run(self):
        # Thread.__init__(self)
        # super(GuiMain, self).__init__(parent)

        body = QGridLayout()

        app = QApplication([])
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
        self.terminal_window = QTextEdit()
        self.terminal_window.setFixedWidth(400)
        self.terminal_window.setFixedHeight(500)

        send_summary = QTextEdit()
        send_summary.setFixedHeight(200)

        clear_windows = QPushButton("clear terminals")
        clear_windows.clicked.connect(self.on_clear_terminal)

        connect_sea = QCheckBox("Connect SEA")
        connect_general = QCheckBox("connect general")
        log = QCheckBox("log data")

        layout = QGridLayout()
        layout.addWidget(self.terminal_window)
        layout.addWidget(send_summary)
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

    def read_sea_inputs(self):
        y_position = self.y_position_input.text()
        p_gain = self.p_gain_input.text()
        i_gain = self.i_gain_input.text()
        d_gain = self.d_gain_input.text()

        print(y_position)
        print(p_gain)
        print(i_gain)
        print(d_gain)

    def read_general_inputs(self):
        z_rot = self.z_rotation_input.text()
        z_pos = self.z_position_input.text()
        alpha_rot = self.alpha_rotation_input.text()

        print(z_rot)
        print(z_pos)
        print(alpha_rot)

    def on_send_sea_click(self):
        self.read_sea_inputs()

    def on_general_click(self):
        self.read_general_inputs()

    def on_all_click(self):
        self.read_sea_inputs()
        self.read_general_inputs()

    def on_clear_terminal(self):
        self.terminal_window.clear()

    # function to update the terminal window
    def update_terminal(self, new_line: str):
        # self.terminal_window.append(new_line)
        print(new_line)

# new_gui = GuiMain()
