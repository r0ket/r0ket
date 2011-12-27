import bridge
import packets
import time
import Queue
import random
import threading

class Player():
    def __init__(self, id):
        self.id = id
        self.nick = 'anonymous'
        self.timeout = 10
        self.active = False

# set shortpackets=True if you want to announce a game to be read with OpenBeacon readers
class Game:
    def __init__(self, device, gameName, gameChannel, announcechannel, announcemac, maxplayer=0, askname=False, shortpackets=False):
        self.gameName = gameName
        self.channel = gameChannel
        self.gamemac = [int(random.random()*254) for x in range(1,6)]
        self.playermac = list(self.gamemac)
        self.playermac[4]+=1
        self.gameid = int(random.random()*(2**15))
        
        flags = 0
        if maxplayer == 0:
            flags = 1
        if shortpackets:
            flags += 2
        self.bridge = bridge.Bridge(device, self.channel, self.gamemac)
        self.announce = packets.Announce(self.gamemac, self.channel,
                                            self.gameid, flags, self.gameName)

        self.announcequeue = Queue.Queue()
        self.bridge.registerQueue(self.announcequeue)
        self.announcechannel = announcechannel
        self.announcemac = announcemac
        self.askname = askname

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
            for id in self.players:
                player = self.players[id]
                player.timeout-=1
                if player.timeout == 0:
                    toremove.append(id)
            for id in toremove:
                player = self.players[id]
                if self.askname:
                    print "removing player", player.nick
                else:
                    print "removing player", id

                del self.players[id]
                if player.active:
                    player.active = False
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
                self.players[packet.id] = Player(packet.id)

            ack = packets.Ack(packet.id, packet.ctr, flags)
            qp = bridge.QueuePacket(
                        self.channel, self.playermac, False, ack)
            self.bridge.putInQueue(self.queue, qp) 
        elif packet.id in self.players:
            print "player known:", packet.id
            player = self.players[packet.id]
            player.timeout = 10
            if not player.active and isinstance(packet, packets.Button):
                if self.askname:
                    nickrequest = packets.Nickrequest(packet.id)
                    qp = bridge.QueuePacket(self.channel,
                        self.playermac, False, nickrequest)
                    self.bridge.putInQueue(self.queue, qp) 
                else:
                    player.active = True
                    for callback in self.callbacks:
                        callback("added", player)
            elif not player.active and isinstance(packet, packets.Nick):
                if self.askname:
                    player.nick = packet.nick
                    player.active = True
                    for callback in self.callbacks:
                        callback("added", player)
        else:
            print "player unknown"

    def sendAnnounce(self):
        aq = bridge.QueuePacket(self.announcechannel,
                                self.announcemac, False, self.announce)
        self.bridge.putInQueue(self.announcequeue, aq)
        self.announcetimer = threading.Timer(1, self.sendAnnounce)
        self.announcetimer.start()

    def registerPlayerCallback(self, callback):
        if not callback in self.callbacks:
            self.callbacks.append(callback)
