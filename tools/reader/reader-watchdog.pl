#!/usr/bin/perl

use strict;
use POSIX ":sys_wait_h"; # for nonblocking read

use FindBin;
use lib "$FindBin::Bin/lib";
use lib "$FindBin::Bin/../mesh/lib";
use r0ket;
use l0gger;

$|=1;

my $DEV="/dev";

my %run;
my %childs;
my %config;

my @exec;

$SIG{CHLD} = sub {
    # don't change $! and $? outside handler
    local ($!, $?);
    my $pid = waitpid(-1, WNOHANG);
    return if $pid == -1;
    return unless defined $childs{$pid};
    delete $run{$childs{$pid}}; # cleanup?
    delete $childs{$pid};
};

# read config

sub readcfg{
    %config=();
    open(R,"<","reader.cf")|| die "Can't find reader.cf\n";
    while(<R>){
        chomp;
        next unless /(.*?)=(.*)/;
        $config{$1}=$2;
    };
    close(R);
    @exec=$config{"prog"},split(/\s+/,$config{"args"});
    delete $config{prog};
    delete $config{args};
};

readcfg();
l0gger::init();

while(1){
	opendir(my $dh, $DEV);
	my @paths=grep {/^ttyACM/} readdir($dh);
	close $dh;
#	print "f: ",join(",",@files),"\n";
	for my $path (@paths){
        next if $run{$path};

        my $id = eval {
            r0ket::r0ket_init($DEV."/".$path);
            return r0ket::get_id();
        };
#        print "r0id: $id\n";
        if(!defined $config{$id}){
            print "No config for r0ket $id @ $path, skipping...\n";
            next;
        };
        $run{$path}=$id;

        my $pid = fork();
        die "cannot fork" unless defined $pid;
        if ($pid == 0) {
            exec @exec,'-d',$DEV."/".$path,'-i',$config{$id};
        } else {
            print "Started $path : $id @ $pid\n";
            l0gger::send("started $path : $id @ $pid");
            $childs{$pid}=$path;
        }
	};
	sleep(1);
	print join(",",map {$run{$childs{$_}}."@".$childs{$_}."[$_]"} sort {$childs{$a}cmp$childs{$b}} keys %childs),"\n";
};
