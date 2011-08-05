#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;

my $good;

my $search=shift;

open(F,"<","SECRETS") || die;

while(<F>){
	chomp;s///;
	if($good){
		s/0x//g;y/ ,;//d;
		print $_,"\n";
	exit;
	};
	if(/$search/){
		$good=1;
	};
};
