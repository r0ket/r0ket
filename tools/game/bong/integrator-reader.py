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
import socket
import time

from tornado.options import define, options

define("port", default=8888, help="run on the given port", type=int)

pong=None
left=0.0
right=0.0
cntr=0
cntl=0
info={}

def sendpos():
    global right, left
    global cntr, cntl
    global info
    sstr="{ \"right\": %s, \"cntr\": %s , \"cntl\": %s, \"left\": %s }"%(right,cntr,cntl,left)
    if pong:
        pong.write_message(sstr)
#    	print sstr
    threading.Timer(.1,sendpos).start()
    for (id,(b,t,r)) in info.items():
	if(t+5<time.time()):
#	    print "time=%s"%(time.time())
	    del info[id]
    sumr=0.0
    suml=0.0
    cntr=0.0
    cntl=0.0
    for (id,(b,t,r)) in info.items():
	if r==1:
	    cntr+=1
	else:
	    cntl+=1
    	if b&1==1:
	    if r==1:
		sumr-=1
	    else:
		suml-=1
    	if b&2==2:
	    if r==1:
		sumr+=1
	    else:
		suml+=1
#	print "summing: suml=%s cntl=%s sumr=%s cntr=%s"%(suml,cntl,sumr,cntr)
    if cntr==0:
        right=0
    else:
        right=sumr/cntr
    if cntl==0:
        left=0
    else:
        left=suml/cntl
#    print "right=%s left=%s"%(right,left)

sendpos()

def workPacket(data, addr):
    global right, left
    global cntr, cntl
    global reid
    global r0id
    global button
    global info
#    print "new packet:", list(data), addr
    reid=ord(data[4])*256 + ord(data[5])
    r0id=ord(data[19])*256*256*256 + ord(data[20])*256*256+ord(data[21])*256+ord(data[22])
    button=ord(data[27])
    rl=1
    if reid == 1123:
    	rl=2
    info[r0id]=(button,time.time(),rl)
    print "added rl=%s r0=%s bu=%s"%(rl,r0id,button)

def readerThread():
    sock = socket.socket( socket.AF_INET, # Internet
                           socket.SOCK_DGRAM ) # UDP
    sock.bind(("0.0.0.0", 2342))

    while 1:
        data, addr = sock.recvfrom(1024)
        workPacket(data, addr)
        
reader = threading.Thread(target = readerThread)
reader.daemon = True

reader.start()

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
