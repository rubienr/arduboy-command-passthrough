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


    def connect(self):
        availablePortNames=self.getAvailablePortNames()
        print("available ports: {}".format(availablePortNames))
        serialPort="ttyACM0"
        print("connecting to {}...".format(serialPort))
        if serialPort not in availablePortNames:
          self.parent.onPortDisconnected()
          return

        serialPort="/dev/{}".format(serialPort)
        print("connected to {}".format(serialPort))
        self.serial = QSerialPort(serialPort)
        self.serial.setBaudRate(self.serial.Baud9600)
        self.serial.open(QIODevice.ReadWrite)

        self.serial.readyRead.connect(self.__read)

        if self.serial.isOpen():
          self.parent.onPortConnected()
        else:
          self.parent.onPortDisconnected()

    def disconnect(self):
        self.serial.close()
        self.serial.readyRead.disconnect(self.__read)

    def __read(self):
        data = self.serial.readAll()
        self.parent.consumeData(data)


    def write(self, data):
        for line in data.splitlines():
            bytes = bytearray()
            bytes.extend(map(ord, line + "\n"))
            self.serial.write(bytes)
            print("sending: {}".format(bytes))

    def getAvailablePortNames(self):
        return [p.portName() for p in QSerialPortInfo.availablePorts()]

    def printAvailablePorts(self):
        ports = QSerialPortInfo.availablePorts()
        for port in ports:
            pid = port.productIdentifier()
            vid = port.vendorIdentifier()
            print("{} product id: {} vendor id:{}".format(port.portName(), port.productIdentifier(), port.vendorIdentifier()))



class CommandEditor(QDialog):

    def __init__(self, *args):
        QDialog.__init__(self)

        self.ui = uic.loadUi("./ui/command-editor.ui")
        self.ui.show()
        self.ui.twCommandTemplateTree.doubleClicked[QModelIndex].connect(self.__appendCommandToTextField)
        self.ui.cbConnect.clicked[bool].connect(self.__connectSerial)
        self.ui.pbSend.clicked[bool].connect(self.__sendCommands)

        self.serial = SerialConnection(parent=self)
        self.serial.printAvailablePorts()

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
            self.serial.connect()
        else:
            self.serial.disconnect()



    def sendCommands(self, isChecked):
        pass # todo

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = CommandEditor()
    sys.exit(app.exec_())
