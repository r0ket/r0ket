import serialinterface
import thread
import threading
import Queue
import crcmod

class r0ket:
    def __init__(self, path2device):
        self.ser = serialinterface.SerialInterface(path2device, 115200, 0)
        self.free = threading.Lock()
        self.packets = Queue.Queue()
        thread.start_new_thread(self.readerThread,())
        self.setPacketLength(0x20)
        self.crc = crcmod.predefined.mkCrcFun('crc-ccitt-false')

    def writeCommand(self, command, data):
        crc = self.crc(data)
        data += chr(crc>>8);
        data += chr(crc&0xFF);
        self.free.acquire()
        print 'sending command:', command, 'len:', len(data), 'data:', list(data)
        self.ser.writeMessage(command,data);

    def readerThread(self):
        while True:
            try:
                (command, data) = self.ser.readMessage()
                if command == '1':
                    self.newPacket(data)
                elif command == '2':
                    self.free.release()
                elif command:
                    while True:
                        pass
            except Exception as e:
                print e

    def newPacket(self, data):
        print "received:", list(data)
        self.packets.put(data)

    def gotPacket(self):
        return not self.packets.empty()

    def getPacket(self):
        return self.packets.get()

    def sendPacket(self, packet):
        self.writeCommand('1', packet)

    def setPacketLength(self, length):
        self.free.acquire()
        self.ser.writeMessage('6', '%c'%length)
