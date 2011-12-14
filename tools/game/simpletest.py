import r0ketrem0te.rem0te
import r0ketrem0te.packets
import time
import Queue

announcequeue = Queue.Queue()
r = r0ketrem0te.rem0te.Bridge('/dev/ttyACM0')

r.registerQueue(announcequeue)

a = r0ketrem0te.packets.Announce(0,2,(1,2,3,2,1), 81, 1, 0, "testgame")
aq = r0ketrem0te.rem0te.QueuePacket(81, (1,2,3,2,1), False, a)
aq.priority = 4

while True:
    r.putInQueue(announcequeue, aq)
    for i in range(1,1000):
        if r.gotPacket():
            packet = r.getPacket()
            if isinstance(packet, r0ketrem0te.packets.Join):
                r.sendPacket(r0ketrem0te.packets.Ack(packet.id, packet.ctr, 1))
        time.sleep(.001)





