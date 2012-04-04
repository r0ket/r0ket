#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;
use POSIX qw(strftime);
#use Time::HiRes qw(time);
use Digest::CRC qw(crcccitt);

use lib '.';
use r0ket;

$|=1;

r0ket::r0ket_init();

# Default openbeacon settings.
r0ket::set_txmac(pack("H*","0102030201"));
r0ket::set_rxmac(pack("H*","0102030201"));
r0ket::set_channel(81);
r0ket::set_rxlen(16);
$r0ket::quiet=1; # Hackety-hack :)

my %bdata;

use Socket;
use Sys::Hostname;

my($iaddr,$proto,$port,$paddr);
$iaddr = gethostbyname(hostname());
$proto = getprotobyname('udp');
$port = "2342";
$paddr = sockaddr_in(0, $iaddr); # 0 means let kernel pick

socket(SOCKET, PF_INET, SOCK_DGRAM, $proto)   || die "socket: $!";
bind(SOCKET, $paddr)                          || die "bind: $!";

my $host="127.0.0.1";
my $hisiaddr = inet_aton($host)    || die "unknown host";
my $hispaddr = sockaddr_in($port, $hisiaddr);

###send(SOCKET, 0, 0, $hispaddr);


my $beaconctr=0;
my $lasttime;
my $crcerr=0;
my $ctr=0;
my $fast=0;
my $pkt;
while(1){
    $pkt=r0ket::get_packet();
    next if($pkt eq "ack"); # in-band signalling.
    $ctr++;

    my $hdr= pack("CCnnNN",
            1,         # proto (BEACONLOG_SIGHTING)
            0,         # interface (we only have one antenna per "reader")
            1234,      # readerid
            32,        # size
            $ctr,      # sequence
            time       # timestamp
            );
    my $crc=pack("n",0xffff ^ crcccitt($hdr.$pkt));

    send(SOCKET, $crc.$hdr.$pkt,0,$hispaddr);

       next if($fast);
    my $p=r0ket::nice_beacon($pkt);
    if($p->{crc} ne "ok"){
        $crcerr++;
        next;
    };
};
print "received $ctr pkts, $crcerr crc errors\n";
r0ket::rest();
