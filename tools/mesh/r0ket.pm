#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;

use IO::Select;
package r0ket;

use Digest::CRC qw(crcccitt);
use POSIX qw(strftime);


### Utility
sub sprint{
    return join("",map {
		if (ord($_)>30 && ord($_)<127){
			$_;
		}else{
			"[x".unpack("H*",$_)."]";
		}
        }split(//,shift));
};

### Nickname/beacon helper functions
our %beacon;
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
sub getbeacon{
    my $b=shift;
    if(!$beacon{$b}){
        return "";
    }else{
        return $beacon{$b};
    };
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

### Packet mgmt

our $buffer;
sub get_packet{
    my $dev=shift;

    sub _get_bytes{
        my $rr;
        sysread($dev,$rr,1024);
        if(length($rr)<=1){
            select(undef,undef,undef,0.1);
        };
        $buffer.=$rr;
    };

    my $cnt=0;
    while(++$cnt<10){
        if(length($buffer)<2){
            _get_bytes();
        }elsif($buffer !~ /^\\[12]/){
            $buffer=~s/^(.[^\\]*)//s;
#            print STDERR "Unparseable stuff: <",sprint($1),">\n";
        }elsif ($buffer =~ s/^\\([12])(.*?)\\0//s){
            my $str=$2;
            $str=~s/\\\\/\\/g; # dequote
            return $str;
        }else{
            _get_bytes();
        };
    };
    die "No packets for >1sec?\n";
};

sub rest{
    if(length($buffer)>0){
        print "rest: <", sprint($buffer), ">\n";
    };
};

### Pkt beautify
sub nice_mesh{
    my $pkt=shift;
    my $out;
    my $type=substr($pkt,0,1);
#    next if(defined $arg && $arg ne $i);
    $out->{type}=$type;
    $out->{string}="[$type]";
    $out->{generation}=unpack("C",substr($pkt,1,1));
    $out->{string}.= " g=".$out->{generation};
    if($type eq "T"){
        $out->{time}=   unpack("N",substr($pkt,2,4));
        $out->{release}=unpack("H*",substr($pkt,24,2));
        $out->{beacon}= unpack("H*",substr($pkt,26,4));

        $out->{string}.=sprintf " t=%s (%+4d) rel=%s beacon=%s",
            strftime("%Y-%m-%d %H:%M:%S",gmtime $out->{time}),
            $out->{time}-(time+3600),
            $out->{release},
            resolvebeacon($out->{beacon});
    }elsif($type eq "i"){
        $out->{score}=unpack("N",substr($pkt,2,4));
        $out->{nick}= unpack("Z*",substr($pkt,6,length($pkt)-8));

        $out->{string}.=sprintf " score=%d nick=%s",
            $out->{score},
            $out->{nick};
    }elsif($type eq "B"){
        $out->{time}=unpack("N",substr($pkt,2,4));
        $out->{id}=  unpack("c",substr($pkt,6,1));
        $out->{hop}= unpack("n",substr($pkt,11,4));

        $out->{string}.=sprintf " t=%d id=%s hop=%3d",
            $out->{time},
            $out->{id},
            $out->{hop};
    }else{
        $out->{string}.= " <??: ".unpack("H*",substr($pkt,2,length($pkt)-4)).">";
    };
    return $out;
};

sub nice_beacon{
    my $pkt=shift;
    my $out;
    my $type=substr($pkt,1,1);
    $out->{type}=$type;

    if($type eq "\x17"){
        $out->{type}=    "beacon";
        $out->{length}=  unpack("C", substr($pkt,0,1));
        $out->{button}=  unpack("H*",substr($pkt,2,1));
        $out->{strength}=unpack("H*",substr($pkt,3,1));
        $out->{idx}=     unpack("N", substr($pkt,4,4));
        $out->{beacon}=  unpack("H*",substr($pkt,8,4));
        $out->{unused}=  unpack("H*",substr($pkt,12,2));

        $out->{string}=sprintf "BEACON ln=%d bt=%s str=%s idx=%08x beacon=%s",
            $out->{length},
            $out->{button},
            $out->{strength},
            $out->{idx},
            $out->{beacon};
        if(unpack("H*",substr($pkt,12,2)) ne "ffff"){
            print "unused=",unpack("H*",substr($pkt,12,2))," ";
        };
    }elsif($type eq "\x23"){
        $out->{type}=   "nick";
        $out->{beacon}= unpack("H*",substr($pkt,2,4));
        $out->{nick}=   unpack("Z*",substr($pkt,6,length($pkt)-2));

        $out->{string}=sprintf "NICK beacon=%s nick=%s",
            $out->{beacon},
            $out->{nick};
    }else{
        $out->{string}="<?:".unpack("H*",$pkt).">";
    };
    return $out;
};


sub pkt_beauty{
    my $pkt=shift;
    my $out;

    $out=nice_mesh($pkt);

    my $pkt_crc=  unpack("n",substr($pkt,length($pkt)-2,2));
    my $calc_crc= crcccitt(substr($pkt,0,length($pkt)-2));
    
    if ($pkt_crc eq $calc_crc){
        $out->{crc}="ok";
    }else{
        $out->{crc}="fail";
        $out->{string}.= " CRCFAIL";
    };
    return $out;
}


1;
