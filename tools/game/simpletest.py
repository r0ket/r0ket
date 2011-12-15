import r0ketrem0te.game
import r0ketrem0te.bridge
import r0ketrem0te.packets
import time
import Queue

def receivedPacket(packet):
    if isinstance(packet, r0ketrem0te.packets.Join):
        # flags = 1: join ok
        # flags = 0: join not ok
        ack = r0ketrem0te.packets.Ack(packet.id, packet.ctr, 1)
        qp = r0ketrem0te.bridge.QueuePacket(game.channel, game.playermac, False, ack)
        game.bridge.putInQueue(queue, qp) 

game = r0ketrem0te.game.Game('/dev/ttyACM0', "testgame", 83, 81, (1,2,3,2,1))

queue = Queue.Queue()
game.bridge.registerQueue(queue)
game.bridge.registerCallback(receivedPacket)

while True:
    time.sleep(1)

