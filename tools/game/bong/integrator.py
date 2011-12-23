import r0ketrem0te.game
import r0ketrem0te.bridge
import r0ketrem0te.packets
import time
import Queue
import tornado.escape
import tornado.ioloop
import tornado.options
import tornado.web
import tornado.websocket
import os.path
import logging
import threading

from tornado.options import define, options

define("port", default=8888, help="run on the given port", type=int)

pong=None
pos=0.5

def sendpos():
    if pong:
        pong.write_message("{ \"right\": "+str(pos)+" }")
    threading.Timer(.5,sendpos).start()

sendpos()

def receivedPacket(packet):
    global pos
    if isinstance(packet,r0ketrem0te.packets.Button):
        if packet.button == 2:
            pos+=.01
        if packet.button == 1:
            pos-=.01
        if pos <0:
            pos=0
        if pos >1:
            pos=1

game = r0ketrem0te.game.Game('COM4', "testgame", 83,
                           87, [ord(x) for x in 'REM0T'], 2, True)
game.bridge.registerCallback(receivedPacket)

class Application(tornado.web.Application):
    def __init__(self):
        handlers = [
            (r"/data", DataSocketHandler),
        ]
        settings = dict(
            cookie_secret="43oETzKXQAGaYdkL5gEmGeJJFuYh7EQnp2XdTP1o/Vo=",
            template_path=os.path.join(os.path.dirname(__file__), "templates"),
            static_path=os.path.join(os.path.dirname(__file__), "static"),
            xsrf_cookies=True,
            autoescape=None,
        )
        tornado.web.Application.__init__(self, handlers, **settings)

class DataSocketHandler(tornado.websocket.WebSocketHandler):
    waiters = set()
    def open(self):
        global pong
        print "WebSocket: Open"
        DataSocketHandler.waiters.add(self)
        pong=self

    def on_close(self):
        print "WebSocket: Close"
        DataSocketHandler.waiters.remove(self)

    def on_message(self, message):
        print "Got message: ",message

tornado.options.parse_command_line()
app = Application()
app.listen(options.port,'127.0.0.1')
tornado.ioloop.IOLoop.instance().start()
