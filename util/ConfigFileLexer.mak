all: FileLexer FontLexer

FileLexer:	ConfigFileLexer.l
	flex -L -+ -oConfigFileLexer.cpp ConfigFileLexer.l 

FontLexer:	ConfigFontLexer.l
	flex -L -+ -oConfigFontLexer.cpp ConfigFontLexer.l 
