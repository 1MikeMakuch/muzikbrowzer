// $Id$

#ifdef WIN32
	#include <config.h.win32>
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


#include <id3.h>

#include <id3/tag.h>
#include <id3/utils.h>
#include <id3/misc_support.h>
#include <id3/readers.h>
#include <id3/io_helpers.h>

using namespace std;


int deleteTag(ID3_Tag & tag, const char * value);
int deleteField(ID3_Tag & tag, const char * value);
int getArt(ID3_Tag & tag, const char * value);
int updateApic(ID3_Tag & tag, const char * value);
int updateVersion(ID3_Tag & tag, const char * value);
int updateGenre(ID3_Tag & tag, const char * value);
int updateArtist(ID3_Tag & tag, const char * value);
int updateAlbum(ID3_Tag & tag, const char * value);
int updateTitle(ID3_Tag & tag, const char * value);
int updateTrack(ID3_Tag & tag, const char * value);
int updateLength(ID3_Tag & tag, const char * value);
int updateYear(ID3_Tag & tag, const char * value);
int updatePart(ID3_Tag & tag, const char * value);
int updatePublisher(ID3_Tag & tag, const char * value);
int updateVolume(ID3_Tag & tag, const char * value);
int updateUFID(ID3_Tag & tag, const char * value);
int updateMedia(ID3_Tag & tag, const char * value);
int updateComment(ID3_Tag & tag, const char * value);

ID3_Frame* id3_AddFrame(ID3_FrameID frameid, ID3_Tag *tag, const char *text, bool replace = true);
size_t id3_RemoveFrame(ID3_FrameID frameid, ID3_Tag *tag);
int	updateFlag = 0;

typedef int (*Handler)(ID3_Tag &tag, const char * value);
typedef struct {
	char * key;
	char * desc;
	Handler func;
	int arg;
} ID3TagRecord;

ID3TagRecord tagopts[] = {
	{"v", "id3 version (1 or 2)", updateVersion, 1},
	{"dt", "delete tag (version 1 or 2 if specified else both", deleteTag, 0},
	{"df", "delete tag field", deleteField, 1},
	{"getart", "getart, writes to <file>", getArt, 1},
	{"TCON", "genre", updateGenre, 1},
	{"TPE1", "artist", updateArtist, 1},
	{"TALB", "album", updateAlbum, 1},
	{"TIT2", "title", updateTitle, 1},
	{"TRCK", "track", updateTrack, 1},
	{"TLEN", "length", updateLength, 1},
	{"TYER", "year", updateYear, 1},
	{"TPOS", "part in set", updatePart, 1},
	{"TPUB", "publisher", updatePublisher, 1},
// 	{"RVAD", "volume", updateVolume, 1},
	{"UFID", "ufid", updateUFID, 1},
	{"TMED", "dig...", updateMedia, 1},
	{"COMM", "comment", updateComment, 1},
	{"APIC", "art file, reads from <file>", updateApic, 1},
	{ 0, 0, 0 }
};

// static flags_t ID3_tag_ver = ID3TT_ID3V2;
static ID3_TagType ID3_tag_ver = ID3TT_ALL;

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
//         char
//         *sMimeType = ID3_GetString(frame, ID3FN_MIMETYPE),
//         *sDesc     = ID3_GetString(frame, ID3FN_DESCRIPTION),
//         *sFormat   = ID3_GetString(frame, ID3FN_IMAGEFORMAT);
//         size_t
//         nPicType   = frame->GetField(ID3FN_PICTURETYPE)->Get(),
//         nDataSize  = frame->GetField(ID3FN_DATA)->Size();
// //         const uchar * sRawData  = frame->GetField(ID3FN_DATA)->GetRawBinary();
//         cout << "(" << sDesc << ")[" << sFormat << ", "
//              << nPicType << "]: " << sMimeType << ", " << nDataSize
//              << " bytes" << endl;

