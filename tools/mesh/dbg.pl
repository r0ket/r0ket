#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;

use IO::Select;
use Digest::CRC qw(crcccitt);
use POSIX qw(strftime);

$|=1;

my @fh;
my $read;

sub sprint{
    return join("",map {
		if (ord($_)>30 && ord($_)<127){
			$_;
		}else{
			"[x".unpack("H*",$_)."]";
		}
        }split(//,shift));
};

my %beacon;
sub readbeacon{
       return if( ! -f "BEACON" );
       open(B,"<","BEACON") || die "open: $!";
       while(<B>){
           /(\w+)\s+(.*)/ && do {
               $beacon{$1}=$2;
           };
       };
       close(B);
};
sub resolvebeacon{
    my $b=shift;
    if(!$beacon{$b}){
        return $b;
    }else{
        return "$b ($beacon{$b})";
    };
};
sub addbeacon{
    my($b,$n)=@_;
    if(!$beacon{$b}){
        $beacon{$b}=$n;
    };
};
sub writebeacon{
    open(B,">","BEACON") || die "write: $!";
    for(sort keys %beacon){
        print B "$_ $beacon{$_}\n";
    };
    close(B);
};

my $ser="<undef>";

do {$ser=$_ if ( -e $_ ) } for qw(/dev/ttyS3 /dev/ttyACM0);

if ($ARGV[0] eq "-h"){
    print STDERR "Mini-Help:\n";
    print STDERR "-s <devicename>\n";
    print STDERR "\n";
    print STDERR "r<num> receive (number) pakets\n";
    print STDERR " - r x : hexdump packets\n";
    print STDERR " - r m : parse as mesh packet\n";
    print STDERR " - r m <letter>: and show only <letter>\n";
    print STDERR "\n";
    print STDERR "s<num> send packet (number) times\n";
    print STDERR " - s <hex> : send raw hexdump\n";
    print STDERR " - S ... : see source \n";
    print STDERR "\n";
    print STDERR "p config per preset\n";
    print STDERR "- pM - preset mesh\n";
    print STDERR "- pB - preset openbeacon\n";
    print STDERR "\n";
    print STDERR "etc...\n";
    exit(1);
};
if ($ARGV[0] eq "-s"){
    shift;
    $ser=shift;
};

my $writend=0;
if ($ARGV[0] eq "-w"){
    shift;
    $writend=1;
};

END{
    writebeacon if($writend);
};

open(SER, "+<",$ser) || die "open serial: $!";

my $sel = IO::Select->new;

$sel->add(\*SER);

my $cmd=shift;

if($cmd =~ /^r/){

    readbeacon();
	$cmd=~s/r//;
	$cmd=100 if $cmd+0==0;
	my $fmt=shift;
    my $arg=shift || undef;
	my $read="";

    while($cmd>0){
        if(length($read)>2 && $read !~ /^\\1/){
            $read=~s/^(.[^\\]*)//s;
            print "Unparseable stuff: <",sprint($1),">\n";
#            print "Rest was: ",sprint($read),"!\n";
        };
        if ($read !~ s/^\\1(.*?)\\0//s){
            my $rr="";
            sysread(SER,$rr,1024);
            $read.=$rr;
        }else{
            my $str=$1;
            $str=~s/\\\\/\\/g; # dequote
            my $pkt_crc=  unpack("n",substr($str,length($str)-2,2));
            my $calc_crc= crcccitt(substr($str,0,length($str)-2));

            if($fmt eq "m"){
                my $i=substr($str,0,1);
                next if(defined $arg && $arg ne $i);
                print "M [$i] ";
                print "g=",unpack("C",substr($str,1,1))," ";
                if($i eq "T"){
                    print "t=";
# print unpack("N",substr($str,2,4))," ";
                    print strftime("%Y-%m-%d %H:%M:%S",gmtime unpack("N",substr($str,2,4)));
                    printf " (%+3d)",unpack("N",substr($str,2,4))-(time+3600);
                    print " rel=",unpack("H*",substr($str,24,2));
                    print " beacon=",resolvebeacon(unpack("H*",substr($str,26,4)))," ";
                }elsif($i eq "i"){
                    print "score=",unpack("N",substr($str,2,4))," ";
                    print "nick=",unpack("Z*",substr($str,6,length($str)-8))," ";
                }elsif($i eq "B"){
                    print "t=",unpack("N",substr($str,2,4))," ";
                    print "ID=",unpack("c",substr($str,6,1))," ";
                    print "HOP=",unpack("n",substr($str,11,4))," ";
                }else{
                    print "<??: ",unpack("H*",substr($str,2,length($str)-4)),">";
                };
#			    print "\n";
            }elsif($fmt eq "b"){
                my $i=substr($str,1,1);
                if($i eq "\x17"){
                    print "BEACON ";
                    print "ln=",unpack("C",substr($str,0,1))," ";
                    print "bt=",unpack("H*",substr($str,2,1))," ";
                    print "str=",unpack("H*",substr($str,3,1))," ";
                    printf "idx=%08d ",unpack("N",substr($str,4,4));
                    print "beacon=",resolvebeacon(unpack("H*",substr($str,8,4)))," ";
                    if(unpack("H*",substr($str,12,2)) ne "ffff"){
                        print "unused=",unpack("H*",substr($str,12,2))," ";
                    };
                }elsif($i eq "\x23"){
                    print "NICK ";
                    print "beacon=",resolvebeacon(unpack("H*",substr($str,2,4)))," ";
                    print "nick=",unpack("Z*",substr($str,6,length($str)-2))," ";
                    addbeacon(unpack("H*",substr($str,2,4)),unpack("Z*",substr($str,6,length($str)-2)));
                }else{
                    #<?:1023332ed221312d342e312e3400dddb>
                    print "<?:",unpack("H*",$str),">";
                };
            }elsif($fmt eq "x"){
                print "<",unpack("H*",$str),">";
            }else{
                print "<", sprint($str), ">\n";
            };
            print "CRCFAIL" if ($pkt_crc ne $calc_crc);
            print "\n";
            $cmd--;
            next;
        };
        if($read !~ /^\\1/){
        };
    };
    if(length($read)>0){
        print "rest: <", sprint($read), ">\n";
    };
	exit;
}elsif ($cmd eq "pM"){
	syswrite(SER, '\3ORBIT\0');
	syswrite(SER, '\4ORBIT\0');
	syswrite(SER, '\5S\0');
	syswrite(SER, '\6'.pack("H*","20").'\0');
}elsif ($cmd eq "pB"){
	syswrite(SER, '\3'.pack("H*","0102030201").'\0');
	syswrite(SER, '\4'.pack("H*","0102030201").'\0');
	syswrite(SER, '\5Q\0');
	syswrite(SER, '\6'.pack("H*","10").'\0');
}elsif ($cmd eq "mt"){
	my $par=pack("H*",shift);
	print "Write: <", sprint($par),">\n";
	syswrite(SER, '\3'.$par.'\0');
}elsif ($cmd eq "mta"){
	my $par=shift;
	print "Write: <", sprint($par),">\n";
	print "len: ",length($par),"\n";
	syswrite(SER, '\3'.$par.'\0');
}elsif ($cmd eq "mr"){
	my $par=pack("H*",shift);
	print "Write: <", sprint($par),">\n";
	syswrite(SER, '\4'.$par.'\0');
}elsif ($cmd eq "mra"){
	my $par=shift;
	print "Write: <", sprint($par),">\n";
	syswrite(SER, '\4'.$par.'\0');
}elsif ($cmd eq "ch"){
	my $par=pack("H*",shift);
	print "Write: <", sprint($par),">\n";
	syswrite(SER, '\5'.$par.'\0');
}elsif ($cmd eq "len"){
	my $par=pack("H*",shift);
	print "Write: <", sprint($par),">\n";
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
	}elsif($scmd eq "rrr"){
		$par.="G";
		$par.=chr(shift); #gen
#		$par.=pack("N",scalar(time)+300);
		$par.=pack("N",1325250000);

		$par.="r0ket mass p0ng";

		$par.=pack("C",0);
		$par.=pack("C",0);
		$par.=pack("C",0);
		$par.=pack("C",0);
		$par.=pack("C",0);
		$par.=pack("C",0);
		$par.=pack("C",0);
		$par.=pack("C",0);
		$par.=pack("C",0);
		print length($par),"\n";
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
	}elsif($scmd eq "c"){
		$par.="\x1";
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
	}elsif($scmd eq "i"){
		$par.="i";
		$par.=chr(shift); #gen
		$par.=pack("N",shift||42);

        $par.=shift;
        $par.="\0"x(30-length($par));
	}else{
		die;
	};
	
	$par.=pack("n",crcccitt($par));
#	$par.="00";
	print "Write: <", sprint($par),">\n";
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
			print "Send: <". sprint($read) , ">\n";
		};
	};
}elsif ($cmd =~ /^s/){
	$cmd=~s/s//;
	$cmd+=1;
	my $par=pack("H*",shift);
	$par.=pack("n",crcccitt($par));
	print "Write: <", sprint($par), ">\n";
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

		print "Send: <", sprint($read) , ">\n";
	};
}else{
    die "Option not understood\n";
};

if (@fh = $sel->can_read(10)) {
  sysread($fh[0],$read,1024);
}
print "PostRead: <", sprint($read), ">\n";

