
		#include <stdafx.h>
		#include <stdio.h>
		#include <errno.h>
		#include <malloc.h>
		#include <string.h>

		#include <stack>
		using namespace std;
		#include "ConfigFileLexer.h"

		stack<KVPair> * _kvstack;

		void pushkv(char * key, char * val ) {
			KVPair kv(key,val);
			_kvstack->push(kv);
		}



%option c++
%option nounput noyywrap
%option prefix="ConfigFile"

%x keyac
%x valinit
%x valac


COMMENT			#.*
ALNUM			[0-9a-zA-Z]
VALDATA			[!"$-~]
VALDATAWS		[ \t!"$-~]
WS				[ \t]
NL				[\r\n]
OP				[:=]

%%
						char buf[1000];
						char * pbuf;
						char * key;
						char * val;

{COMMENT}
{OP}
{NL}					
{ALNUM}				{
						pbuf = buf; key = pbuf;
						*pbuf++ = yytext[0];
						BEGIN(keyac);
					}
<keyac>{ALNUM}+		{	
						strcpy(pbuf,yytext);
						pbuf += strlen(yytext);
					}
<keyac>{NL}+		{
						BEGIN(INITIAL);
					}
<keyac>{WS}*{OP}		*pbuf++ = 0; BEGIN(valinit);

<keyac>{ALNUM}*{WS}+{ALNUM}+.* {
						*pbuf++=0;
						BEGIN(INITIAL);
					}
<keyac>{WS}+
<valinit>{WS}+
<valinit>{NL}+		{
						BEGIN(INITIAL);
					}
<valinit>{ALNUM}	{
						val = pbuf;
						*pbuf++ = yytext[0];
						BEGIN(valac);
					}
<valac>{VALDATAWS}+{VALDATA}		{
						strcpy(pbuf,yytext);
						pbuf += strlen(yytext);
					}
<valac>{WS}|{COMMENT}	{
						*pbuf++ = 0;
						/*sprintf(buf2,"[%s]",key);
						printf("%-10s = [%s]\n", buf2,val);*/
						pushkv(key,val);
						BEGIN(INITIAL);
					}
<valac>{NL}			{
						*pbuf++ = 0;
						/*sprintf(buf2,"[%s]",key);
						printf("%-10s = [%s]\n", buf2,val);*/
						pushkv(key,val);
						BEGIN(INITIAL);
					}

%%
/*
void ConfigFileParser(const char * file, stack<KVPair> * kvstack) {
	yyin = fopen( file, "r" );

	_kvstack = kvstack;

	yylex();
	fclose(yyin);
}
*/

#include <fcntl.h>
#include <share.h>
#include <fstream>
#include "MyString.h"

void ConfigFileParser(const char * file, stack<KVPair> * kvstack) {

	std::ifstream fs(file, ios::in | ios::binary);

	_kvstack = kvstack;

	ConfigFileFlexLexer lexer(&fs, NULL);
	 lexer.yylex();

// 	yylex();
// 	fclose(yyin);
}
