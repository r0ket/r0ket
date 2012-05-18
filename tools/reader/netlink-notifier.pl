#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;
use POSIX;
use Socket;

use FindBin;
use lib "$FindBin::Bin/lib";
use l0gger;

l0gger::init();


use constant AF_NETLINK => 16;
#use Socket::Netlink;

# Netlink setup
socket my $sock,AF_NETLINK,SOCK_RAW,0 #(domain, type, bus//protocol)
	or die "socket: $!\n";
bind $sock, pack("vvVV",AF_NETLINK,0,0,1) #(domain, zero , pid, group);
	or die "bind: $!\n";

while(1){
	sysread($sock,my $msg, 65535) or die "recv: $!";
	my ($ifname,$state)=(undef,undef);

#	print unpack("H*",substr($message,0,16)),"\n"; # ifi_type etc. 
	$msg=substr($msg,16);

	my (undef,undef,undef,undef,undef,$type)=unpack("nnnnCC",$msg);
	$msg=substr($msg,16);
#	print "Type=$type\n";
	next if($type!=16); 	# RTM_NEWLINK

	while(length($msg)){
		my($len,$type)=unpack("vv",$msg);
#		print "len= $len,type=$type\n";
		last if($len<4);

		$msg=substr($msg,4);
		if    ($type ==  3){	# IFLA_IFNAME
			$ifname=unpack("Z*",$msg);
		}elsif($type == 16){	# IFLA_OPERSTATE
			$state=unpack("v",$msg);
		}else{
#			print "content=",unpack("H*",substr($msg,0,$len-4)),"\n";
		};

		$len=(int(($len-1)/4))*4; # 4-byte alignment
		$msg=substr($msg,$len);
	};
	next if(!defined($ifname) || !defined($state));
#	print "ifname=$ifname, state=$state\n";
# IF_OPER_UNKNOWN (0):
#  Interface is in unknown state, neither driver nor userspace has set
#  operational state. Interface must be considered for user data as
#  setting operational state has not been implemented in every driver.
# IF_OPER_NOTPRESENT (1):
#  Unused in current kernel (notpresent interfaces normally disappear),
#  just a numerical placeholder.
# IF_OPER_DOWN (2):
#  Interface is unable to transfer data on L1, f.e. ethernet is not
#  plugged or interface is ADMIN down.
# IF_OPER_LOWERLAYERDOWN (3):
#  Interfaces stacked on an interface that is IF_OPER_DOWN show this
#  state (f.e. VLAN).
# IF_OPER_TESTING (4):
#  Unused in current kernel.
# IF_OPER_DORMANT (5):
#  Interface is L1 up, but waiting for an external event, f.e. for a
#  protocol to establish. (802.1X)
# IF_OPER_UP (6):
#  Interface is operational up and can be used.
	if($state==6){
        l0gger::send("$ifname up");
	};
}
