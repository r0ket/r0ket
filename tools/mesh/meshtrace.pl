#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;
use Curses;
use POSIX qw(strftime);

use lib '.';
use r0ket;

$|=1;

my $ser="<undef>";

do {$ser=$_ if ( -e $_ ) } for qw(/dev/ttyS3 /dev/ttyACM0);

if ($ARGV[0] eq "-s"){
    shift;
    $ser=shift;
};

open(SER, "+<",$ser) || die "open serial: $!";

r0ket::readbeacon();

my $str;
my %bdata;

initscr;
END{endwin;}
use constant WIDTH => 80;
use constant m_height => 15;
my $win_top=subwin(2,WIDTH,0,0);
my $win=subwin(m_height,WIDTH,2,0);
noecho;
curs_set(0);
$win_top->addstr(0,0,"r0ket Mesh-Trace 0.1");
$win_top->addstr(1,0,"-"x20);
$win_top->refresh;

my $beaconctr=0;
while(1){
    $str=r0ket::get_packet(\*SER);
    my $p=r0ket::pkt_beauty($str);
    if(!$bdata{$p->{beacon}}){
        $bdata{$p->{beacon}}=++$beaconctr;
    };
    $win->addstr($bdata{$p->{beacon}},0,
            sprintf "%s | g=%d rel=%s time=%s =%+4d | %s",
            $p->{beacon},
            $p->{generation},
            $p->{release},
            strftime("%Y-%m-%d %H:%M:%S",gmtime $p->{time}),
            $p->{time}-(time+3600),
            r0ket::getbeacon($p->{beacon})
            );
    $win->refresh;
};
r0ket::rest();
