To build open the MSVC6 workspace muzik/browzer/muzikbrowzer.dsw

You can safely ignore the projects
Inno, PreBuild, Publish, mydiff, as they are included in the muzikbrowzer.dsw
but not in the zip file and are not part of muzikbrowzer proper.
Just cancel when MSVC prompts you for the locations of these projects.

Build either Debug or Release should build without error. There are warnings
in some of the 3rdparty stuff.

Projects

config			Configuration
controls		most of the controls
id3				id3 tag stuff
id3lib			www.id3lib.org
id3tagger		my handy id3 tagger (seperate .exe)
id3utils		
irman			support for www.evation.com irman
muzikbrowzer	main project, depends upon all others
ogg_static		ogg support
oggtagger
regset			registry i/o
Serial			serial i/o for irman
TestHarness		unit test macros
util			utility stuff
vorbis*			ogg support
zlib			id3lib requirement

