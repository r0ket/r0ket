import r0ketrem0te.game
import r0ketrem0te.bridge
import r0ketrem0te.packets
import time
import Queue

def receivedPacket(packet):
       pass 
game = r0ketrem0te.game.Game('/dev/ttyACM0', "testgame", 83, 81, (1,2,3,2,1), 2)

queue = Queue.Queue()
game.bridge.registerQueue(queue)
game.bridge.registerCallback(receivedPacket)

while True:
    time.sleep(1)
