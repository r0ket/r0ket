def inttouint32(v):
    return chr(v&0xff)+chr((v>>8)&0xff)+chr((v>>16)&0xff)+chr((v>>24)&0xff)

def uint32toint(v):
    return (ord(v[3])<< 24) + (ord(v[2])<<16) + (ord(v[1])<<8) + (ord(v[0]))

def inttouint16(v):
    return chr(v&0xff)+chr((v>>8)&0xff)

class Packet:
    def __init__(self, command, id=None):
        self.ctr = 0
        if id == None:
            message = command
            command = message[2]
            id = uint32toint(message[3:7])
            self.ctr = uint32toint(message[7:11])
        self.length = 32
        self.protocol = 'G'
        self.command = command
        self.id = id
        self.priority = 5

    def toMessage(self):
        message = chr(self.length)
        message += self.protocol
        message += self.command
        message += inttouint32(self.id)
        message += inttouint32(self.ctr)
        return message

    def headerString(self):
        return "id=%d, ctr=%d"%(self.id, self.ctr)

    def __str__(self):
        s = "Packet with protocol=" + self.protocol
        s += ", command=" + self.command
        s += ", "+  self.headerString()
        return s

class Button(Packet):
    def __init__(self, id, button=None):
        if button!= None:
            Packet.__init__(self, 'B', id)
        else:
            message = id
            Packet.__init__(self, message)
            button = ord(message[11])
        self.button = button

    def toMessage(self):
        base = Packet.toMessage(self)
        return base + chr(self.button) + '\x00'*18

    def __str__(self):
        s = "Button packet with " + self.headerString()
        s += ", button=%d"%self.button
        return s

class Announce(Packet):
    def __init__(self, gameMac, gameChannel, gameId, gameFlags, gameTitle, interval=5, jitter=10):
        #always a broadcast
        Packet.__init__(self, 'A', 0)
        self.gameMac = gameMac
        self.gameChannel = gameChannel
        self.gameId = gameId
        self.gameFlags = gameFlags
        self.gameTitle = gameTitle[0:8]
        self.interval = interval
        self.jitter = jitter
        self.priority = 3

    def toMessage(self):
        message = Packet.toMessage(self)
        message += ''.join([chr(x) for x in self.gameMac])
        message += chr(self.gameChannel)
        message += inttouint16(self.gameId)
        message += chr(self.gameFlags)
        message += chr(self.interval)
        message += chr(self.jitter)
        message += self.gameTitle
        if len(self.gameTitle) < 8:
            message += '\x00'*(8-len(self.gameTitle))
        return message
 
    def __str__(self):
        s = "Announce packet with " + self.headerString()
        s += ", gameMac="+str(self.gameMac)
        s += ", gameChannel=%d"%self.gameChannel
        s += ", gameId=%d"%self.gameId
        s += ", gameFlags=%d"%self.gameFlags
        s += ", interval=%d"%self.interval
        s += ", jitter=%d"%self.jitter
        s += ", gameTitle="+self.gameTitle
        return s
   
class Join(Packet):
    def __init__(self, id, gameId=None):
        if gameId != None:
            Packet.__init__(self, 'J', id)
        else:
            message = id
            Packet.__init__(self, message)
            gameId = uint32toint(message[11:15])
        self.gameId = gameId

    def toMessage(self):
        message = Packet.toMessage(self)
        message += inttouint32(self.gameId)
        message += '\x00'*15
        return message

    def __str__(self):
        s = "Join packet with " + self.headerString()
        s += ", gameId=%d"%self.gameId
        return s
 
class Ack(Packet):
    def __init__(self, id, ctr, flags=None):
        if ctr != None and flags != None:
            Packet.__init__(self, 'a', id)
            self.ctr = ctr
        else:
            message = id
            Packet.__init__(self, message)
            flags = ord(message[11])
        self.flags = flags
        self.priority = 3

    def toMessage(self):
        message = Packet.toMessage(self)
        message += chr(self.flags)
        message += '\x00'*18
        return message

    def __str__(self):
        s = "Ack packet with " + self.headerString()
        s += ", flags=%d"%self.flags
        return s
 
class Nickrequest(Packet):
    def __init__(self, id):
        Packet.__init__(self, 'N', id)

    def __str__(self):
        s = "Nickrequest packet with " + self.headerString()
        return s

    def toMessage(self):
        message = Packet.toMessage(self)
        message += '\x00'*19
        return message

class Nick(Packet):
    def __init__(self, id, flags=None, nick=None):
        if flags != None and nick != None:
            Packet.__init__(self, 'n', id)
        else:
            message = id
            Packet.__init__(self, message)
            flags = ord(message[11])
            nick = message[12:30].rstrip(' \t\r\n\0')
        self.flags = flags
        self.nick = nick
    
    def toMessage(self):
        message = Packet.toMessage(self)
        message += chr(self.flags)
        message += self.nick
        if len(self.nick) < 18:
            message += '\x00'*(18-len(self.nick))
        return message

    def __str__(self):
        s = "Nick packet with " + self.headerString()
        s += ", flags=%d"%self.flags
        s += ", nick="+self.nick
        return s
 
class Text(Packet):
    def __init__(self, id, x, y, flags, text):
        Packet.__init__(self, 'T', id)
        self.x = x
        self.y = y
        self. flags = flags
        self.text = text[0:16]
    
    def toMessage(self):
        message = Packet.toMessage(self)
        message += chr(self.x)
        message += chr(self.y)
        message += chr(self.flags)
        message += self.text
        if len(self.text) < 16:
            message += '\x00'*(16-len(self.text))
        return message
        
    def __str__(self):
        s = "Text packet with " + self.headerString()
        s += ", x=%d"%self.x
        s += ", y=%d"%self.y
        s += ", flags=%d"%self.flags
        s += ", text="+self.text
        return s
 
def fromMessage(message):
    if len(message) >= 30 and ord(message[0]) == 32 and message[1] == 'G':
        if message[2] == 'B':
            return Button(message)
        if message[2] == 'n':
            return Nick(message)
        if message[2] == 'J':
            return Join(message)
        if message[2] == 'a':
            return Ack(message)
    return None
