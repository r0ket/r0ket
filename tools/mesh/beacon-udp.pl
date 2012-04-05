#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;
use warnings;
use POSIX qw(strftime);
#use Time::HiRes qw(time);
use Digest::CRC qw(crcccitt);

use FindBin;
use lib "$FindBin::Bin/lib";
use r0ket;

$|=1;

use Getopt::Long;
my $server  = "127.0.0.1";
my $port    = 2342;
my $id      = 1234;
my $verbose = 0;
my $fast    = 0;
my $channel = 81;
my $mac     = "0102030201";
my $ser     = undef;
my $help    = 0;
my $intvl   = 2;
my $lintvl  = 60;
GetOptions (
        "server=s"  => \$server,
        "port=n"    => \$port,
        "id=n"      => \$id,
        "dev=s"     => \$ser,
        "fast"      => \$fast,
        "verbose"   => \$verbose,
        "channel=n" => \$channel,
        "mac=s"     => \$mac,
        "help"      => \$help,
        );

if($help){
    die "Currently no help. Please check the source\n";
};

$ser=r0ket::r0ket_init($ser);

# Default openbeacon settings.
r0ket::set_txmac(pack("H*",$mac)); # Not really needed.
r0ket::set_rxmac(pack("H*",$mac));
r0ket::set_channel($channel);
r0ket::set_rxlen(16);
$r0ket::quiet=1; # Hackety-hack :)

my %bdata;

use Socket;
use Sys::Hostname;

my($iaddr,$proto,$paddr);
$iaddr = gethostbyname(hostname());
$iaddr = pack('C4', 0,0,0,0);
$proto = getprotobyname('udp');
$paddr = sockaddr_in(0, $iaddr); # 0 means let kernel pick

socket(SOCKET, PF_INET, SOCK_DGRAM, $proto)   || die "socket: $!";
bind(SOCKET, $paddr)                          || die "bind: $!";

my $hisiaddr = inet_aton($server)    || die "unknown server name";
my $hispaddr = sockaddr_in($port, $hisiaddr);

###send(SOCKET, 0, 0, $hispaddr);

my $xterm=0;
my $screen=1;

my $crcerr=0;
my $errors=0;
my $ctr=0;
my($lcrcerr,$lctr,$lerrors)=(0,0,0);
if($verbose){
    my($dev)=$ser;
    if(!defined $dev){
        $dev="<undef>";
    }else{
        $dev=~s!/dev/!!;
    };
    print "OpenBeacon Reader $id sending [$dev] to [$server:$port]\n";
    if($xterm){
        print "\e]2;",
              "$id\[$dev] -> $server:$port @ ", 
              strftime("%Y-%m-%d %H:%M:%S ",localtime),
              "\a";
    }elsif($screen){
        print "\ek",
              "$id\[$dev]", 
              "\e\\";
    };
    print "\n";
};

sub interrupt {
    if($xterm){
        print "\e]2;", "<exit>", "\a";
    }elsif($screen){
        print "\ek", `hostname`, "\e\\";
    };
    exit;
}
if($verbose){
    $SIG{INT} = \&interrupt;
};
my $lasttime=time;
my $llasttime=time;
my $pkt;
my $donl=0;

my($typenick,$typebeacon,$typeunknown)=(0,0,0);
my($ltypenick,$ltypebeacon,$ltypeunknown)=(0,0,0);
while(1){
    $pkt=r0ket::get_packet();

    if($verbose){
        if(time-$lasttime >= $intvl){
            print "\r";
            if(time-$llasttime >= $lintvl){
                $donl=1;
                $llasttime=time;
            };
            $lasttime=time;
            print strftime("%Y-%m-%d %H:%M:%S ",localtime);
            printf "[%ds] cnt=%3d [b=%3d, n=%3d, ?=%3d] errs=%3d crcerr=%3d ",
                   $intvl,
                  ($ctr-$lctr),
                  ($typebeacon-$ltypebeacon),
                  ($typenick-$ltypenick),
                  ($typeunknown-$ltypeunknown),
                  ($errors-$lerrors),
                  ($crcerr-$lcrcerr);
            ($lctr,$lerrors,$lcrcerr)= ($ctr,$errors,$crcerr);
            ($ltypenick,$ltypebeacon,$ltypeunknown)= ($typenick,$typebeacon,$typeunknown);
            if($donl){
                $donl=0;
                print "\n";
            };
        };
    };

    next if($pkt eq "ack"); # in-band signalling.
    if(length($pkt) != 16){  # Sanity check
        $errors++;
        next;
    };
    $ctr++;

    my $hdr= pack("CCnnNN",
            1,         # proto (BEACONLOG_SIGHTING)
            0,         # interface (we only have one antenna per "reader")
            $id,       # readerid
            length($pkt)+16, # size
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
    if($p->{type} eq "beacon"){
        $typebeacon++;
    }elsif($p->{type} eq "nick"){
        $typenick++;
    }else{
        $typeunknown++;
    };
};
r0ket::rest();
