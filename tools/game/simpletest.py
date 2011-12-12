import r0ketrem0te.rem0te
import time
r = r0ketrem0te.rem0te.r0ket('/dev/ttyACM0')

while True:
    r.sendPacket("\x20GA\x00\x00\x00\x00\x01\x02\x03\x04\x01\x02\x03\x01\x02\x51\x01\x02\x03\x04\x00test\x00\x06\x07\x08")
    time.sleep(1)
    #packet = r.getPacket()
    #print list(packet)
    #r.sendPacket('12345678901234567890123456789012')