int deleteTag(ID3_Tag & tag, const char * value) {
	flags_t ulTags = tag.Strip(ID3_tag_ver);
// 	printf("deleteTag %d\n", ulTags);
	return 0;
}
int deleteField(ID3_Tag & tag, const char * value) {
// 	printf("deleteField %d\n", ulTags);
	if (strcmp(value, "TCON") == 0) {
		ID3_RemoveGenres(&tag);
		updateFlag = 1;
	} else if (strcmp(value, "TPE1") == 0) {
		ID3_RemoveArtists(&tag);
		updateFlag = 1;
	} else if (strcmp(value, "TALB") == 0) {
		ID3_RemoveAlbums(&tag);
		updateFlag = 1;
	} else if (strcmp(value, "TIT2") == 0) {
		ID3_RemoveTitles(&tag);
		updateFlag = 1;
	} else if (strcmp(value, "TRCK") == 0) {
		ID3_RemoveTracks(&tag);
		updateFlag = 1;
	} else if (strcmp(value, "TLEN") == 0) {
		id3_RemoveFrame(ID3FID_SONGLEN, &tag);
		updateFlag = 1;
	} else if (strcmp(value, "TYER") == 0) {
		ID3_RemoveYears(&tag);
		updateFlag = 1;
	} else if (strcmp(value, "TPOS") == 0) {
		id3_RemoveFrame(ID3FID_PARTINSET, &tag);
		updateFlag = 1;
	} else if (strcmp(value, "TPUB") == 0) {
		id3_RemoveFrame(ID3FID_PUBLISHER, &tag);
		updateFlag = 1;
	} else if (strcmp(value, "UFID") == 0) {
		id3_RemoveFrame(ID3FID_UNIQUEFILEID, &tag);
		updateFlag = 1;
	} else if (strcmp(value, "TMED") == 0) {
		id3_RemoveFrame(ID3FID_MEDIATYPE, &tag);
		updateFlag = 1;
	} else if (strcmp(value, "COMM") == 0) {
		ID3_RemoveComments(&tag);
		updateFlag = 1;
	} else if (strcmp(value, "APIC") == 0) {
		id3_RemoveFrame(ID3FID_PICTURE, &tag);
		updateFlag = 1;
	}
	return 0;
}

