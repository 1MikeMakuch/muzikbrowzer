#!/usr/bin/perl

sub mysystem {
	local($com) = @_;
	print $com."\n";
	system($com);
}
#$HOME="/cygdrive/c/cygwin/home/mkm";
$HOME="/Users/home";

$f=$ARGV[0];

#print "ok test!\n";
#exit(0);

open(F,"<$f");
read(F,$buf,(-s $f));
close(F);

($decl,$versionstring,$rest) = split('"',$buf);
($version,$ds,$dt) = split(' ',$versionstring);

($major,$minor,$patch) = split('\.',$version);

$exe = "Muzikbrowzer_setup";
$exe_rev = "_" . $major . "_" . $minor . "_" . $patch ;
$exe_sfx = ".exe";
$exefile = $exe . $exe_rev . $exe_sfx;
$exesrc = "../Inno/Output/$exefile";
$exedst  = "beech:/var/www/virtuals/muzikbrowzer/httpdocs/dl/$exefile";
$exedst2 = "beech:/var/www/virtuals/muzikbrowzer/httpdocs/dl/".$exe.$exe_sfx;
#$src  = "/cygdrive/w/muzikbrowzer/httpdocs/dl/muzikbrowzerSrc"."$exe_rev";

# copy to www
#open(F,"<$exesrc");
#read(F,$buf,(-s $exesrc));
#close(F);
#open(F,">$exedst");
#print F $buf;
#close(F);
#open(F,">$exedst2");
#print F $buf;
#close(F);

mysystem("/usr/bin/scp $exesrc $exedst");
$exedst  = "beech:/var/www/virtuals/muzikbrowzer/httpdocs/dl/pad/MuzikbrowzerSetup.exe";
mysystem("/usr/bin/scp $exesrc $exedst");

open(F,">current_rev");
print F $major . '.' . $minor . '.' . $patch;
close(F);
mysystem("/usr/bin/touch Publish.exe");
open(F,">current_rev_built");
print F "$ds $dt\n";
close(F);

mysystem("/usr/bin/scp current_rev beech:/var/www/virtuals/muzikbrowzer/httpdocs/dl/current_rev");
mysystem("/usr/bin/scp current_rev_built beech:/var/www/virtuals/muzikbrowzer/httpdocs/dl/current_rev_built");

#mysystem("cd /cygdrive/c/mkm/src/muzik/browzer/scripts; /usr/bin/bash --login /cygdrive/c/mkm/src/muzik/browzer/scripts/makezip.sh $src");


# $to="michaelk\@makuch.org";
# $to="mikeandmark\@makuch.org";
# open(F,">/tmp/mbpublish.txt");
# print F "To: $to\n";
# print F "Subject: new mb on mbdev.makuch.org\n";
# print F "From: michaelk\@makuch.org\n\n";
# print F "Your engineering team has published a new and 
# improved version of Muzikbrowzer for your pleasure.
# This is an automated message.\n\n";
# print F "version $major.$minor.$patch built $ds $dt\n";
# print F "http://mbdev.makuch.org\n
# 
# Here is the first 100 lines of the ChangeLog, the rest can be found at the link above.\n\n";
# close(F);
# 
# mysystem("(cd $HOME/src/muzik ; $HOME/scripts/cvs2cl.pl --summary --hide-filenames -f /tmp/mbpublish.txt2 2>&1 >/dev/null)");

#mysystem("/usr/bin/head -100 /tmp/mbpublish.txt2 >> /tmp/mbpublish.txt");
#mysystem("/usr/sbin/ssmtp $to < /tmp/mbpublish.txt");

# mysystem("/usr/bin/cp /tmp/mbpublish.txt2 ../../ChangeLog");
# mysystem("/usr/bin/cat ../../ChangeLog2003 >> ../../ChangeLog");
# mysystem("/usr/bin/scp ../../ChangeLog beech:/var/www/virtuals/mbdev/httpdocs/ChangeLog.txt");
# mysystem("/usr/bin/scp ../../ChangeLog beech:/var/www/virtuals/muzikbrowzer/httpdocs/ChangeLog.txt");
# mysystem("/usr/bin/scp ../Help/Html/*htm beech:/var/www/virtuals/muzikbrowzer/httpdocs/content/Help");
# mysystem("/usr/bin/scp ../Help/Html/*gif beech:/var/www/virtuals/muzikbrowzer/httpdocs/content/Help");
# mysystem("/usr/bin/scp ../Help/Html/*jpg beech:/var/www/virtuals/muzikbrowzer/httpdocs/content/Help");
# mysystem("/usr/bin/scp ../Help/Html/*css beech:/var/www/virtuals/muzikbrowzer/httpdocs/content/Help");
