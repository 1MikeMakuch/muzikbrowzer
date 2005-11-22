all: fonts files

fonts: ConfigFontLexer.l
	flex -L -+ ConfigFontLexer.l
	g++ -D_MUZKBR_MAIN_ -o fontlexer lex.ConfigFont.cc

files: ConfigFileLexer.l
	flex -L -+ ConfigFileLexer.l
	g++ -D_MUZKBR_MAIN_ -o filelexer lex.ConfigFile.cc
