from PySide6.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QLabel, QMessageBox
import sys
import serial
import pyqtgraph as pg
import os
import struct
import numpy as np
import shutil

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.conversion_number = 0

        self.setWindowTitle("STM32 Plotter")
        self.resize(600, 400)

        self.ser = serial.Serial(port="COM3", baudrate=9600, timeout=2)

        self.log_dir = os.path.join(os.path.dirname(__file__), "Log")
        os.makedirs(self.log_dir, exist_ok=True)
        self.output_path = os.path.join(self.log_dir, f"output_{self.conversion_number}.txt")

        layout = QVBoxLayout()

        self.button_conv = QPushButton("Start conversion")
        self.button_conv.clicked.connect(self.start_conversion)
        layout.addWidget(self.button_conv)

        self.button_plot = QPushButton("Plot")
        self.button_plot.clicked.connect(self.plot)
        layout.addWidget(self.button_plot)

        self.button_clear_log = QPushButton("Clear log")
        self.button_clear_log.clicked.connect(self.clear_log)
        layout.addWidget(self.button_clear_log)

        self.plot_widget = pg.PlotWidget()
        self.plot_widget.addLegend()
        self.plot_widget.setLabel('left', 'Current, mA')
        self.plot_widget.setLabel('bottom', 'Voltage, V')
        self.plot_widget.showGrid(x=True, y=True)
        layout.addWidget(self.plot_widget)

        self.setLayout(layout)

    def start_conversion(self):
        try:
            self.ser.write(b'\xAA')
            while True:
                byte = self.ser.read(1)
                if not byte:
                    continue
                if byte == b'\xC0':
                    break
                if byte[0] != 0xAA:
                    continue

                packet = self.ser.read(9)
                if len(packet) != 9:
                    continue

                data_bytes = byte + packet[:-1]
                crc_received = packet[-1]

                crc_calc = 0
                for b in data_bytes:
                    crc_calc ^= b
                if crc_calc != crc_received:
                    continue

                x = struct.unpack('<f', packet[0:4])[0]
                y = struct.unpack('<f', packet[4:8])[0]

                with open(self.output_path, "a", encoding="utf-8") as f:
                    f.write(f"{x:.6f}\t{y:.6f}\n")
            QMessageBox.information(self, "Info", "Conversion completed successfully!")
            self.conversion_number = self.conversion_number + 1
            self.output_path = os.path.join(self.log_dir, f"output_{self.conversion_number}.txt")

        except Exception as e:
            QMessageBox.critical(self, "Error", str(e))

    def plot(self):
        try:
            self.plot_widget.clear()
            for i in range(0, self.conversion_number):    
                data = np.loadtxt(f"Log/output_{i}.txt", delimiter="\t")
                x = data[:, 0]
                y = data[:, 1]

                colors = ['b', 'r', 'g', 'c', 'm', 'y', 'k', 'w']
                pen = pg.mkPen(color=colors[i % len(colors)], width=2)
                self.plot_widget.plot(x, y, pen=pen, name=f"output_{i}")
        except Exception as e:
            QMessageBox.critical(self, "Error", str(e))
    
    def clear_log(self):
        self.conversion_number = 0
        if os.path.exists(self.log_dir):
            shutil.rmtree(self.log_dir)
        os.makedirs(self.log_dir, exist_ok=True)
        QMessageBox.information(self, "Info", "Log folder cleared!")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec())
