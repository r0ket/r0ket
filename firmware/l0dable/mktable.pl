#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;

my $DIR="l0dable";

if( -d "../$DIR"){
    chdir("..");
}

if ( ! -d $DIR ){
    die "Can't find $DIR?";
};

my @symb;
open(Q,"<","l0dable/EXPORTS") || die "$!";
while(<Q>){
    chomp;s/\r$//;
    push @symb,$_;
};
close(Q);

$\="\n";

open (C,">","table.c")||die;
print C '#include "table.h"';

open (H,">","table.h")||die;
print H "#include <sysdefs.h>";

open (I,">","$DIR/usetable.h")||die;
print I "extern const void * TheTable[];";

print I "";

my %types;
my %files;
my %variable;

use File::Find ();

sub wanted {
    my $id;
    next unless /\.h$/;
	open(F,"<",$_) || die;
	while(<F>){
		chomp;s/\r$//;
		if(m!^[^(]* ([\w]+)\s*\(.*\);\s*(//.*)?$!){
            $id=$1;
            s/$id/(*)/;
            s/;//;
            s!//.*!!;
			$types{$id}="*($_)";
			$files{$id}=$File::Find::name;
		}elsif (m!^\s*extern\s[^(]* ([\w]+)\s*(\[\w*\]\s*)?;\s*(//.*)?$!){
            $id=$1;
            s/extern //;
            my $star="*";
            if( s/\[\w*\]//){
                $star="";
            };
            s/$id/*/;
            s/;//;
            s!//.*!!;
			$types{$id}="$star($_)";
            $variable{$id}=1;
			$files{$id}=$File::Find::name;
		};
	};
	close(F);
}

File::Find::find({wanted => \&wanted}, '.');

print C "";
print C qq!__attribute__ ((used, section("table"))) const void * TheTable[]={!;

my %defs;

for my $idx (0..$#symb){
    $_=$symb[$idx];
	if(!$types{$_}){
		warn "Couldn't find $symb[$idx]";
	};
    if(!$defs{$files{$_}}){
		print H qq!#include "$files{$_}"!;
        $defs{$files{$_}}++;
	};

    if($variable{$_}){
        print C "\&$_,";
    }else{
        print C "$_,";
    };
    print I "#define $_ ($types{$_}(TheTable[$idx]))";
};

print C "};";

close(I);
close(H);
close(C);

