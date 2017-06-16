import os.path
import sys

import array
from time import sleep

from PyQt5 import QtGui, uic
from PyQt5.QtCore import QModelIndex, QIODevice, QTextStream
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtWidgets import QDialog, QApplication


class SerialConnection():

    def __init__(self, parent=None):
        self.parent = parent


    def connect(self, serialPort="ttyACM0"):
        availablePortNames=self.getAvailablePortNames()
        print("available ports: {}".format(availablePortNames))

        serialPortSystemLocation = None
        for serialPortSystemLocation in availablePortNames:
            if serialPort not in serialPortSystemLocation:
                serialPortSystemLocation = None
            else:
                print("found serial port {}".format(serialPortSystemLocation))
                break

        if serialPortSystemLocation == None:
            self.parent.onPortDisconnected()
            return

        print("connecting to {}".format(serialPortSystemLocation))
        self.serial = QSerialPort(serialPortSystemLocation)
        self.serial.setBaudRate(self.serial.Baud9600)
        self.serial.open(QIODevice.ReadWrite)
        self.serial.readyRead.connect(self.__read)

        if self.serial.isOpen():
            parity = {0:"N", 1:"Y"}[self.serial.parity()]
            self.parent.log("{} connected {}{}{}".format(serialPortSystemLocation, self.serial.baudRate(), parity, self.serial.stopBits()))
            self.parent.onPortConnected()
        else:
            self.parent.log("failed to connect {}".format(serialPortSystemLocation))
            self.parent.onPortDisconnected()

    def disconnect(self):
        self.serial.close()
        self.serial.readyRead.disconnect(self.__read)
        self.parent.log("{} disconnected".format(self.serial.portName()))


    def __read(self):
        data = self.serial.readAll()
        self.parent.consumeData(data)


    def write(self, data):
        for line in data.splitlines():
            bytes = bytearray()
            bytes.extend(map(ord, line + "\n"))
            self.serial.write(bytes)
            self.parent.log("sending: {}".format(bytes))

    def getAvailablePortNames(self):
        return [p.systemLocation() for p in QSerialPortInfo.availablePorts()]

    def printAvailablePorts(self):
        ports = QSerialPortInfo.availablePorts()
        for port in ports:
            pid = port.productIdentifier()
            vid = port.vendorIdentifier()
            print("{} product id: {} vendor id:{}".format(port.systemLocation(), port.productIdentifier(), port.vendorIdentifier()))



class CommandEditor(QDialog):

    def __init__(self, *args):
        QDialog.__init__(self)

        self.serial = SerialConnection(parent=self)

        basepath = os.path.dirname(__file__)
        uiFilePath = os.path.abspath(os.path.join(basepath, "./ui/command-editor.ui"))
        self.ui = uic.loadUi(uiFilePath)
        self.__refreshSerialPorts()

        self.ui.show()
        self.ui.twCommandTemplateTree.doubleClicked[QModelIndex].connect(self.__appendCommandToTextField)
        self.ui.cbConnect.clicked[bool].connect(self.__connectSerial)
        self.ui.pbSend.clicked[bool].connect(self.__sendCommands)
        self.ui.tbRefresh.clicked[bool].connect(self.__refreshSerialPorts)


    def __refreshSerialPorts(self):
        self.log("available ports {}".format(self.serial.getAvailablePortNames()))
        self.ui.cbSerialPortName.clear()
        self.ui.cbSerialPortName.addItems(self.serial.getAvailablePortNames())

        if self.ui.cbSerialPortName.count() <= 0:
            self.ui.cbConnect.setEnabled(False)
        else:
            self.ui.cbConnect.setEnabled(True)


    def onPortConnected(self):
        self.ui.cbConnect.setChecked(True)

    def onPortDisconnected(self):
        self.ui.cbConnect.setChecked(False)


    def consumeData(self, inData):
        self.ui.pteReception.appendPlainText(QTextStream(inData).readAll())

    def __sendCommands(self):
        self.serial.write(self.ui.pteCommands.toPlainText())


    def __appendCommandToTextField(self, index):
        index # type: QModelIndex
        getSelected = self.ui.twCommandTemplateTree.selectedItems() # type: PyQt4.QtGui.QTreeWidgetItem
        if getSelected:
            baseNode = getSelected[0]

            parents = []
            parent = baseNode
            while parent is not None:
                parents.append(parent.text(0))
                parent = parent.parent()

            text = ".".join(reversed(parents)) + " " + " " + baseNode.text(1)
            self.ui.pteCommands.appendPlainText(text.strip())


    def __connectSerial(self, isChecked):
        if isChecked:
            self.serial.connect(self.ui.cbSerialPortName.currentText())
        else:
            self.serial.disconnect()


    def log(self, message):
        self.ui.teSerialLog.appendPlainText(message)
        print(message)

    def sendCommands(self, isChecked):
        pass # todo

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = CommandEditor()
    sys.exit(app.exec_())