int getArt(ID3_Tag & tag, const char * value) {
	ID3_Tag::Iterator* iter = tag.CreateIterator();
	const ID3_Frame* frame = NULL;
	while (NULL != (frame = iter->GetNext())) {
		ID3_FrameID eFrameID = frame->GetID();
		if (eFrameID == ID3FID_PICTURE) {
			char
			*sMimeType = ID3_GetString(frame, ID3FN_MIMETYPE),
			*sDesc     = ID3_GetString(frame, ID3FN_DESCRIPTION),
			*sFormat   = ID3_GetString(frame, ID3FN_IMAGEFORMAT);
			size_t
			nPicType   = frame->GetField(ID3FN_PICTURETYPE)->Get(),
			nDataSize  = frame->GetField(ID3FN_DATA)->Size();
			const uchar * sRawData  = frame->GetField(ID3FN_DATA)->GetRawBinary();

			int n = strlen(sMimeType);
//			int i;
			char * ext;
			for(ext = sMimeType + (n-1); ext > sMimeType && *ext != '/' ; --ext)
				;
			if (*ext == '/') {
				ext++;
				char file[1000];
				strcpy(file, value);
				strcat(file, ".");
				strcat(file, ext);

				FILE * f = fopen(file, "w");
				if (f != NULL) {
					fwrite(sRawData, 1, nDataSize, f);
					fclose(f);
					printf("%s written\n", file);
				}
			} else {
				printf("Unable to determine file extension from mimetype\n");
			}
			delete sMimeType;
			delete sDesc;
			delete sFormat;
			return 0;
		}
	}
	return 0;

}
int updateApic(ID3_Tag & tag, const char * value) {
	
	unsigned char pic_type = 0;
// 	char * pic_description;
//	char * pic_mimetype = "image/jpg";
	unsigned char * pic_bindata;
	int pic_bindatasize;

	char * tmp = strdup(value);
	char * ext = getExtension(tmp);
//	if (strcmp(ext, "jpg") != 0) {
//		printf("only jpg's supported\n");
//		exit(1);
//	}

	char pic_mimetype[1000];
	strcpy(pic_mimetype,"image/");
	strcat(pic_mimetype, ext);

#ifdef WIN32
	int f = open(value, O_RDONLY|_O_BINARY);
#else
	int f = open(value, O_RDONLY);
#endif
	if (f == -1) {
		printf("Couldn't open %s\n", value);
		exit(1);
	}
	struct stat pic_stat;
	int r = fstat(f, &pic_stat);
	if (r == -1) {
		printf("Couldn't stat %s\n", value);
		exit(1);
	}
	pic_bindatasize = pic_stat.st_size;
	pic_bindata = (unsigned char *) malloc(pic_bindatasize);
#ifdef WIN32
	size_t  bread = read(f, pic_bindata, pic_bindatasize);
#else
	ssize_t  bread = read(f, pic_bindata, pic_bindatasize);
#endif
	if (bread != pic_bindatasize) {
		printf("Read failure: %s\n", value);
		exit(1);
	}

	id3_RemoveFrame(ID3FID_PICTURE, &tag);

	ID3_Frame *hFrame = new ID3_Frame(ID3FID_PICTURE);
// 	hFrame->GetField(ID3FN_DESCRIPTION)->Set(pic_description);
	hFrame->GetField(ID3FN_PICTURETYPE)->Set((uint32) pic_type);
	hFrame->GetField(ID3FN_MIMETYPE)->Set(pic_mimetype);
	hFrame->GetField(ID3FN_DATA)->Set(pic_bindata, pic_bindatasize);
	tag.AttachFrame(hFrame);

// In my program i use this code to create a new APIC frame from a given
// image, this does completely wo rk for me:
// 
// ...,where "crnt" is one of my image objects (doesnt matter to you)
// which contains "pic_type" as unsi gned char, "pic_description"
// and "pic_mimetype" are *char, "pic_bindata" is *unsigned char and
// "pic_bindatasize " is int.
// 
// So if my "crnt->pic_type" is 0x01, the new image will have image type
// "file icon", 0x02 will cause the frame to have "Other file icon" as
// image type and so on.
// 
// Especially for these two values you might need to take care of one hint
// in the v2.4.0 standard: "There may only be one picture with the picture
// type declared as picture type $01 and $02 respective ly" since id3lib
// doesnt seem to take care of this, itself (well, i tested that a long
// time before, maybe it has been altered in between)
// 
// I hope this will help you,
// 
// Tobias Hoppe

// updateFlag = 1;
  
  
	return 0;
}
int updateVersion(ID3_Tag & tag, const char * value) {
	if (value[0] == '1') {
		ID3_tag_ver = ID3TT_ID3V1;
	} else {
		ID3_tag_ver = ID3TT_ID3V2;
	}
// 	updateFlag = 1;
	return 0;
}
int updateGenre(ID3_Tag & tag, const char * genre){ 
// 	size_t genre = atoi(value);
// 	ID3_AddGenre(&tag, genre, true);

	int n = strlen(genre);
	int allnums = 1;
	int i;
	char ch;
	for (i = 0 ; i < n ; ++i) {
		ch = genre[i];
		if (!('0' <= ch && ch <= '9')) {
			allnums = 0;
			break;
		}
	}

	ID3_Frame* frame = NULL;
	if (genre) {
		ID3_RemoveGenres(&tag);

		if (NULL == tag.Find(ID3FID_CONTENTTYPE)) {
			frame = new ID3_Frame(ID3FID_CONTENTTYPE);
			if (NULL != frame) {
				char sGenre[100];
				if (allnums) {
					int g = atoi(genre);
					sprintf(sGenre, "(%d)", g);
				} else {
					sprintf(sGenre, "%s", genre);
				}
				frame->GetField(ID3FN_TEXT)->Set(sGenre);
				tag.AttachFrame(frame);
			}
		}
	}
	updateFlag = 1;
	return 0;
}
int updateComment(ID3_Tag & tag, const char * value){ 
	ID3_AddComment(&tag, value, true);
	updateFlag = 1;
	return 0;
}
int updateArtist(ID3_Tag & tag, const char * value){ 
	ID3_AddArtist(&tag, value, true);
	updateFlag = 1;
	return 0;
}
int updateAlbum(ID3_Tag & tag, const char * value){ 
	ID3_AddAlbum(&tag, value, true);
	updateFlag = 1;
	return 0;
}
int updateTitle(ID3_Tag & tag, const char * value){ 
	ID3_AddTitle(&tag, value, true);
	updateFlag = 1;
	return 0;
}
int updateTrack(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_TRACKNUM, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
	updateFlag = 1;
	return 0;
}
int updateLength(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_SONGLEN, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
	updateFlag = 1;
	return 0;
}
int updateYear(ID3_Tag & tag, const char * value){ 
	ID3_AddYear(&tag, value, true);
	updateFlag = 1;
	return 0;
}
int updatePart(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_PARTINSET, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
	updateFlag = 1;
	return 0;
}
int updatePublisher(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_PUBLISHER, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
	updateFlag = 1;
	return 0;
}
int updateVolume(ID3_Tag & tag, const char * value){ 
	return 0;
}
int updateMedia(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_MEDIATYPE, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
	updateFlag = 1;
	return 0;
}

