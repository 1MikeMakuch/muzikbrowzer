#!/usr/bin/perl

$f=$ARGV[0];

open(F,"<$f");
read(F,$buf,(-s $f));
close(F);

$newbuf = '';
@lines = split("\n",$buf);
$trm = 0;
$tgm = 0;
$tbm = 0;
$trp = 0;
$tgp = 0;
$tbp = 0;

foreach $line(@lines) {
	chop $line;
	($key,$val) = split(':',$line);
	if (substr($val,0,3) eq " L " || substr($val,0,3) eq " S ") {
		$val = substr($val,3,9999);
	}
	while(substr($val,0,1) eq ' ') {
		$val = substr($val,1,9999);
	}
	$addit = 1;
	if ($key eq "TransRedMain") {
		$trm = $val;
		$addit = 0;
	}
	if ($key eq "TransGreenMain") {
		$tgm = $val;
		$addit = 0;
	}
	if ($key eq "TransBlueMain") {
		$tbm = $val;
		$addit = 0;
	}
	if ($key eq "TransRedPanel") {
		$trp = $val;
		$addit = 0;
	}
	if ($key eq "TransGreenPanel") {
		$tgp = $val;
		$addit = 0;
	}
	if ($key eq "TransBluePanel") {
		$tbp = $val;
		$addit = 0;
	}
	if ($addit) {
		$newbuf .= $key.": $val\r\n";
	}
}
$CTM = $trm . "," . $tgm . "," . $tbm;
$CTP = $trp . "," . $tgp . "," . $tbp;
$newbuf .= "ColorTransparentMain: $CTM\r\n";
$newbuf .= "ColorTransparentPanel: $CTP\r\n";


open(F,">$f");
print F $newbuf;
close(F);

system("sortfile $f");

