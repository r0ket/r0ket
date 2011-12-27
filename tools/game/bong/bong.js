// Initialisation
var socket = "ws://127.0.0.1:8888/data";
var field= { w: 320, h: 240 };
var paddle = { w: 2, h: 30 };
var ball = { w: 5, h: 5, s: 5 };

// Pong game class
function pong() {
	this.ball = {};
	this.left =  {};
	this.right =  {};
	this.score = {};
	this.score.left = {};
	this.score.right = {};
	this.cnt = {};
	this.cnt.left = {};
	this.cnt.right = {};
	this.field = {};
	this.game = { run: 0 };

	this.ball.size=ball;
	this.left.size=paddle;
	this.right.size=paddle;
	this.field.size=field;

	this.left.x=-field.w+3*this.left.size.w;
	this.right.x=field.w-3*this.right.size.w;

	this.ball.avatar=$('div#ball');
	this.left.avatar=$('div#paddle_left');
	this.right.avatar=$('div#paddle_right');
	this.score.left.avatar=$('div#score_left');
	this.score.right.avatar=$('div#score_right');
	this.field.avatar=$('#playfield');

	function setupavatar(obj){
		obj.avatar.width(2*obj.size.w)
			      .height(2*obj.size.h);
	};

	setupavatar(this.ball);
	setupavatar(this.left);
	setupavatar(this.right);
	setupavatar(this.field);

	this.cnt.left.avatar=$('div#player_left');
	this.cnt.right.avatar=$('div#player_right');

	$('div#player_left').css({top: 2*this.field.size.h-30});
	$('div#player_right').css({top: 2*this.field.size.h-30});

	this.field.avatar.focus();
	
	this.score.left.pts=0;
	this.score.right.pts=0;
	this.setup( (Math.random()>0.5)?1:-1);

	this.field.avatar.keydown(this,function(ev){
		if(ev.which == 67) ev.data.socketstart(socket);
		if(ev.which == 81) ev.data.game.run=0;
		if(ev.which == 82) ev.data.game.run=1;
		if(ev.which > 48 && ev.which <=57) ev.data.setleft((ev.which-53)/4);
	});

	$('#debug').css({left:0,top:2*this.field.size.h+10});
	$('#socket').css({left:0,top:2*this.field.size.h+30});
}

pong.prototype.setleft = function(where){
	this.left.y=where*(this.field.size.h-this.left.size.h);
};

pong.prototype.setright = function(where){
	this.right.y=where*(this.field.size.h-this.right.size.h);
};

pong.prototype.setup = function(rtl){
	this.ball.x=0;
	this.ball.y=0;
	this.ball.ang=Math.random()*20/180*Math.PI;
	if(rtl<0) this.ball.ang=Math.PI-this.ball.ang;
	this.ball.spd=this.ball.size.s;

	this.left.y=0;
	this.right.y=0;

	this.game.run=0;
};

