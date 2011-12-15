import r0ketrem0te.game
import r0ketrem0te.bridge
import r0ketrem0te.packets
import time
import Queue

maxplayer = 2
players = {}

def receivedPacket(packet):
    if isinstance(packet, r0ketrem0te.packets.Join):
        # flags = 1: join ok
        # flags = 0: join not ok
        flags = 0
        if len(players) < maxplayer:
            flags = 1
            players[packet.id] = 10
        ack = r0ketrem0te.packets.Ack(packet.id, packet.ctr, flags)
        qp = r0ketrem0te.bridge.QueuePacket(game.channel, game.playermac, False, ack)
        game.bridge.putInQueue(queue, qp) 
    elif packet.id in players:
        players[packet.id] = 10
        
game = r0ketrem0te.game.Game('/dev/ttyACM0', "testgame", 83, 81, (1,2,3,2,1))

queue = Queue.Queue()
game.bridge.registerQueue(queue)
game.bridge.registerCallback(receivedPacket)

while True:
    time.sleep(1)
    toremove = []
    for player in players:
        players[player]-=1
        if players[player] == 0:
            toremove.append(player)
    for player in toremove:
        print "removing player", player
        del players[player]
