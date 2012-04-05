#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;
use Curses;
use POSIX qw(strftime);
use Time::HiRes qw(time);

use FindBin;
use lib "$FindBin::Bin/lib";
use r0ket;

$|=1;

r0ket::r0ket_init();

# Default openbeacon settings.
r0ket::set_txmac(pack("H*","0102030201"));
r0ket::set_rxmac(pack("H*","0102030201"));
r0ket::set_channel(81);
r0ket::set_rxlen(16);

#r0ket::readbeacon();

my $str;
my %bdata;

sub do_str;

initscr;
END{endwin;}
use constant WIDTH => 80;
use constant m_height => 15;
my $win_top=subwin(2,WIDTH,0,0);
my $win=subwin(m_height,WIDTH,2,0);
noecho;
curs_set(0);
$win_top->addstr(0,0,"r0ket Beacon-Trace 0.1");
$win_top->addstr(1,0,"-"x20);
$win_top->refresh;

my $beaconctr=0;
use constant CLEAN  => 10;
use constant UPDATE => 0.3;
my $lasttime;
my $lastcleantime;
my $clean;
my $crcerr=0;
while(1){
    $str=r0ket::get_packet();
    my $p=r0ket::nice_beacon($str);
    if($p->{crc} ne "ok"){
        $crcerr++;
        next;
    };

    if($p->{type} eq "beacon"){
        $bdata{$p->{beacon}}{seen}=time;
        $bdata{$p->{beacon}}{beacon}=$p;
        $bdata{$p->{beacon}}{stats}{$p->{strength}}++;
        if(!defined($bdata{$p->{beacon}}{stats}{first})){
            $bdata{$p->{beacon}}{stats}{first}=time;
        };
    }elsif($p->{type} eq "nick"){
        $bdata{$p->{beacon}}{nick}=$p->{nick};
    }else{ #unknown
        ;
    };

    if(time>$lastcleantime+CLEAN){
        $clean=1;
        $lastcleantime=time;
    }else{
        $clean=0;
    };
         
    my $line=0;
    if($clean){
        $win->clear;
        for my $b (sort keys %bdata){
            if($bdata{$b}{seen}+10<time){
                delete $bdata{$b}
            };
        };
    };
    if(time>$lasttime+UPDATE){
        for my $b (sort keys %bdata){
            $win->addstr($line++,0,
                    sprintf "%s | bt=%s idx=%8s | %s | %s",
                    $b,
                    $bdata{$b}{beacon}->{button},
                    $bdata{$b}{beacon}->{idx},
                    do_str($bdata{$b}{stats}),
                    $bdata{$b}{nick}."   "
                    );
        };
        $win_top->addstr(1,20,sprintf" cnt=%2d, crc=%d",scalar(keys %bdata),$crcerr);
        $win_top->refresh;

        $win->refresh;
        $lasttime=time;
    };
};
r0ket::rest();

sub do_str{
    my $hr=shift;
    my $df=time()-$hr->{first};
    $df=1 if $df==0;
    my $out="";
#    for(sort keys %$hr){
    for(qw(00 55 aa ff)){
        next if $_ eq "first";
        $out.=sprintf("%3d% ",($hr->{$_}/$df)*100/2);
    };

    return $out;
};
