#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;

use IO::Select;
use Digest::CRC qw(crc16 crcccitt);

$|=1;

my @fh;
my $read;

sub sprint{
	my @str=split(//,shift);
	for (@str){
		if (ord($_)>30 && ord($_)<127){
			print $_;
		}else{
			print "[x",unpack("H*",$_),"]";
		};
	};
};

my $ser=shift || "/dev/ttyS3";

open(SER, "+<",$ser) || die "open: $!";

my $sel = IO::Select->new;

$sel->add(\*SER);

my $cmd=shift;

if($cmd =~ /^r/){
	$cmd=~s/r//;
	$cmd+=1;
	my $fmt=shift;
	my $read="";
	while($cmd-->0){
		while($read !~ /\\1.*\\0/){
			my $rr="";
			if (@fh = $sel->can_read(100)) {
				sysread($fh[0],$rr,1024);
				$read.=$rr;
			}
		};
		while ($read =~ s/\\1(.*?)\\0//){
			my $str=$1;
			my $cs=substr($str,0,30);
			my $crc=unpack("n",substr($str,30,2));
			my $crc2= crcccitt($cs),"\n";
			if($fmt eq "m"){
				my $i=substr($str,0,1);
				print "M [",substr($str,0,1),"] ";
				print "g=",unpack("C",substr($str,1,1))," ";
				if($i eq "T"){
					print "t=",unpack("N",substr($str,2,4))," ";
					print "(",scalar gmtime unpack("N",substr($str,2,4)),") ";
					print "beacon=",unpack("H*",substr($str,26,4))," ";
				}elsif($i eq "B"){
					print "t=",unpack("N",substr($str,2,4))," ";
					print "ID=",unpack("c",substr($str,6,1))," ";
					print "HOP=",unpack("n",substr($str,11,4))," ";
				};
#				print "\n";
			}else{
				print "Read: <"; sprint $str; print ">\n";
			};
			print "CRCFAIL" if ($crc ne $crc2);
			print "\n";
		};
	};
	print "rest: <"; sprint $read; print ">\n";
	exit;
}elsif ($cmd eq "mt"){
	my $par=pack("H*",shift);
	print "Write: <"; sprint $par; print ">\n";
	syswrite(SER, '\3'.$par.'\0');
}elsif ($cmd eq "mta"){
	my $par=shift;
	print "Write: <"; sprint $par; print ">\n";
	print "len: ",length($par),"\n";
	syswrite(SER, '\3'.$par.'\0');
}elsif ($cmd eq "mr"){
	my $par=pack("H*",shift);
	print "Write: <"; sprint $par; print ">\n";
	syswrite(SER, '\4'.$par.'\0');
}elsif ($cmd eq "mra"){
	my $par=shift;
	print "Write: <"; sprint $par; print ">\n";
	syswrite(SER, '\4'.$par.'\0');
}elsif ($cmd eq "ch"){
	my $par=pack("H*",shift);
	print "Write: <"; sprint $par; print ">\n";
	syswrite(SER, '\5'.$par.'\0');
}elsif ($cmd eq "len"){
	my $par=pack("H*",shift);
	print "Write: <"; sprint $par; print ">\n";
	syswrite(SER, '\6'.$par.'\0');
}elsif ($cmd =~ /^S/){
	$cmd=~s/S//;
	$cmd+=1;
	
	my $par="";
	my $scmd=shift||"t";
	if($scmd eq "t"){
		$par.="T";
		$par.=chr(shift); #gen
		$par.=pack("N",scalar(time)+1*60*60);

		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
	}elsif($scmd eq "a"){
		$par.="A";
		$par.=chr(shift); #gen
		$par.=pack("N",scalar(time)+1*60*60+ 300);

        $par.= pack("C",shift||0);
        $par.= pack("C",0);
        $par.= pack("C",0);
        $par.= pack("C",0);

		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
	}elsif($scmd eq "b"){
		$par.="B";
		$par.=chr(shift); #gen
		$par.=pack("N",scalar(time)+1*60*60+ 600);

        $par.= pack("C",shift||0);
        $par.= pack("C",0);
        $par.= pack("C",0);
        $par.= pack("C",0);

		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
		$par.=pack("N",0);
	}else{
		die;
	};
	
	$par.=pack("n",crcccitt($par));
#	$par.="00";
	print "Write: <"; sprint $par; print ">\n";
	while($cmd-->0){
		syswrite(SER, '\1'.$par.'\0');
		print "len: ",length($par),"\n" if($cmd==0);

		my $read="";
		while($read !~ /\\2.*\\0/){
			my $rr="";
			if (@fh = $sel->can_read(100)) {
				sysread($fh[0],$rr,1024);
				$read.=$rr;
			}
		};

		if($cmd==0){
			print "Send: <"; sprint $read; print ">\n";
		};
	};
}elsif ($cmd =~ /^s/){
	$cmd=~s/s//;
	$cmd+=1;
	my $par=pack("H*",shift);
	$par.=pack("n",crcccitt($par));
	print "Write: <"; sprint $par; print ">\n";
	while($cmd-->0){
		syswrite(SER, '\1'.$par.'\0');
		print "len: ",length($par),"\n";

		my $read="";
		while($read !~ /\\2.*\\0/){
			my $rr="";
			if (@fh = $sel->can_read(100)) {
				sysread($fh[0],$rr,1024);
				$read.=$rr;
			}
		};

		print "Send: <"; sprint $read; print ">\n";
	};
}else{
die;
};

if (@fh = $sel->can_read(10)) {
  sysread($fh[0],$read,1024);
}
print "PostRead: <"; sprint $read; print ">\n";

