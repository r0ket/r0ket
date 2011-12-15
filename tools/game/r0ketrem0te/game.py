import bridge
import packets
import time
import Queue
import random
import threading

class Game:
    def __init__(self, device, gameName, gameChannel, announcechannel, announcemac):
        self.gameName = gameName
        self.channel = gameChannel
        self.gamemac = [int(random.random()*254) for x in range(1,6)]
        self.playermac = list(self.gamemac)
        self.playermac[4]+=1
        self.gameid = int(random.random()*(2**31))
        
        self.bridge = bridge.Bridge(device, self.channel, self.gamemac)
        self.announce = packets.Announce(self.gamemac, self.channel,
                                            self.gameid, 0, "testgame")

        self.announcequeue = Queue.Queue()
        self.bridge.registerQueue(self.announcequeue)
        self.announcechannel = announcechannel
        self.announcemac = announcemac

        self.sendAnnounce()

    def sendAnnounce(self):
        aq = bridge.QueuePacket(self.announcechannel,
                                self.announcemac, False, self.announce)
        self.bridge.putInQueue(self.announcequeue, aq)
        self.announcetimer = threading.Timer(1, self.sendAnnounce)
        self.announcetimer.start()

