		/* This matches the font spec written by ConfigDisplay
		FFFFFFEC 00000000 00000000 00000000 000002BC 00 00 00 00 00 00 00 00 Arial 
		*/

		#ifndef _MUZKBR_MAIN_
			#include <stdafx.h>
		#else
			#define BOOL bool
			#define TRUE 1
			#define FALSE 0
		#endif

		#include <iostream>
		#include <sstream>
		#include <stdio.h>
		#include <errno.h>
		#include <malloc.h>
		#include <string.h>

		using namespace std;

		// The windows loader doesn't seem to execute this assignment,
		// leastwise it ain't TRUE in ConfigFontParser below so it
		// has to be set there.
		static BOOL good = TRUE;




%option c++
%option nounput noyywrap
%option prefix="ConfigFont"

ALNUM			[0-9a-zA-Z]
HEX				[0-9a-fA-F]
VALDATA			[!"$-~]
VALDATAWS		[ \t!"$-~]
WS				[ \t]
SPACE			[ ]
NL				[\r\n]
OP				[:=]

%%

{HEX}{8}{SPACE}{HEX}{8}{SPACE}{HEX}{8}{SPACE}{HEX}{8}{SPACE}{HEX}{8}{SPACE}{HEX}{2}{SPACE}{HEX}{2}{SPACE}{HEX}{2}{SPACE}{HEX}{2}{SPACE}{HEX}{2}{SPACE}{HEX}{2}{SPACE}{HEX}{2}{SPACE}{HEX}{2}{SPACE}{VALDATAWS}+ 	

.				good = FALSE;

%%

#include <fcntl.h>
#ifndef _MUZKBR_MAIN_
#include <share.h>
#endif
#include <sstream>



BOOL ConfigFontParser(const char * line) {

	istringstream iss;
	iss.str(line);

	good = TRUE;

	ConfigFontFlexLexer lexer(&iss,NULL);
	 lexer.yylex();

	 return good;

}

#ifdef _MUZKBR_MAIN_
int
main(int argc, char *argv[]) {
	
	BOOL r = ConfigFontParser(argv[1]);
	if (TRUE == r)
		printf("good\n");
	else
		printf("no good\n");
}
#endif
