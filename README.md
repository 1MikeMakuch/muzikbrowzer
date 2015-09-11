A unique digital music browser/player, IMHO the only one worth a shit.

http://www.muzikbrowzer.com

Todo: I'd like to port this to something multi platform, either QT or java. Like to find others interested in joining in on the effort.

Let me know if you're interested.

1MikeMakuch at gmail.com

2015/09/10



```
Build instructions:

To build Muzikbrowzer:

I'm using MSVC6 sp6. 

All 3rd party libs are included in the src tree except the 
MS Platform sdk.  I'm using the last sdk for VC6, 

"MS Platform SDK, Feb 2003 Edition (Windows 2003 server)" 

is what it says on the CD. In case you don't have it;
http://mike.makuch.org/ps/MSSDK2003.zip Unzip it and point IE at the
home.htm file.

The "c:\Program Files\Microsoft SDK\include" has to be included 
before the stock VC98 includes.  In VC it's Tools/Options/Directories.
Same for the Lib.  Just in case you're rusty :-)

Also add 3rdparty/nasm to the dirs for EXEs. (for flac)

To build open the MSVC6 workspace muzik/browzer/muzikbrowzer.dsw

Set active project to 'muzikbrowzer'
Set active config to 'muzikbrowzer -Win32 Debug'
Build
Set working dir to muzik/browzer/Debug
Debug

Build either Debug or Release should build without error. There are warnings
in some of the 3rdparty stuff. DebugBC is just to keep seperate binaries for 
using BoundsChecker.

Projects. # denotes not required for building MB. Dependencies are
set so. Some of these accessory projects depend upon cygwin but
you shouldn't need it.

#BitmapCutter	util for making skins
#bitmaptest		testing
config			Settings Dialog
ConfigFileLexer	Simple lexer to parse the config files. Shouldn't have to flex it. src's are
				there. 
controls		most of the controls
GetBlackOut		easter egg. The secret is out!
#id3			id3 cl editor
id3lib			www.id3lib.org
#id3tagger		id3 Dlg editor
id3utils		my id3 tag lib utils
#Inno			makes setup.exe package
irman			support for www.evation.com irman
#mbpw			copy protection
#mbpwDlg			"
md5					"
#MetaDataEdit	wma tag command line editor
muzikbrowzer	main project, just build this
#mydiff			my cvs diff util
ogg_static		ogg support
#oggtagger		cl
#Prebuild		increments version info
#Publish		pushes setup.exe out to the web site
#regset			registry i/o cl
Serial			serial i/o for irman
TestHarness		unit test macros
util			utilities
vorbis*			ogg support
WmaTagg			wma tag lib
Wmp				C++ wrapper for WMP control
zlib			id3lib requirement
```