ID3_Frame* id3_AddFrame(ID3_FrameID frameid, ID3_Tag *tag, const char *text, bool replace) {
	ID3_Frame* frame = NULL;
	if (NULL != tag && NULL != text && strlen(text) > 0) {
		if (replace) {
			id3_RemoveFrame(frameid, tag);
		}
		if (replace || tag->Find(frameid) == NULL) {
			frame = new ID3_Frame(frameid);
			if (frame) {
				frame->GetField(ID3FN_TEXT)->Set(text);
				tag->AttachFrame(frame);
			}
		}
	}

	updateFlag = 1;
	return frame;
}

size_t id3_RemoveFrame(ID3_FrameID frameid, ID3_Tag *tag) {
	size_t num_removed = 0;
	ID3_Frame *frame = NULL;

	if (NULL == tag) {
		return num_removed;
	}

	while ((frame = tag->Find(frameid))) {
		frame = tag->RemoveFrame(frame);
		delete frame;
		num_removed++;
	}

	updateFlag = 1;
	return num_removed;
}

char *id3_GetStringUFID(const ID3_Frame *frame, ID3_FieldID fldName)
{
  const char *text1 = NULL;
  const uchar *text2 = NULL;
  char * text = NULL;
  size_t bsize = 0;

  if (NULL != frame)
  {

   ID3_Frame::ConstIterator* iter = frame->CreateIterator();
   const ID3_Field* myField = NULL;
   while (NULL != (myField = iter->GetNext()))
   {
        ID3_FieldID field_id = myField->GetID ();
        if (field_id == ID3FN_OWNER) {
            text1 = myField->GetRawText();
        } else if (field_id == ID3FN_DATA) {
            bsize = myField->BinSize();
            text2 = myField->GetRawBinary();
        }

   }
   delete iter;
   if (text1 && text2) {
       int len1 = strlen(text1);
       int lent = len1 + bsize + 2;
       text = new char[lent];
       memset(text,0, lent);
       strcpy(text, text1);
       text[len1] = ' ';
       memcpy(text + len1 + 1, text2, bsize);

   }

  }
  return text;
}

int updateUFID(ID3_Tag & tag, const char * ivalue){ 
	char *value;
	char *owner;
	char *space;
	const uchar *data;
	value = new char[strlen(ivalue)+1];
	strcpy(value,ivalue);
	owner = value;
	space = strstr(value, " ");
	data = (uchar*)space + 1;
	*space = 0;
	printf("owner:%s data:%s\n",owner, data);

	id3_RemoveFrame(ID3FID_UNIQUEFILEID, &tag);

	ID3_Frame *frame = new ID3_Frame(ID3FID_UNIQUEFILEID);
	if (frame) {
		frame->GetField(ID3FN_OWNER)->Set(owner);
		frame->GetField(ID3FN_DATA)->Set(data, strlen((const char*)data));
		tag.AttachFrame(frame);
	}
	updateFlag = 1;
	return 0;
}

using namespace dami;

