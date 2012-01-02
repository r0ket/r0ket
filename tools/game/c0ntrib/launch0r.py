#
# allows you to control the roketlauncher via the rem0te l0dable
# documented this in the wiki:
# - http://r0ket.badge.events.ccc.de/r0ketlauncher#launch0r
#
import r0ketrem0te.game
import time
import hid

class Launch0r:
    def __init__(self):
        self.rem0te = r0ketrem0te.game.Game('/dev/cu.usbmodem411', "Launch0r", 83, 87, [ord(x) for x in 'REM0T'], 1, False)
        self.rem0te.registerPlayerCallback(self.playercallback)
        self.player = Rem0tePlayer(self.rem0te)

    def playercallback(self, action, player):
        if action == 'added':
            if self.player == None:
                self.player.player = player
        elif action == 'removed':
            self.player = None

    def run(self):
        self.player.launcher = hid.device(0x0a81, 0x0701)
        self.player.launcher.set_nonblocking(1)

class Rem0tePlayer(object):
    def __init__(self, rem0te):
        self.rem0te = rem0te
        self.rem0te.bridge.registerCallback(self.receivedPacket)
        self.player = None
        self.launcher = None

    def receivedPacket(self, packet):
        if packet.button == 0:#off
            self.launcher.write([0x20, 0, 0])
        if packet.button == 4:#left
            self.launcher.write([0x04, 0, 0])
        if packet.button == 8:#right
            self.launcher.write([0x08, 0, 0])
        if packet.button == 1:#up
            self.launcher.write([0x02, 0, 0])
        if packet.button == 2:#down
            self.launcher.write([0x01, 0, 0])
        if packet.button == 16:#fire
            self.launcher.write([0x10, 0, 0])

if __name__ == '__main__':
    launch0r = Launch0r()
    launch0r.run()
