// MBTagger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "MBTag.h"
#define LOGOWNER
#include "MyLog.h"

#ifdef WIN32
//	#include <config.h.win32>
	#include <iostream>
#include <io.h>
#else
	#include <config.h>
	#include <unistd.h>
	#include <iostream.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int deleteTag(MBTag & tag, const char * value);
int deleteField(MBTag & tag, const char * value);
int getArt(MBTag & tag, const char * value);
int updateApic(MBTag & tag, const char * value);
int help(MBTag & tag, const char * value);
int showComments(MBTag & tag, const char * value);

static BOOL SetField = FALSE;
static CString Key ;
static CString Val;
static int	updateFlag = 0;
static int commentsOnly = 0;

int setKey(MBTag & tag, const char * value){
	Key = tag.Id3Key2NativeKey(value);
	if (Key.GetLength() && Val.GetLength()) {
		tag.setValNativeKey(Key,Val);
		updateFlag = 1;
	}
	return 0;
}
int setVal(MBTag & tag, const char * value){
	Val = value;
	if (Key.GetLength() && Val.GetLength()) {
		tag.setValNativeKey(Key,Val);
		updateFlag = 1;
	}
	return 0;
}

int showComments(MBTag & tag, const char * x) {
	commentsOnly = 1;
	return 0;
}


typedef int (*Handler)(MBTag &tag, const char * value);
typedef struct {
	char * key;
	char * desc;
	Handler func;
	int arg;
} MBTagHandler;

MBTagHandler tagopts[] = {
	{"k",	"key, use with -v", setKey, 1},
	{"v",	"val, use with -k to set key/val", setVal, 1},
	{"dt",	"delete tag", deleteTag, 0},
	{"df",	"delete tag field", deleteField, 1},
	{"c",	"display comments only (COMM, COMMENTS, DESCRIPTION",showComments, 0},
//	{"getart", "gets art from tag, writes to <file>", getArt, 1},
//	{"setart", "read from <file>, add to tag (if applicable)", updateApic, 1},
	{"help",	"full help", help, 0},
	{ 0, 0, 0 }
};

static char * getExtension(char * file) {
	int n;
	int i;
	char * p;
	n = strlen(file);
	for(i = n-1 ; i > 0 ; --i) {
		p = &file[i];
		if (p[0] == '.') {
			return p+1;
		}
	}
	return NULL;
}

int deleteTag(MBTag & tag, const char * value) {
	updateFlag = 1;
	tag.SetDeleteTag();
	return 0;
}
int deleteField(MBTag & tag, const char * value) {
	tag.SetDeleteKey(value);
	updateFlag = 1;
	return 0;
}

int getArt(MBTag & tag, const char * value) {
	return 0;
}
int updateApic(MBTag & tag, const char * value) {
	return 0;
}
void usage();

int help(MBTag & tag, const char * value){
	CStringList types;
	MBTag t;
	t.GetExtensions(types);
	CString ts;
	for(POSITION pos = types.GetHeadPosition();pos != NULL; types.GetNext(pos)){
		ts += types.GetAt(pos) + " ";
	}
	cout << "mbtag from Muzikbrowzer www.muzikbrowzer.com" << endl <<
		"Standalone command line tag editor/viewer. Feel free" << endl <<
		"to distribute mbtag.exe." << endl << endl;
	cout << "usage:" << endl;

	usage();

	cout << endl << 
		"Examples:"<<endl<<endl<<
		"Set Genre:"<<endl<<
		"   mbtag -k TCON -v \"Texas Rock\" Fandango.mp3" << endl <<
		"   mbtag -k GENRE -v RockAndRollBaby ElvisLives.flac" << endl <<
		"Set Artist:"<<endl<<
		"   mbtag -k WM/AlbumArtist -v RockAndRollBaby ElvisLives.wma" << endl <<
		"   mbtag -k ARTIST -v \"The King\" ElvisLives.ogg" << endl <<
		"   mbtag -k TPE1 -v \"XYZZY\" TwistyLittleMaze.mp3" << endl <<
		endl <<
		"id3 keys converted to appropriate corresponding key when used on"<< endl <<
		"ogg, flac, wma files. Example:" << endl << 
		"   mbtag -k TCON -v \"Classical\" BethovensFifth.ogg" << endl <<
		"      is converted to" << endl <<
		"   mbtag -k GENRE -v \"Classical\" BethovensFifth.ogg" << endl <<
		endl <<
		"id3 keys supported: TCON,TPE1,TALB,TIT2,TRCK,TYER,COMM" << endl <<
		"ogg, flac, wma: anything goes" << endl <<
		"For wma's only type STRING fields are supported." <<
		endl;
return 0;
}
void usage() {
	cout << 
		"mbtag [options] <file> # w/out options display tag info.  options:" 
		<< endl;

	int i = 0;
	while (tagopts[i].key != 0) {
		char * arg = NULL;
		if (tagopts[i].arg) {
			arg = "<value>";
		} else {
			arg = "";
		}
		printf( "     -%-6s %-7s # %s\n", tagopts[i].key, arg, tagopts[i].desc);
		++i;
	}

}

CWinApp theApp;

int _tmain(int argc, TCHAR * argv[], TCHAR * envp[])
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		return 1;
	}
//	logger.open("c:\\temp\\mbtag.log");


	if (argc < 2) {
		usage();
		exit(0);
	}
	if (argv[1] == CS("-help")) {
		MBTag m;
		help(m,"");
		exit(0);
	}
	char * filename = argv[argc-1];
	int fd = open(filename, O_RDONLY);
	if (fd < 1) {
		cerr << "can't open " << filename << "!" << endl;
		exit(1);
	}
	close(fd);


	if (argc < 3) { // just display tag
		{
			MBTag tag;
			tag.read(filename,FALSE);
			printf("%s",tag.getInfo());
		}
		exit(0);
	}

// check args
	int i = 1;
	int error = 0;
	while(i < argc-1) {
		char * opt = argv[i];
		if (!(opt[0] == '-')) {
			usage();
			exit(1);
		}
		char * optval;
		opt++;
		int j = 0;
		int good = 0;
		while (tagopts[j].key != 0) {
			if (strcmp(tagopts[j].key, opt) == 0) {
				if (tagopts[j].arg) {
					if (i+1 > argc-1) {
						break;
					}
					optval = argv[i+1];
					if (optval[0] == '-') {
						break;
					}
					i += 2;
				} else {
					i += 1;
				}
				good = 1;
				break;
			}
			++j;
		}
		if (!good) {
			usage();
			exit(1);
		}
	}

	// Update tag
	MBTag tag;
	tag.SetReadAllTags(TRUE);
    size_t pbytes2 = tag.read(filename);

	i = 1;
	updateFlag = 0;
	while(i < argc-1) {
		char * opt = argv[i];
		opt++;
		int j = 0;
		while (tagopts[j].key != 0) {
			if (strcmp(tagopts[j].key, opt) == 0) {
				char * arg;
				if (tagopts[j].arg) {
					arg = argv[i+1];
					i += 2;
				} else {
					arg = NULL;
					i++;
				}
				(*tagopts[j].func)(tag,arg);
				break;
			}
			++j;
		}
	}
	if (commentsOnly) {
		double rg;
		printf("%s\n", tag.getComments(rg,filename));
	}
	if (updateFlag) {
		if (!tag.write()) {
			printf("error!\n");
			exit(-1);
		}

//		MBTag tag(filename);
//		printf("%s",tag.getInfo());

	}

  return 0;
}