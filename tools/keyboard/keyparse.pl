#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;
my $scan;
my $r;

my @keys=(
          "abcdefghijkl".
          "mnopqrstuvwxyz12".
		  "34567890\n\h	 -=[". #0x39 == caps lock
		  "]\\_;'`,./",            #0x65 == menu
          "ABCDEFGHIJKL".
          "MNOPQRSTUVWXYZ!@".
		  "#\$%^&*() _+{".
		  "}|:\"~<>?",
);

# Mod: 1= ctrl, 2=lshift, 4=lalt, 8=lwin
#      40=ralt, 80=rwin, 10=rctrl

my @key;

for (@keys){
	push @key, [split //];
};

#use Data::Dumper; print Dumper \@key;

open(my $in,"<",shift)||die;

while(read($in,$scan,2)){
	my $mod=ord substr($scan,0,1);
	my $key=ord substr($scan,1,1);

	print "<$mod,$key> = ";

	if ($mod==2 || $mod == 32) { # shift;
		$r=1;
	}else{
		$r=0;
	};
	if($key!=0){
		print $key[$r][$key];
	};
	print "\n";
};

