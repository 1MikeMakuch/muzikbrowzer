// $Id$

//#ifdef HAVE_CONFIG_H
#  include <config.h.win32>
//#endif


#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <unistd.h>
#include <io.h>

#include <id3/tag.h>
#include <id3/utils.h>
#include <id3/misc_support.h>
#include <id3/readers.h>
#include <id3/io_helpers.h>

using namespace std;

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

typedef int (*Handler)(ID3_Tag &tag, const char * value);
typedef struct {
	char * key;
	char * desc;
	Handler func;
} ID3TagRecord;

ID3TagRecord tagopts[] = {
	{"v", "id3 version", updateVersion},
	{"TCON", "genre", updateGenre},
	{"TPE1", "artist", updateArtist},
	{"TALB", "album", updateAlbum},
	{"TIT2", "title", updateTitle},
	{"TRCK", "track", updateTrack},
	{"TLEN", "length", updateLength},
	{"TYER", "year", updateYear},
	{"TPOS", "part in set", updatePart},
	{"TPUB", "publisher", updatePublisher},
	{"RVAD", "volume", updateVolume},
	{"UFID", "ufid", updateUFID},
	{"TMED", "dig...", updateMedia},
	{"COMM", "comment", updateComment},
	{ 0, 0, 0 }
};

static flags_t ID3_tag_ver = ID3TT_ID3V2;

int updateVersion(ID3_Tag & tag, const char * value) {
	if (value[0] == '1') {
		ID3_tag_ver = ID3TT_ID3V1;
	} else {
		ID3_tag_ver = ID3TT_ID3V2;
	}
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
	return 0;
}
int updateComment(ID3_Tag & tag, const char * value){ 
	ID3_AddComment(&tag, value, true);
	return 0;
}
int updateArtist(ID3_Tag & tag, const char * value){ 
	ID3_AddArtist(&tag, value, true);
	return 0;
}
int updateAlbum(ID3_Tag & tag, const char * value){ 
	ID3_AddAlbum(&tag, value, true);
	return 0;
}
int updateTitle(ID3_Tag & tag, const char * value){ 
	ID3_AddTitle(&tag, value, true);
	return 0;
}
int updateTrack(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_TRACKNUM, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
	return 0;
}
int updateLength(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_SONGLEN, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
	return 0;
}
int updateYear(ID3_Tag & tag, const char * value){ 
	ID3_AddYear(&tag, value, true);
	return 0;
}
int updatePart(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_PARTINSET, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
	return 0;
}
int updatePublisher(ID3_Tag & tag, const char * value){ 
	ID3_Frame* f;
	f = id3_AddFrame(ID3FID_PUBLISHER, &tag, value, true);
	if (f == 0)
		cerr << "error" << endl;
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
        cout << "(" << sDesc << ")[" << sFormat << ", "
             << nPicType << "]: " << sMimeType << ", " << nDataSize
             << " bytes" << endl;

//         ID3_FreeString(  sMimeType);
//         ID3_FreeString(  sDesc);
//         ID3_FreeString(  sFormat);
		delete sMimeType;
		delete sDesc;
		delete sFormat;
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
        cout << " frame" << endl;
        break;
      }
    }
  }
//   ID3_Free(iter);
	delete iter;
}

void usage() {
cout << 
"id3 [options] <file> # w/out options display tag info. options:" << endl;
		cout << "     -d   # deletes all id3 tag info from file" << endl;

	int i = 0;
	while (tagopts[i].key != 0) {
		cout << "     -" << tagopts[i].key << " <value> # " << tagopts[i].desc 
			<< endl;
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

	ID3_Tag tag1, tag2, tag;
    size_t pbytes1 = tag1.Link(filename, ID3TT_ID3V1);
    size_t pbytes2 = tag2.Link(filename, ID3TT_ID3V2);
    size_t pbytes = tag.Link(filename, ID3TT_ALL);

	if (argc < 3) {
		// just display tag
			if (tag1.HasV1Tag()) {
				cout << "id3v1:\n" ; 
				displayTag(tag);
			}
			if (tag2.HasV2Tag()) {
				cout << "id3v2:\n" ; 
				displayTag(tag);
			}
// 			if (pbytes != 0 && pbytes != pbytes2) {
// 				cout << "id3vAll\n";
// 				displayTag(tag);
// 			}
		exit(0);
	}

	// each option is followed by a value
	int i = 1;
	int error = 0;
	while(i < argc-1) {
		char * opt = argv[i];
		if (strcmp(opt,"-d") == 0) {
			flags_t ulTags = tag.Strip(ID3TT_ALL);
			cout << "id3Tag removed" << endl;
			exit(0);
		}
		char * optval = argv[i+1];
		if (!(opt[0] == '-' && optval[0] != '-')) {
			usage();
			exit(1);
		}
		opt++;
		int j = 0;
		int good = 0;
		while (tagopts[j].key != 0) {
			if (strcmp(tagopts[j].key, opt) == 0) {
				good = 1;
				break;
			}
			++j;
		}
// 		if (!good) {
// 			usage();
// 			exit(1);
// 		}
		i += 2;
	}

	// 
	i = 1;
	while(i < argc-1) {
		char * opt = argv[i];
		opt++;
		int j = 0;
		while (tagopts[j].key != 0) {
			if (strcmp(tagopts[j].key, opt) == 0) {
				(*tagopts[j].func)(tag, argv[i+1]);
				break;
			}
			++j;
		}
		i += 2;
	}
    luint nTags = tag.Update(ID3_tag_ver);
	displayTag(tag);

  return 0;
}