void displayTag(const ID3_Tag &myTag)
{
  ID3_Tag::ConstIterator* iter = myTag.CreateIterator();
  const ID3_Frame* frame = NULL;
  while (NULL != (frame = iter->GetNext()))
  {
    const char* desc = frame->GetDescription();
    if (!desc) desc = "";
//     cout << "=== " << frame->GetTextID() << " (" << desc << "): ";
    cout << "=== " << frame->GetTextID() << " ";
    ID3_FrameID eFrameID = frame->GetID();
    switch (eFrameID)
    {
      case ID3FID_ALBUM:
      case ID3FID_BPM:
      case ID3FID_COMPOSER:
      case ID3FID_CONTENTTYPE:
      case ID3FID_COPYRIGHT:
      case ID3FID_DATE:
      case ID3FID_PLAYLISTDELAY:
      case ID3FID_ENCODEDBY:
      case ID3FID_LYRICIST:
      case ID3FID_FILETYPE:
      case ID3FID_TIME:
      case ID3FID_CONTENTGROUP:
      case ID3FID_TITLE:
      case ID3FID_SUBTITLE:
      case ID3FID_INITIALKEY:
      case ID3FID_LANGUAGE:
      case ID3FID_SONGLEN:
      case ID3FID_MEDIATYPE:
      case ID3FID_ORIGALBUM:
      case ID3FID_ORIGFILENAME:
      case ID3FID_ORIGLYRICIST:
      case ID3FID_ORIGARTIST:
      case ID3FID_ORIGYEAR:
      case ID3FID_FILEOWNER:
      case ID3FID_LEADARTIST:
      case ID3FID_BAND:
      case ID3FID_CONDUCTOR:
      case ID3FID_MIXARTIST:
      case ID3FID_PARTINSET:
      case ID3FID_PUBLISHER:
      case ID3FID_TRACKNUM:
      case ID3FID_RECORDINGDATES:
      case ID3FID_NETRADIOSTATION:
      case ID3FID_NETRADIOOWNER:
      case ID3FID_SIZE:
      case ID3FID_ISRC:
      case ID3FID_ENCODERSETTINGS:
      case ID3FID_YEAR:
      {
        char *sText = ID3_GetString(frame, ID3FN_TEXT);
        cout << sText << endl;
       delete  sText;
//         ID3_FreeString(sText);
        break;
      }
      case ID3FID_USERTEXT:
      {
        char 
        *sText = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);
        cout << sDesc << ": " << sText << endl;
		delete sText;
		delete sDesc;
//         ID3_FreeString(sText);
//         ID3_FreeString(  sDesc);
        break;
      }
      case ID3FID_COMMENT:
      case ID3FID_UNSYNCEDLYRICS:
      {
        char 
        *sText = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sLang = ID3_GetString(frame, ID3FN_LANGUAGE);
        cout << sDesc << " "
             << sText << endl;
			delete sText;
			delete sDesc;
			delete sLang;
//         ID3_FreeString(  sText);
//         ID3_FreeString(  sDesc);
//         ID3_FreeString(  sLang);
        break;
      }
      case ID3FID_WWWAUDIOFILE:
      case ID3FID_WWWARTIST:
      case ID3FID_WWWAUDIOSOURCE:
      case ID3FID_WWWCOMMERCIALINFO:
      case ID3FID_WWWCOPYRIGHT:
      case ID3FID_WWWPUBLISHER:
      case ID3FID_WWWPAYMENT:
      case ID3FID_WWWRADIOPAGE:
      {
        char *sURL = ID3_GetString(frame, ID3FN_URL);
        cout << sURL << endl;
//         ID3_FreeString(  sURL);
		delete sURL;
        break;
      }
      case ID3FID_WWWUSER:
      {
        char 
        *sURL = ID3_GetString(frame, ID3FN_URL),
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);
        cout << "(" << sDesc << "): " << sURL << endl;
//         ID3_FreeString(  sURL);
//         ID3_FreeString(  sDesc);
		delete sURL;
		delete sDesc;
        break;
      }
      case ID3FID_INVOLVEDPEOPLE:
      {
        size_t nItems = frame->GetField(ID3FN_TEXT)->GetNumTextItems();
        for (size_t nIndex = 0; nIndex < nItems; nIndex++)
        {
          char *sPeople = ID3_GetString(frame, ID3FN_TEXT, nIndex);
          cout << sPeople;
//           ID3_FreeString(  sPeople);
		  delete sPeople;
          if (nIndex + 1 < nItems)
          {
            cout << ", ";
          }
        }
        cout << endl;
        break;
      }
      case ID3FID_PICTURE:
      {
        char
        *sMimeType = ID3_GetString(frame, ID3FN_MIMETYPE),
        *sDesc     = ID3_GetString(frame, ID3FN_DESCRIPTION),
        *sFormat   = ID3_GetString(frame, ID3FN_IMAGEFORMAT);
        size_t
        nPicType   = frame->GetField(ID3FN_PICTURETYPE)->Get(),
        nDataSize  = frame->GetField(ID3FN_DATA)->Size();
//         const uchar * sRawData  = frame->GetField(ID3FN_DATA)->GetRawBinary();
        cout << "(" << sDesc << ")[" << sFormat << ", "
             << nPicType << "]: " << sMimeType << ", " << nDataSize
             << " bytes" << endl;

//         ID3_FreeString(  sMimeType);
//         ID3_FreeString(  sDesc);
//         ID3_FreeString(  sFormat);
		delete sMimeType;
		delete sDesc;
		delete sFormat;

#ifdef adsf
		FILE * wp = fopen("/tmp/file.jpg", "wb");
		if (wp != NULL) {
			size_t r = fwrite(sRawData, 1, nDataSize, wp);
			if (r != nDataSize) {
				cerr << "failed writing apic" << endl;
			}
		} else {
			cerr << "failed opening tmp apic file" << endl;
		}
#endif
				

        break;
      }
      case ID3FID_GENERALOBJECT:
      {
        char 
        *sMimeType = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sFileName = ID3_GetString(frame, ID3FN_FILENAME);
        size_t 
        nDataSize = frame->GetField(ID3FN_DATA)->Size();
        cout << "(" << sDesc << ")[" 
             << sFileName << "]: " << sMimeType << ", " << nDataSize
             << " bytes" << endl;
//         ID3_FreeString(  sMimeType);
//         ID3_FreeString(  sDesc);
//         ID3_FreeString(  sFileName);
		delete sMimeType;
		delete sDesc;
		delete sFileName;
        break;
      }
      case ID3FID_UNIQUEFILEID:
      {
        char *sOwner = id3_GetStringUFID(frame, ID3FN_TEXT);
        if (sOwner) {
            size_t nDataSize = frame->GetField(ID3FN_DATA)->Size();
//            cout << sOwner << ", " << nDataSize
//                 << " bytes" << endl;
              cout << sOwner << endl; 
//               ID3_FreeString(  sOwner);
			delete sOwner;
        } else {
            cout << endl;
        }
        break;
      }
      case ID3FID_PLAYCOUNTER:
      {
        size_t nCounter = frame->GetField(ID3FN_COUNTER)->Get();
        cout << nCounter << endl;
        break;
      }
      case ID3FID_POPULARIMETER:
      {
        char *sEmail = ID3_GetString(frame, ID3FN_EMAIL);
        size_t
        nCounter = frame->GetField(ID3FN_COUNTER)->Get(),
        nRating = frame->GetField(ID3FN_RATING)->Get();
        cout << sEmail << ", counter=" 
             << nCounter << " rating=" << nRating << endl;
//         ID3_FreeString(  sEmail);
		delete sEmail;
        break;
      }
      case ID3FID_CRYPTOREG:
      case ID3FID_GROUPINGREG:
      {
        char *sOwner = ID3_GetString(frame, ID3FN_OWNER);
        size_t 
        nSymbol = frame->GetField(ID3FN_ID)->Get(),
        nDataSize = frame->GetField(ID3FN_DATA)->Size();
        cout << "(" << nSymbol << "): " << sOwner
             << ", " << nDataSize << " bytes" << endl;
        break;
      }
      case ID3FID_SYNCEDLYRICS:
      {
        char 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sLang = ID3_GetString(frame, ID3FN_LANGUAGE);
        size_t
        nTimestamp = frame->GetField(ID3FN_TIMESTAMPFORMAT)->Get(),
        nRating = frame->GetField(ID3FN_CONTENTTYPE)->Get();
        const char* format = (2 == nTimestamp) ? "ms" : "frames";
        cout << "(" << sDesc << ")[" << sLang << "]: ";
        switch (nRating)
        {
          case ID3CT_OTHER:    cout << "Other"; break;
          case ID3CT_LYRICS:   cout << "Lyrics"; break;
          case ID3CT_TEXTTRANSCRIPTION:     cout << "Text transcription"; break;
          case ID3CT_MOVEMENT: cout << "Movement/part name"; break;
          case ID3CT_EVENTS:   cout << "Events"; break;
          case ID3CT_CHORD:    cout << "Chord"; break;
          case ID3CT_TRIVIA:   cout << "Trivia/'pop up' information"; break;
        }
        cout << endl;
        ID3_Field* fld = frame->GetField(ID3FN_DATA);
        if (fld)
        {
          ID3_MemoryReader mr(fld->GetRawBinary(), fld->BinSize());
          while (!mr.atEnd())
          {
            cout << io::readString(mr).c_str();
            cout << " [" << io::readBENumber(mr, sizeof(uint32)) << " " 
                 << format << "] ";
          }
        }
        cout << endl;
//         ID3_FreeString(  sDesc);
//         ID3_FreeString(  sLang);
		delete sDesc;
		delete sLang;
        break;
      }
      case ID3FID_AUDIOCRYPTO:
      case ID3FID_EQUALIZATION:
      case ID3FID_EVENTTIMING:
      case ID3FID_CDID:
      case ID3FID_MPEGLOOKUP:
      case ID3FID_OWNERSHIP:
      case ID3FID_PRIVATE:
      case ID3FID_POSITIONSYNC:
      case ID3FID_BUFFERSIZE:
      case ID3FID_VOLUMEADJ:
      case ID3FID_REVERB:
      case ID3FID_SYNCEDTEMPO:
      case ID3FID_METACRYPTO:
      {
        cout << " (unimplemented)" << endl;
        break;
      }
      default:
      {
        cout << " unknown frame" << endl;
        break;
      }
    }
  }
