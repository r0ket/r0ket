import serialinterface
import threading
import Queue
import crcmod
import packets

class QueuePacket:
    def __init__(self, channel, mac, acked, packet):
        self.channel = channel
        self.mac = mac
        self.acked = acked
        self.packet = packet
        self.priority = 5
        self.retriesleft = 5
        self.timeout = 0.1
        self.timer = None
        self.timedout = False
        self.lock = threading.RLock()
        self.isdone = False

    def __cmp__(self, other):
        if not isinstance(other,QueuePacket):
            return 1
        if self.priority < other.priority:
            return -1
        if self.priority > other.priority:
            return 1
        return 0

    def valid(self):
        with self.lock:
            return self.retriesleft > 0 and not self.acked

    def sent(self, timeoutcallback):
        with self.lock:
            self.timedout = False
            if self.acked:
                self.timeoutcallback = timeoutcallback
                self.timer = threading.Timer(self.timeout, self.timercallback)
                self.timer.start()

    def done(self):
        with self.lock:
            if self.timer != None:
                self.timer.cancel()
                self.timer = None
            self.isdone = True

    def timercallback(self):
        with self.lock:
            self.timedout = True
            self.timeoutcallback(self)

class Bridge:
    def __init__(self, path2device):
        self.ser = serialinterface.SerialInterface(path2device, 115200, 0)
        self.free = threading.Lock()
        self.queueslock = threading.Lock()
        self.packets = Queue.PriorityQueue()
        self.outpackets = Queue.Queue()
        self.crc = crcmod.predefined.mkCrcFun('crc-ccitt-false')
        self.queues = {}
        
        self.reader = threading.Thread(target = self.readerThread)
        self.reader.daemon = True
        
        self.writer = threading.Thread(target = self.writerThread)
        self.writer.daemon = True
        
        self.writer.start()
        self.reader.start()

        self.setPacketLength(0x20)
        self.setTxMAC((1,2,3,2,1))
        self.setRxMAC((1,2,3,2,1))
        self.setChannel(81)

    def registerQueue(self, queue):
        if queue not in self.queues:
            self.queues[queue] = None

    def putInQueue(self, queue, qp):
        if queue in self.queues:
            queue.put(qp);
        self.checkQueues()

    def processAck(self, ack):
        #find the corresponding packet in the queues
        found = False
        for pq in self.queues.values():
            if pq.packet.id == ack.id and pq.packet.ctr == ack.ctr:
                #notify it
                pq.done()
                found = True
        #notify the queue system
        if found:
            self.checkQueues()
        else:
            print "got an ack for an unknown packet"
    
    def packetTimeout(self, qp):
        self.checkQueues()

    def checkQueues(self):
        with self.queueslock:
            for q in self.queues:
                #check if a packet has to be resent
                #remove it from the packet slot if it has been resent to often
                qp = self.queues[q]
                if qp != None:
                    if qp.valid():
                        self.queues[q] = None
                    elif qp.timedout:
                        print "packet timed out" + qp.packet
                        self.outpackets.put(qp)
                #check if a idle queue has a new packet in line
                qp = self.queues[q]
                if qp == None and not q.empty():
                    qp = q.get()
                    self.queues[q] = qp
                    self.outpackets.put(qp)

    def writerThread(self):
        while True:
            try:
                #wait until we have packets to take care of
                qp = self.outpackets.get()
                #send it and notify the queuepacket
                self.sendPacket(qp.packet)
                qp.sent(self.packetTimeout)
            except Exception as e:
                print e

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
        #print "received:", list(data)
        crc = self.crc(data[:-2])
        if data[-2:] == chr(crc>>8) + chr(crc&0xFF):
            packet = packets.fromMessage(data)
            print "received:", packet
            if isinstance(packet,packets.Ack):
                self.ProcessAck(packet)
            else:
                self.packets.put(packet)

    def gotPacket(self):
        return not self.packets.empty()

    def getPacket(self):
        return self.packets.get()

    def sendPacket(self, packet):
        print 'sending', packet
        data = packet.toMessage()
        crc = self.crc(data)
        data += chr(crc>>8);
        data += chr(crc&0xFF);
        self.free.acquire()
        #print 'sending packet: len:', len(data), 'data:', list(data)
        self.ser.writeMessage('1',data);

    def setPacketLength(self, length):
        self.free.acquire()
        self.ser.writeMessage('6', '%c'%length)
    
    def setTxMAC(self, mac):
        self.free.acquire()
        self.ser.writeMessage('3', ''.join([chr(x) for x in mac]))
    
    def setRxMAC(self, mac):
        self.free.acquire()
        self.ser.writeMessage('4', ''.join([chr(x) for x in mac]))

    def setChannel(self, channel):
        self.free.acquire()
        self.ser.writeMessage('5', '%c'%channel)

