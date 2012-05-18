#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;
use warnings;
use Socket;

our $port=514;
my  $hispaddr;

my $socket;

my($iaddr,$proto,$paddr);
$iaddr = pack('C4', 0,0,0,0);
$proto = getprotobyname('udp');
$paddr = sockaddr_in($port, $iaddr); # 0 means let kernel pick

socket($socket, PF_INET, SOCK_DGRAM, $proto)   || die "socket: $!";
bind($socket, $paddr)                          || die "bind: $!";

my ($hisiaddr,$host);
my $buf;
while (1){
    $hispaddr = recv($socket, $buf, 2048, 0)    || die "recv: $!";
    ($port, $hisiaddr) = sockaddr_in($hispaddr);
#    $host = gethostbyaddr($hisiaddr, AF_INET);
    $host=join(".",unpack("CCCC",$hisiaddr));
    $buf =~ y!a-zA-Z0-9.:,; _()[]{}?-!!cd;
    print substr(scalar(localtime),11,8)," ",$host," ",$buf,"\n";
};

