from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout
from PyQt5.QtWidgets import (QApplication, QCheckBox, QComboBox, QDateTimeEdit,
                             QDial, QDialog, QGridLayout, QGroupBox, QHBoxLayout, QLabel, QLineEdit,
                             QProgressBar, QPushButton, QRadioButton, QScrollBar, QSizePolicy,
                             QSlider, QSpinBox, QStyleFactory, QTableWidget, QTabWidget, QTextEdit,
                             QVBoxLayout, QWidget, QFormLayout)
from messages import Messages

class GuiMain:

    def __init__(self):
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
        p_gain_input = QLineEdit('')
        i_gain_input = QLineEdit('')
        d_gain_input = QLineEdit('')

        layout = QFormLayout()
        layout.addRow("p gain", p_gain_input)
        layout.addRow("i gain", i_gain_input)
        layout.addRow("d gain", d_gain_input)

        self.gains_input.setLayout(layout)

    def create_position_input(self):
        y_position_input = QLineEdit('')
        z_position_input = QLineEdit('')
        z_rotation_input = QLineEdit('')
        alpha_rotation_input = QLineEdit('')

        layout = QFormLayout()
        layout.addRow("y position", y_position_input)
        layout.addRow("z position", z_position_input)
        layout.addRow("z rotation", z_rotation_input)
        layout.addRow("alpha rotation", alpha_rotation_input)

        self.position_input.setLayout(layout)

    def create_terminal(self):
        terminal_window = QTextEdit()
        terminal_window.setFixedWidth(400)
        terminal_window.setFixedHeight(500)

        send_summary = QTextEdit()
        send_summary.setFixedHeight(200)

        connect_sea = QCheckBox("Connect SEA")
        connect_general = QCheckBox("connect general")
        log = QCheckBox("log data")

        layout = QGridLayout()
        layout.addWidget(terminal_window)
        layout.addWidget(send_summary)
        layout.addWidget(connect_sea)
        layout.addWidget(connect_general)
        layout.addWidget(log)

        self.terminal_box.setLayout(layout)


    def create_send(self):
        layout = QGridLayout()
        send_sea = QPushButton("send to SEA")
        send_general = QPushButton("send General")

        layout.addWidget(send_sea)
        layout.addWidget(send_general)

        self.send_box.setLayout(layout)


new_gui = GuiMain()