//   ID3_Free(iter);
	delete iter;
}

void usage() {
cout << 
"id3 [options] <file> # w/out options display tag info.  options:" << endl;
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

int main(int argc, char * const argv[])
{
	// must have at least 1 arg and total must be even
// 	if (argc < 2 || argc % 2 != 0) {
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
			ID3_Tag tag1;
			tag1.Link(filename, ID3TT_ID3V1);
			if (tag1.HasV1Tag()) {
				cout << "id3v1:\n" ; 
				displayTag(tag1);
			}
		}
		{
			ID3_Tag tag2;
			tag2.Link(filename, ID3TT_ID3V2);
			if (tag2.HasV2Tag()) {
				cout << "id3v2:\n" ; 
				displayTag(tag2);
			}
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
	ID3_Tag tag;
    size_t pbytes2 = tag.Link(filename, ID3TT_ALL);
//     size_t pbytes2 = tag.Link(filename, ID3TT_ID3V2);
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
// 					ID3_tag_ver == ID3TT_ID3V1 ? tag1 : tag2,
						tag,
						arg);
				break;
			}
			++j;
		}
	}
	if (updateFlag) {
// 		printf("ID3_tag_ver=%d ID3TT_ID3V1=%d ID3TT_ID3V2=%d ID3TT_ALL=%d ID3TT_NONE=%d\n", 
// 			ID3_tag_ver, ID3TT_ID3V1, ID3TT_ID3V2, ID3TT_ALL, ID3TT_NONE);
		flags_t updated = tag.Update(ID3_tag_ver);
		if (!(updated && ID3_tag_ver)) {
			printf("error!\n");
			printf("updated=%d, ID3_tag_ver=%d\n", updated, ID3_tag_ver);
			exit(-1);
		}
		if (ID3_tag_ver == ID3TT_ID3V1) {
			printf("update tag1\n");
// 			tag1.Update(ID3TT_ID3V1);
		}  else if (ID3_tag_ver == ID3TT_ID3V2) {
			printf("update tag2\n");
// 			tag2.Update(ID3TT_ID3V2);
		} else if (ID3_tag_ver == ID3TT_ALL) {
			printf("update all\n");
		}
// 		cout << "id3v1:\n" ; 
		displayTag(tag);
// 		cout << "id3v2:\n" ; 
// 		displayTag(tag2);
	}

  return 0;
}
