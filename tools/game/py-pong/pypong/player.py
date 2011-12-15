import pygame, random
import r0ketrem0te.game
import r0ketrem0te.bridge
import r0ketrem0te.packets
import time
import Queue
import threading

class Rem0te(object):
    def __init__(self):
        self.maxplayer = 1
        self.players = {}
        self.game = r0ketrem0te.game.Game('/dev/ttyACM0', "pong", 83, 81, (1,2,3,2,1))

        self.queue = Queue.Queue()
        self.game.bridge.registerQueue(self.queue)
        self.game.bridge.registerCallback(self.receivedPacket)
        self.state = 0
        self.checkPlayers()

    def checkPlayers(self):
        toremove = []
        for player in self.players:
            self.players[player]-=1
            if self.players[player] == 0:
                toremove.append(player)
        for player in toremove:
            print "removing player", player
            del self.players[player]
        self.timer = threading.Timer(1, self.checkPlayers)
        self.timer.start()

    def receivedPacket(self, packet):
        if isinstance(packet, r0ketrem0te.packets.Join):
            # flags = 1: join ok
            # flags = 0: join not ok
            flags = 0
            if len(self.players) < self.maxplayer:
                flags = 1
                self.players[packet.id] = 10
            ack = r0ketrem0te.packets.Ack(packet.id, packet.ctr, flags)
            qp = r0ketrem0te.bridge.QueuePacket(
                        self.game.channel, self.game.playermac, False, ack)
            self.game.bridge.putInQueue(self.queue, qp) 
        elif packet.id in self.players:
            self.players[packet.id] = 10
            if isinstance(packet, r0ketrem0te.packets.Button):
                self.state = packet.button
 
    def update(self, paddle, game):
        if self.state == 1:
            paddle.direction = -1
        elif self.state == 2:
            paddle.direction = 1
        else:
            paddle.direction = 0

    def hit(self):
        pass

    def lost(self):
        pass
        
    def won(self):
        pass
        
class BasicAIPlayer(object):
    def __init__(self):
        self.bias = random.random() - 0.5
        self.hit_count = 0
        
    def update(self, paddle, game):
        # Dead simple AI, waits until the ball is on its side of the screen then moves the paddle to intercept.
        # A bias is used to decide which edge of the paddle is going to be favored.
        if (paddle.rect.x < game.bounds.centerx and game.ball.rect.x < game.bounds.centerx) or (paddle.rect.x > game.bounds.centerx and game.ball.rect.x > game.bounds.centerx):
            delta = (paddle.rect.centery + self.bias * paddle.rect.height) - game.ball.rect.centery 
            if abs(delta) > paddle.velocity:
                if delta > 0:
                    paddle.direction = -1
                else:
                    paddle.direction = 1
            else:
                paddle.direction = 0
        else:
            paddle.direction = 0

    def hit(self):
        self.hit_count += 1
        if self.hit_count > 6:
            self.bias = random.random() - 0.5 # Recalculate our bias, this game is going on forever
            self.hit_count = 0
            
    def lost(self):
        # If we lose, randomise the bias again
        self.bias = random.random() - 0.5
        
    def won(self):
        pass
        
class KeyboardPlayer(object):
    def __init__(self, input_state, up_key=None, down_key=None):
        self.input_state = input_state
        self.up_key = up_key
        self.down_key = down_key
        
    def update(self, paddle, game):
        if self.input_state['key'][self.up_key]:
            paddle.direction = -1
        elif self.input_state['key'][self.down_key]:
            paddle.direction = 1
        else:
            paddle.direction = 0

    def hit(self):
        pass

    def lost(self):
        pass
        
    def won(self):
        pass
        
class MousePlayer(object):
    def __init__(self, input_state):
        self.input_state = input_state
        pygame.mouse.set_visible(False)
        
    def update(self, paddle, game):
        centery = paddle.rect.centery/int(paddle.velocity)
        mousey = self.input_state['mouse'][1]/int(paddle.velocity)
        if centery > mousey:
            paddle.direction = -1
        elif centery < mousey:
            paddle.direction = 1
        else:
            paddle.direction = 0

    def hit(self):
        pass

    def lost(self):
        pass

    def won(self):
        pass
