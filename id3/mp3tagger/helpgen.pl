#!/usr/bin/perl

$helptxtfile = "help.txt";
$helptmplfile = "help.tmpl";

open(F,"<$helptmplfile");
read(F, $helptmpl, (-s $helptmplfile));
close(F);

open(F,"<$helptxtfile");
read(F, $helptxt, (-s $helptxtfile));
close(F);

$helptxt =~ s/"/\\"/g;
@helptxt = split("\n", $helptxt);
foreach $line(@helptxt) {
	chop $line;
	$newline = 'msg += "';
	$newline .= $line;
	$newline .= "\\r\\n\";";
	push(@newhelptxt,$newline);
}
$helptxt = join("\n",@newhelptxt);

@helptmpl = split("\n", $helptmpl);
foreach $line(@helptmpl) {
	if ($line =~ m/it goes here/) {
		push(@helpdoth, $helptxt);
	} else {
		push(@helpdoth, $line);
	}
}

$helpdoth = join("\n", @helpdoth);
open(F,">help.h");
print F $helpdoth;
close(F);

