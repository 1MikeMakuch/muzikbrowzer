I did a fresh checkout of the sources and did a build to make sure
I've got all my ducks in a row. Only found 1 file missing from my
repository. Added that and it built and ran.

I'm using MSVC6 sp6. 

I'm using the last sdk for VC6, 
"MS Platform SDK, Feb 2003 Edition (Windows 2003 server)" is
what it says on the CD. In case you don't have it;
http://mike.makuch.org/ps/MSSDK2003.zip

The "MS SDK\include" has to be included before the stock VC98 includes".

I have the WindowsMediaPlayer format sdk rev 9.5 installed on my box. I
thought my build was depending on it but I don't see reference to it
anywhere in the workspace. DL it from MS or here
http://mike.makuch.org/ps/WMFormat95SDK.exe

To build open the MSVC6 workspace muzik/browzer/muzikbrowzer.dsw

Set active project to 'muzikbrowzer'
Set active config to 'muzikbrowzer -Win32 Debug'
Build
Set working dir to muzik/browzer/Debug
Debug

Build either Debug or Release should build without error. There are warnings
in some of the 3rdparty stuff. DebugBC is just to keep seperate binaries for 
using BoundsChecker.

Projects. # denotes not required for building MB.

#BitmapCutter	util for making skins
#bitmaptest		testing
config			Configuration Dialog
ConfigFileLexer	Simple lexer to parse the config files. Shouldn't have to build. src's are
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
#regset			registry i/o
Serial			serial i/o for irman
TestHarness		unit test macros
util			utilities
vorbis*			ogg support
WmaTagg			wma tag lib
Wmp				C++ wrapper for WMP control
zlib			id3lib requirement

