#!/usr/bin/perl

$f=$ARGV[0];

open(F,"<$f");
read(F,$buf,(-s $f));
close(F);

($decl,$versionstring,$rest) = split('"',$buf);
($version,$stamp) = split(' ',$versionstring);

($major,$minor,$patch) = split('\.',$version);

$exe = "muzikbrowzer_setup";
$exe_rev = "_" . $major . "_" . $minor . "_" . $patch ;
$exe_sfx = ".exe";
$exefile = $exe . $exe_rev . $exe_sfx;
$exesrc = "../Inno/Output/$exefile";
$exedst = "/cygdrive/w/muzikbrowzer/httpdocs/dl/$exefile";
$exedst2 = "/cygdrive/w/muzikbrowzer/httpdocs/dl/".$exe.$exe_sfx;
$src  = "/cygdrive/w/muzikbrowzer/httpdocs/dl/muzikbrowzerSrc"."$exe_rev";

# copy to www
open(F,"<$exesrc");
read(F,$buf,(-s $exesrc));
close(F);
open(F,">$exedst");
print F $buf;
close(F);
open(F,">$exedst2");
print F $buf;
close(F);

open(F,">/cygdrive/w/muzikbrowzer/httpdocs/dl/current_rev");
print F $major . '.' . $minor . '.' . $patch . "\n";
close(F);

system("cd /cygdrive/c/mkm/src/muzik/browzer/scripts; /usr/bin/bash --login /cygdrive/c/mkm/src/muzik/browzer/scripts/makezip.sh $src");
