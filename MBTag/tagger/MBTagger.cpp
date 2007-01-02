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
//int updateVersion(MBTag & tag, const char * value);
int updateGenre(MBTag & tag, const char * value);
int updateArtist(MBTag & tag, const char * value);
int updateAlbum(MBTag & tag, const char * value);
int updateTitle(MBTag & tag, const char * value);
int updateTrack(MBTag & tag, const char * value);
int updateLength(MBTag & tag, const char * value);
int updateYear(MBTag & tag, const char * value);
//int updatePart(MBTag & tag, const char * value);
//int updatePublisher(MBTag & tag, const char * value);
//int updateVolume(MBTag & tag, const char * value);
//int updateUFID(MBTag & tag, const char * value);
//int updateMedia(MBTag & tag, const char * value);
int updateComment(MBTag & tag, const char * value);

int	updateFlag = 0;

typedef int (*Handler)(MBTag &tag, const char * value);
typedef struct {
	char * key;
	char * desc;
	Handler func;
	int arg;
} MBTagHandler;

MBTagHandler tagopts[] = {
	{"dt", "delete tag", deleteTag, 0},
	{"df", "delete tag field", deleteField, 1},
	{"getart", "getart, writes to <file>", getArt, 1},
	{"TCON", "genre", updateGenre, 1},
	{"TPE1", "artist", updateArtist, 1},
	{"TALB", "album", updateAlbum, 1},
	{"TIT2", "title", updateTitle, 1},
	{"TRCK", "track", updateTrack, 1},
	{"TLEN", "length", updateLength, 1},
	{"TYER", "year", updateYear, 1},
//	{"TPOS", "part in set", updatePart, 1},
//	{"TPUB", "publisher", updatePublisher, 1},
// 	{"RVAD", "volume", updateVolume, 1},
//	{"UFID", "ufid", updateUFID, 1},
//	{"TMED", "dig...", updateMedia, 1},
	{"COMM", "comment", updateComment, 1},
	{"APIC", "art file, reads from <file>", updateApic, 1},
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
	//flags_t ulTags = tag.Strip(MBTag_ver);
	return 0;
}
int deleteField(MBTag & tag, const char * value) {
	return 0;
}

int getArt(MBTag & tag, const char * value) {
	return 0;
}
int updateApic(MBTag & tag, const char * value) {
	return 0;
}
int updateGenre(MBTag & tag, const char * genre){ 
	tag.setValId3Key("TCON", genre);
	updateFlag = 1;
	return 0;
}
int updateComment(MBTag & tag, const char * value){ 
	tag.setValId3Key("COMM", value);
	updateFlag = 1;
	return 0;
}
int updateArtist(MBTag & tag, const char * value){ 
	tag.setValId3Key("TPE1", value);
	updateFlag = 1;
	return 0;
}
int updateAlbum(MBTag & tag, const char * value){ 
	tag.setValId3Key("TALB", value);
	updateFlag = 1;
	return 0;
}
int updateTitle(MBTag & tag, const char * value){ 
	tag.setValId3Key("TIT2", value);
	updateFlag = 1;
	return 0;
}
int updateTrack(MBTag & tag, const char * value){ 
	tag.setValId3Key("TRCK", value);
	updateFlag = 1;
	return 0;
}
int updateLength(MBTag & tag, const char * value){ 
	tag.setValId3Key("TLEN", value);
	updateFlag = 1;
	return 0;
}
int updateYear(MBTag & tag, const char * value){ 
	tag.setValId3Key("TYER", value);
	updateFlag = 1;
	return 0;
}
int updatePart(MBTag & tag, const char * value){ 
	updateFlag = 0;
	return 0;
}
int updatePublisher(MBTag & tag, const char * value){ 
	updateFlag = 0;
	return 0;
}
int updateVolume(MBTag & tag, const char * value){ 
	return 0;
}
int updateMedia(MBTag & tag, const char * value){ 
	updateFlag = 0;
	return 0;
}


void usage() {
cout << 
"mbtag [options] <file> # w/out options display tag info.  options:" << endl;
// 		cout << "     -d   # deletes all id3 tag info from file" << endl;

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
void dumptag(MBTag & tag) {
	CString key,val;
	for(POSITION pos = tag.GetSortedHead(); pos != NULL;) {
		tag.GetNextAssoc(pos,key,val);
		printf("%s = %s\n",key,val);
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


	if (argc < 2) {
		usage();
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
			tag.SetReadAllTags(TRUE);
			tag.read(filename,FALSE); // FALSE=don't convert keys to id3 names
			dumptag(tag);
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
    size_t pbytes2 = tag.read(filename,FALSE);

	i = 1;
	updateFlag = 0;
	while(i < argc-1) {
		char * opt = argv[i];
		opt++;
		int j = 0;
// 		printf("opt:%s\n", opt);
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
// 				printf("   calling %s\n", tagopts[j].desc);
				(*tagopts[j].func)(
// 					MBTag_ver == ID3TT_ID3V1 ? tag1 : tag2,
						tag,
						arg);
				break;
			}
			++j;
		}
	}
	if (updateFlag) {
		if (!tag.write()) {
			printf("error!\n");
			exit(-1);
		}

		dumptag(tag);

	}

  return 0;
}