import bridge
import packets
import time
import Queue
import random
import threading

class Game:
    def __init__(self, device, gameName, gameChannel, announcechannel, announcemac, maxplayer=0):
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

        self.maxplayer = maxplayer
        self.players = {}
        self.callbacks = []
        self.queue = Queue.Queue()
        self.bridge.registerQueue(self.queue)
        self.bridge.registerCallback(self.receivedPacket)
        self.checkPlayers()

    def checkPlayers(self):
        if self.maxplayer > 0:
            toremove = []
            for player in self.players:
                self.players[player]-=1
                if self.players[player] == 0:
                    toremove.append(player)
            for player in toremove:
                print "removing player", player
                del self.players[player]
                for callback in self.callbacks:
                    callback("removed", player)
        self.timer = threading.Timer(1, self.checkPlayers)
        self.timer.start()

    def receivedPacket(self, packet):
        if self.maxplayer == 0:
            return
        if isinstance(packet, packets.Join):
            # flags = 1: join ok
            # flags = 0: join not ok
            flags = 0
            if len(self.players) < self.maxplayer:
                flags = 1
                self.players[packet.id] = 10
                for callback in self.callbacks:
                    callback("added", packet.id)

            ack = packets.Ack(packet.id, packet.ctr, flags)
            qp = bridge.QueuePacket(
                        self.channel, self.playermac, False, ack)
            self.bridge.putInQueue(self.queue, qp) 
        elif packet.id in self.players:
            self.players[packet.id] = 10

    def sendAnnounce(self):
        aq = bridge.QueuePacket(self.announcechannel,
                                self.announcemac, False, self.announce)
        self.bridge.putInQueue(self.announcequeue, aq)
        self.announcetimer = threading.Timer(1, self.sendAnnounce)
        self.announcetimer.start()

    def registerPlayerCallback(self, callback):
        if not callback in self.callbacks:
            self.callbacks.append(callback)
