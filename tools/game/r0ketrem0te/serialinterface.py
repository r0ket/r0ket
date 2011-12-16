import serial
import string
import sys
import time

class SerialInterface:
    def  __init__ ( self, path2device, baudrate, timeout=0):
      self.portopen = False
      while not self.portopen:
        try:
            self.ser = serial.Serial(path2device, baudrate)
            self.path2device = path2device
            self.baudrate = baudrate
            self.timeout = timeout+1
            self.ser.flushInput()
            self.ser.flushOutput()
            if timeout:
                self.ser.setTimeout(timeout+1)
            self.portopen = True
        except serial.SerialException:
            print "exception while opening"
            pass
        time.sleep(1)
      #print "done"
    def close(self):
        try:
            self.portopen = False
            self.ser.close()
        except serial.SerialException:
            pass
    def reinit(self):
        self.close()
        print "reopening"
        while not self.portopen:
            self.__init__(self.path2device, self.baudrate, self.timeout)
            time.sleep(1)
        print "done"

    def writeMessage(self,command,message):
        enc = "\\"+ command + message.replace('\\','\\\\') + "\\0";
        #print 'writing %s' % list(enc)
        try:
            self.ser.write(enc)
        except :
            pass
            #self.reinit()

    def readMessage(self):
        data = ""
        escaped = False
        stop = False
        start = False
        inframe = False
        command = ''
        while True:
            starttime = time.time()
            c = self.ser.read(1)
            endtime = time.time()
            if len(c) == 0:             #A timout occured
                if endtime-starttime < self.timeout - 1:
                    print "port broken"
                    self.reinit()
                    raise Exception()
                else:
                    #print 'TIMEOUT'
                    return (False, '')
            if escaped:
                if c == '\\':
                    d = '\\'
                elif c == '0':
                    stop = True
                    inframe = False
                else:
                    start = True
                    inframe = True
                    command = c
                    data = ""
                escaped = False
            elif c == '\\':
                escaped = 1
            else:
                d = c
                
            if start and inframe:
                start = False
            elif stop:
                #print 'received message: len=%d data=%s'%(len(data),data)
                #print 'received message. command=',command, "data=" ,list(data)
                return (command, data)
            elif escaped == False and inframe:
                data += str(d)