pong.prototype.update= function(t){
	var upv=0;

	this.ball.vx=Math.cos(this.ball.ang)*this.ball.spd;
	this.ball.vy=Math.sin(this.ball.ang)*this.ball.spd;
	this.ball.x+=t*this.ball.vx;
	this.ball.y+=t*this.ball.vy;

	this.ball.x+=t*this.ball.vx;
	this.ball.y+=t*this.ball.vy;

	// Clipping
	if(this.ball.y-this.ball.size.h<-field.h){
		this.ball.ang=-this.ball.ang;
		this.ball.y=-field.h+this.ball.size.h;
	};

	if(this.ball.y+this.ball.size.h>field.h){
		this.ball.ang=-this.ball.ang;
		this.ball.y=field.h-this.ball.size.h;
	};

	function newang(off,bath){
		var trig=(bath-1)/4;
		var ad=0;
		if(Math.abs(off)<trig){
			ad=0;
		}else {
			if(off<0){
				off+=trig;
			}else{
				off-=trig;
			};
			ad=off/(3*trig)*Math.PI/3;
		};
		this.ball.off=off;
		this.ball.ad=ad;
		return ad;
	};

	// Hit bat?
	if(this.ball.x-this.ball.size.w<this.right.x+this.right.size.w &&
	   this.ball.x+this.ball.size.w>this.right.x-this.right.size.w &&
	   this.ball.y+this.ball.size.h>this.right.y-this.right.size.h &&
	   this.ball.y-this.ball.size.h<this.right.y+this.right.size.h){
		this.ball.ang=Math.PI-this.ball.ang;
		this.ball.ang=this.ball.ang-newang(this.ball.y-this.right.y,this.right.size.h+this.ball.size.h);
		this.ball.x=this.right.x-this.right.size.w-this.ball.size.w;
		this.ball.spd*=1.1;
	};

	if(this.ball.x-this.ball.size.w<this.left.x+this.left.size.w &&
	   this.ball.x+this.ball.size.w>this.left.x-this.left.size.w &&
	   this.ball.y+this.ball.size.h>this.left.y-this.left.size.h &&
	   this.ball.y-this.ball.size.h<this.left.y+this.left.size.h){
		this.ball.ang=Math.PI-this.ball.ang;
		this.ball.ang=this.ball.ang+newang(this.ball.y-this.left.y,this.left.size.h+this.ball.size.h);
		this.ball.x=this.left.x+this.left.size.w+this.ball.size.w;
		this.ball.spd*=1.1;
	};

	// Score point
	if(this.ball.x+this.ball.size.w>field.w){
		this.score.left.pts++;
		this.setup(-1);
		this.ball.ang=Math.PI-this.ball.ang;
		this.game.run=1;
	};

	if(this.ball.x-this.ball.size.w<-field.w){
		this.score.right.pts++;
		this.setup(1);
		this.game.run=1;
	};

	if(this.ball.ang<0)
		this.ball.ang+=2*Math.PI;
	if(this.ball.ang>2*Math.PI)
		this.ball.ang-=2*Math.PI;

	$('#debug').html("ang="+Math.abs(this.ball.ang/Math.PI*180)+" ,x="+Math.ceil(this.ball.x)+" ,y="+Math.ceil(this.ball.y));//+" ,off="+(this.ball.off)+" ,ad="+Math.ceil(this.ball.ad));
};

pong.prototype.display=function(){
	function move(obj,av){
	  var x = obj.x - obj.size.w + field.w;
	  var y = obj.y - obj.size.h + field.h;
	  obj.avatar.css({left:x,top:y});
	};

	move(this.ball);
	move(this.left);
	move(this.right);
    this.score.left.avatar.html(this.score.left.pts);
    this.score.right.avatar.html(this.score.right.pts);
};

var time = function() { return +(new Date) / 100 };
var last = time();
pong.prototype.run = function(){
	var now = time();

	if(game.game.run==1)
		game.update(now - last);
      
	game.display();
	last=now;
};

pong.prototype.socketstart = function(uri){
	function debug(str){
		$("#socket").append("<br>" +  str);
	};

	ws = new WebSocket(uri);

	debug("ws start");
	ws.onopen = function() {
		debug("ws open");
		$('#socket').css({"color": "green"});
		ws.send("hello");
		this.game.run=1;
	};

	ws.onmessage = function (evt) {
		var data = evt.data;
		$('#socket').html(data);
		if(data.slice(0,1)=="{"){
			var result=JSON.parse(data);
			if ("right" in result){
				game.setright(result.right);
			} ;
			if ("left" in result){
				game.setleft(result.left);
			};
			if ("cntl" in result){
				game.cnt.left.avatar.html(result.cntl);
			};
			if ("cntr" in result){
				game.cnt.right.avatar.html(result.cntr);
			};
		}else{
			// unknown non-json input
		};
	};

	ws.onclose = function() {
		debug("socket closed");
		$('#socket').css({"color": "red"});
		// reconnect
	};
};

// Start utility stuff 
(function() {
	var onEachFrame;
	if (window.webkitRequestAnimationFrame) {
		onEachFrame = function(cb) {
			var _cb = function() { cb(); webkitRequestAnimationFrame(_cb); }
			_cb();
		};
	} else if (window.mozRequestAnimationFrame) {
		onEachFrame = function(cb) {
			var _cb = function() { cb(); mozRequestAnimationFrame(_cb); }
			_cb();
		};
	} else {
		onEachFrame = function(cb) {
			setInterval(cb, 1000 / 60);
		}
	}
	window.onEachFrame = onEachFrame;
})();

var game;
$(document).ready(function(){
	game=new pong();
    if (window.MozWebSocket) {
        window.WebSocket = window.MozWebSocket;
    }
	if ("WebSocket" in window) {
		game.socketstart(socket);
	} else {
		alert("You have no web sockets");
	};
	window.onEachFrame(game.run);
});
