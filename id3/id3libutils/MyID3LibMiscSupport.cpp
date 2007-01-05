// $Id$

// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
// Copyright 1999, 2000  Scott Thomas Haug

// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
// License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

// The id3lib authors encourage improvements and optimisations to be sent to
// the id3lib coordinator.  Please see the README file for details on where to
// send such submissions.  See the AUTHORS file for a list of people who have
// contributed to id3lib.  See the ChangeLog file for a list of changes to
// id3lib.  These files are distributed with id3lib at
// http://download.sourceforge.net/id3lib/

#if defined HAVE_CONFIG_H
#include <config.h>
#endif

#include "StdAfx.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include <id3/misc_support.h>
#include <id3/readers.h>
#include <id3/io_helpers.h>
#include "MyID3LibMiscSupport.h"
#include "MyString.h"
#include "Mp3Header.h"
#include "MBTag/MBTag.h"
#include "MyLog.h"

#include <id3/field.h>
#include <id3/utils.h>

#include <afxtempl.h>
#include "SortedArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dami;

CString id3_GetStringUFID(const ID3_Frame *frame, ID3_FieldID fldName)
{
  const char *text1 = NULL;
  const uchar *text2 = NULL;
  char * text = NULL;
  size_t bsize = 0;
  CString Text;
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
       Text = text;
       ID3_FreeString(text);

   }

  }
  return Text;
}

CString id3_GetString(const ID3_Frame * frame, ID3_FieldID fldName) {
    CString Text;
    if (NULL == frame) { return Text; }
	char * text = ID3_GetString(frame, fldName);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetString(const ID3_Frame * frame, ID3_FieldID fldName,
                      size_t nIndex) {
    CString Text;
    if (NULL == frame) { return Text; }
	char * text = ID3_GetString(frame, fldName, nIndex);
    Text = text;
    ID3_FreeString(text);
    return Text;
}

CString id3_GetArtist(const ID3_Tag * tag) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetArtist(tag);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetAlbum(const ID3_Tag * tag) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetAlbum(tag);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetTitle(const ID3_Tag * tag) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetTitle(tag);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetYear(const ID3_Tag * tag) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetYear(tag);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetComment(const ID3_Tag * tag, const char * desc) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetComment(tag, desc);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetTrack(const ID3_Tag * tag) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetTrack(tag);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetGenre(const ID3_Tag * tag) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetGenre(tag);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetLyrics(const ID3_Tag * tag) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetLyrics(tag);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetLyricist(const ID3_Tag * tag) {
    CString Text;
    if (NULL == tag) { return Text; }
	char * text = ID3_GetLyricist(tag);
    Text = text;
    ID3_FreeString(text);
    return Text;
}
CString id3_GetTLEN(const ID3_Tag * tag) {
	CString Text;
	if (NULL == tag) { return Text; }

	ID3_Frame *frame = NULL;
	if (frame = tag->Find(ID3FID_SONGLEN)) {
		char * text = ID3_GetString(frame, ID3FN_TEXT);
		Text = text;
		ID3_FreeString(text);
	}
	return Text;
}

typedef CMapStringToString GenreTable;
static GenreTable gdb;
static GenreTable rgdb;

void
Genre_init() {
	gdb.SetAt("Blues","(0)");
	gdb.SetAt("Classic rock","(1)");
	gdb.SetAt("Country","(2)");
	gdb.SetAt("Dance","(3)");
	gdb.SetAt("Disco","(4)");
	gdb.SetAt("Funk","(5)");
	gdb.SetAt("Grunge","(6)");
	gdb.SetAt("Hip-Hop","(7)");
	gdb.SetAt("Jazz","(8)");
	gdb.SetAt("Metal","(9)");
	gdb.SetAt("New Age","(10)");
	gdb.SetAt("Oldies","(11)");
	gdb.SetAt("Other","(12)");
	gdb.SetAt("Pop","(13)");
	gdb.SetAt("R & B","(14)");
	gdb.SetAt("Rap","(15)");
	gdb.SetAt("Reggae","(16)");
	gdb.SetAt("Rock","(17)");
	gdb.SetAt("Techno","(18)");
	gdb.SetAt("Industrial","(19)");
	gdb.SetAt("Alternative","(20)");
	gdb.SetAt("Ska","(21)");
	gdb.SetAt("Death Metal","(22)");
	gdb.SetAt("Pranks","(23)");
	gdb.SetAt("Soundtrack","(24)");
	gdb.SetAt("Euro Techno","(25)");
	gdb.SetAt("Ambient","(26)");
	gdb.SetAt("Trip Hop","(27)");
	gdb.SetAt("Vocal","(28)");
	gdb.SetAt("Jazz+Funk","(29)");
	gdb.SetAt("Fusion","(30)");
	gdb.SetAt("Trance","(31)");
	gdb.SetAt("Classical","(32)");
	gdb.SetAt("Instrumental","(33)");
	gdb.SetAt("Acid","(34)");
	gdb.SetAt("House","(35)");
	gdb.SetAt("Game","(36)");
	gdb.SetAt("Sound Clip","(37)");
	gdb.SetAt("Gospel","(38)");
	gdb.SetAt("Noise","(39)");
	gdb.SetAt("Alternrock","(40)");
	gdb.SetAt("Bass","(41)");
	gdb.SetAt("Soul","(42)");
	gdb.SetAt("Punk","(43)");
	gdb.SetAt("Space","(44)");
	gdb.SetAt("Meditative","(45)");
	gdb.SetAt("Instrumental Pop","(46)");
	gdb.SetAt("Instrumental Rock","(47)");
	gdb.SetAt("Ethnic","(48)");
	gdb.SetAt("Gothic","(49)");
	gdb.SetAt("Darkwave","(50)");
	gdb.SetAt("Techno-Industrial","(51)");
	gdb.SetAt("Electronic","(52)");
	gdb.SetAt("Pop-Folk","(53)");
	gdb.SetAt("Eurodance","(54)");
	gdb.SetAt("Dream","(55)");
	gdb.SetAt("Southern Rock","(56)");
	gdb.SetAt("Comedy","(57)");
	gdb.SetAt("Cult","(58)");
	gdb.SetAt("Gangsta","(59)");
	gdb.SetAt("Top 40","(60)");
	gdb.SetAt("Christian","(61)");
	gdb.SetAt("Pop/Funk","(62)");
	gdb.SetAt("Jungle","(63)");
	gdb.SetAt("Native American","(64)");
	gdb.SetAt("Cabaret","(65)");
	gdb.SetAt("New Wave","(66)");
	gdb.SetAt("Psychadelic","(67)");
	gdb.SetAt("Rave","(68)");
	gdb.SetAt("Showtunes","(69)");
	gdb.SetAt("Trailer","(70)");
	gdb.SetAt("Lo-Fi","(71)");
	gdb.SetAt("Tribal","(72)");
	gdb.SetAt("Acid Punk","(73)");
	gdb.SetAt("Acid Jazz","(74)");
	gdb.SetAt("Polka","(75)");
	gdb.SetAt("Retro","(76)");
	gdb.SetAt("Musical","(77)");
	gdb.SetAt("Rock & Roll","(78)");
	gdb.SetAt("Hard Rock","(79)");
	gdb.SetAt("Folk","(80)");
	gdb.SetAt("Folk-Rock","(81)");
	gdb.SetAt("National Folk","(82)");
	gdb.SetAt("Swing","(83)");
	gdb.SetAt("Fast Fusion","(84)");
	gdb.SetAt("Bebob","(85)");
	gdb.SetAt("Latin","(86)");
	gdb.SetAt("Revival","(87)");
	gdb.SetAt("Celtic","(88)");
	gdb.SetAt("Bluegrass","(89)");
	gdb.SetAt("Avantgarde","(90)");
	gdb.SetAt("Gothic Rock","(91)");
	gdb.SetAt("Progressive Rock","(92)");
	gdb.SetAt("Psychedelic Rock","(93)");
	gdb.SetAt("Symphonic Rock","(94)");
	gdb.SetAt("Slow Rock","(95)");
	gdb.SetAt("Big Band","(96)");
	gdb.SetAt("Chorus","(97)");
	gdb.SetAt("Easy Listening","(98)");
	gdb.SetAt("Acoustic","(99)");
	gdb.SetAt("Humour","(100)");
	gdb.SetAt("Speech","(101)");
	gdb.SetAt("Chanson","(102)");
	gdb.SetAt("Opera","(103)");
	gdb.SetAt("Chamber Music","(104)");
	gdb.SetAt("Sonata","(105)");
	gdb.SetAt("Symphony","(106)");
	gdb.SetAt("Booty Bass","(107)");
	gdb.SetAt("Primus","(108)");
	gdb.SetAt("Porn Groove","(109)");
	gdb.SetAt("Satire","(110)");
	gdb.SetAt("Slow Jam","(111)");
	gdb.SetAt("Club","(112)");
	gdb.SetAt("Tango","(113)");
	gdb.SetAt("Samba","(114)");
	gdb.SetAt("Folklore","(115)");
	gdb.SetAt("Ballad","(116)");
	gdb.SetAt("Power Ballad","(117)");
	gdb.SetAt("Rhythmic Soul","(118)");
	gdb.SetAt("Freestyle","(119)");
	gdb.SetAt("Duet","(120)");
	gdb.SetAt("Punk Rock","(121)");
	gdb.SetAt("Drum Solo","(122)");
	gdb.SetAt("a Capella","(123)");
	gdb.SetAt("Euro-House","(124)");
	gdb.SetAt("Dance Hall","(125)");

    POSITION pos;
    CString key;
    CString val;
    for( pos = gdb.GetStartPosition(); pos != NULL; ) {
        gdb.GetNextAssoc(pos, key, val);
        rgdb.SetAt(val, key);
    }
}

CString
Genre_normalize(const CString genre) {
	CString tmp, tmp2;
//  This was the id3v1 way of doing it.
// 	tmp = string_replace(genre, '(', "");
// 	tmp = string_replace(tmp, ')', "");
	tmp = genre;

//  id3v2 says it can be "Rock", "(17)", "(17)Rock", ... and more
//  but MusicMatch doesn't get any fancier so I don't
//  Besides MM uses the # in parens and overrides the text so do i.
    if (rgdb.Lookup(tmp, tmp2) != 0) {
        tmp = tmp2;
    } else {
		CString parens = CS("(") + genre + CS(")");
		if (rgdb.Lookup(parens, tmp2) != 0) {
			tmp = tmp2;
		} else {
			char * str = tmp.GetBuffer(0);
			char * p1 = strchr(str, '(');
			char * p2 = strchr(str, ')');
			if (p2 > (p1+1)) {
				tmp = tmp.Mid((p1-str),(p2-p1)+1);
				if (rgdb.Lookup(tmp, tmp2) != 0) {
					tmp = tmp2;
				}
			}
		}
	}
	return tmp;
//	return string_downcase(tmp);
}

int
Genre_getInt(CString genre) {
    CString val;
    if (gdb.Lookup(genre, val) != 0) {
		val = val.Right(val.GetLength()-1);
		val = val.Left(val.GetLength()-1);
        return atoi((LPCTSTR) val);
    } else {
        return -1;
    }
}

void
Genre_getGenres(CStringArray & glist) {
    POSITION pos;
    for (pos = gdb.GetStartPosition(); pos != NULL;) {
        CString genre, val;
        gdb.GetNextAssoc(pos, genre, val);
        String::insertSort(glist, genre);
    }
    return;
}

void
Genre_addGenre(ID3_Tag & id3, const char * genre) {

  ID3_Frame* frame = NULL;
  if (genre)
  {
    ID3_RemoveGenres(&id3);

    if (NULL == id3.Find(ID3FID_CONTENTTYPE))
    {
      frame = new ID3_Frame(ID3FID_CONTENTTYPE);
      if (NULL != frame)
      {
//        int g = Genre_getInt(genre);
//        char sGenre[100];
//        if (g > 0) {
//            sprintf(sGenre, "(%d)", g);
//        } else {
//            sprintf(sGenre, "%s", genre);
//        }
        frame->GetField(ID3FN_TEXT)->Set(genre);
        id3.AttachFrame(frame);
      }
    }
  }
  
  return;
}

CString
displayTag(ID3_Tag *id3, BOOL showLabels, CString file) {


  CString out,filelabel;

  if (showLabels) {
	out +=     "Genre:  ";
	out += Genre_normalize(id3_GetGenre(id3));
	out += "\r\nArtist: ";
	out += id3_GetArtist(id3);
	out += "\r\nAlbum:  ";
	out += id3_GetAlbum(id3);
	out += "\r\nTitle:  ";
	out += id3_GetTitle(id3);
	out += "\r\nTrack:  ";
	out += id3_GetTrack(id3);
	out += "\r\nYear:   ";
	out += id3_GetYear(id3);
	out += "\r\n\r\nRaw id3 tag:\r\n";
  }

  ID3_Tag::ConstIterator* iter = (ID3_Tag::ConstIterator *)id3->CreateIterator();
  const ID3_Frame* frame = NULL;

  while (NULL != (frame = iter->GetNext()))
  {
    const char* desc = frame->GetDescription();
    if (!desc) desc = "";

	
    CString keylabel = (char*)frame->GetTextID();
    CString commkey;
    filelabel = "";
    if (keylabel == "COMM") {
        commkey = keylabel;
    } else {
        out += keylabel;
        out += " ";
    }
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
        out += sText;
        out += (char*)"\r\n";
//       delete  sText;
         ID3_FreeString(sText);
        break;
      }
//#ifdef asdf
      case ID3FID_USERTEXT:
      {
        char 
        *sText = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);
        out += sDesc;
        out += ": ";
        out += sText;
        out += "\r\n";
//		delete sText;
//		delete sDesc;
         ID3_FreeString(sText);
         ID3_FreeString(  sDesc);
        break;
      }
      case ID3FID_COMMENT:
      case ID3FID_UNSYNCEDLYRICS:
      {
        char 
        *sText = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sLang = ID3_GetString(frame, ID3FN_LANGUAGE);
        CString text = sText;
        if (commkey == "COMM" && text.Left(4) == "FILE") {
			file = text.Right(text.GetLength()-5);
            filelabel = "\r\nOther info:\r\n" + text;
        } else {
            out += commkey ; out += " ";
            out += sDesc;
            out += " ";
            out += sText;
            out += "\r\n";
        }

         ID3_FreeString(  sText);
         ID3_FreeString(  sDesc);
         ID3_FreeString(  sLang);
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
        out += sURL;
        out += "\r\n";
         ID3_FreeString(  sURL);
//		delete sURL;
        break;
      }
      case ID3FID_WWWUSER:
      {
        char 
        *sURL = ID3_GetString(frame, ID3FN_URL),
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);
        out += "(";
        out += sDesc; out += "): "; out += sURL; out += "\r\n";
         ID3_FreeString(  sURL);
         ID3_FreeString(  sDesc);
//		delete sURL;
//		delete sDesc;
        break;
      }
      case ID3FID_INVOLVEDPEOPLE:
      {
        size_t nItems = frame->GetField(ID3FN_TEXT)->GetNumTextItems();
        for (size_t nIndex = 0; nIndex < nItems; nIndex++)
        {
          char *sPeople = ID3_GetString(frame, ID3FN_TEXT, nIndex);
          out += sPeople;
           ID3_FreeString(  sPeople);
//		  delete sPeople;
          if (nIndex + 1 < nItems)
          {
            out += ", ";
          }
        }
        out += "\r\n";
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
        out += "("; out += sDesc; out += ")["; out += sFormat; out += ", ";
        out += nPicType; out += "]: ";out += sMimeType;out += ", ";out += nDataSize;
        out += " bytes"; out += "\r\n";

         ID3_FreeString(  sMimeType);
         ID3_FreeString(  sDesc);
         ID3_FreeString(  sFormat);
//		delete sMimeType;
//		delete sDesc;
//		delete sFormat;
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
        out += "(";
		out += sDesc;
		out += ")[" ;
		out += sFileName;out += "]: "; out += sMimeType;out += ", ";out += nDataSize;
        out += " bytes";out += "\r\n";
         ID3_FreeString(  sMimeType);
         ID3_FreeString(  sDesc);
         ID3_FreeString(  sFileName);
//		delete sMimeType;
//		delete sDesc;
//		delete sFileName;
        break;
      }
//#endif
      case ID3FID_UNIQUEFILEID:
      {
        CString sOwner = id3_GetStringUFID(frame, ID3FN_TEXT);
        if (sOwner.GetLength()) {
            size_t nDataSize = frame->GetField(ID3FN_DATA)->Size();
//            out += sOwner;out += ", ";out += nDataSize;
//                 + " bytes" + "\r\n";
              out += sOwner;out += "\r\n"; 
//               ID3_FreeString(  sOwner);
//			delete sOwner;
        } else {
            out += "\r\n";
        }
        break;
      }
//#ifdef asdf
      case ID3FID_PLAYCOUNTER:
      {
        size_t nCounter = frame->GetField(ID3FN_COUNTER)->Get();
        out += nCounter + "\r\n";
        break;
      }
      case ID3FID_POPULARIMETER:
      {
        char *sEmail = ID3_GetString(frame, ID3FN_EMAIL);
        size_t
        nCounter = frame->GetField(ID3FN_COUNTER)->Get(),
        nRating = frame->GetField(ID3FN_RATING)->Get();
        out += sEmail;out += ", counter="; 
		out += nCounter; out += " rating=";out += nRating; out += "\r\n";
         ID3_FreeString(  sEmail);
//		delete sEmail;
        break;
      }
      case ID3FID_CRYPTOREG:
      case ID3FID_GROUPINGREG:
      {
        char *sOwner = ID3_GetString(frame, ID3FN_OWNER);
        size_t 
        nSymbol = frame->GetField(ID3FN_ID)->Get(),
        nDataSize = frame->GetField(ID3FN_DATA)->Size();
        out += "("; out += nSymbol;out += "): "; out += sOwner;
		out += ", "; out += nDataSize; out += " bytes"; out += "\r\n";
        break;
      }
      case ID3FID_SYNCEDLYRICS:
      {
        char 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sLang = ID3_GetString(frame, ID3FN_LANGUAGE);
        size_t nTimestamp = frame->GetField(ID3FN_TIMESTAMPFORMAT)->Get();
        size_t nRating = frame->GetField(ID3FN_CONTENTTYPE)->Get();
        const char* format = (2 == nTimestamp) ? "ms" : "frames";
        out += "(";out += sDesc;out += ")["; out += sLang; out += "]: ";
        switch (nRating)
        {
          case ID3CT_OTHER:    out += "Other"; break;
          case ID3CT_LYRICS:   out += "Lyrics"; break;
          case ID3CT_TEXTTRANSCRIPTION:     out += "Text transcription"; break;
          case ID3CT_MOVEMENT: out += "Movement/part name"; break;
          case ID3CT_EVENTS:   out += "Events"; break;
          case ID3CT_CHORD:    out += "Chord"; break;
          case ID3CT_TRIVIA:   out += "Trivia/'pop up' information"; break;
        }
        out += "\r\n";
        ID3_Field* fld = frame->GetField(ID3FN_DATA);
        if (fld)
        {
          ID3_MemoryReader mr(fld->GetRawBinary(), fld->BinSize());
          while (!mr.atEnd())
          {
            out += io::readString(mr).c_str();
            out += " ["; out += io::readBENumber(mr, sizeof(uint32)); out += " " ;
			out += format; out += "] ";
          }
        }
        out += "\r\n";
         ID3_FreeString(  sDesc);
         ID3_FreeString(  sLang);
//		delete sDesc;
//		delete sLang;
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
        out += " (unimplemented)"; out += "\r\n";
        break;
      }
//#endif
      default:
      {
//        out += " frame"; out += "\r\n";
        break;
      }
    }
  }
//   ID3_Free((void*)iter);
    delete iter;
    out += filelabel;
//	out += "\r\n";

	AutoBuf buf(2000);
    Mp3Header mpg;
    if (mpg.mb_getFilePath(file) == TRUE) { 
		
		int chpos;
        chpos = sprintf(buf.p, "\r\nDuration    %d\r\n", mpg.getLengthInSeconds());
        chpos += sprintf((char*)buf.p + chpos, "Size       %d\r\n", mpg.getSize());
        chpos += sprintf((char*)buf.p + chpos, "Frames    %d\r\n", mpg.getNumberOfFrames());
        chpos += sprintf((char*)buf.p + chpos, "emphasis   %s\r\n", (LPCTSTR) mpg.getEmphasis());
        chpos += sprintf((char*)buf.p + chpos, "Original   %s\r\n", mpg.getOriginal() == TRUE ? "true" : "false" );
        chpos += sprintf((char*)buf.p + chpos, "Copyright  %s\r\n", mpg.getCopyright() == TRUE ? "true" : "false" );
        chpos += sprintf((char*)buf.p + chpos, "Protection %s\r\n", mpg.getProtection() == TRUE ? "true" : "false" );
        chpos += sprintf((char*)buf.p + chpos, "Version    %f\r\n", mpg.getVersion() );
        chpos += sprintf((char*)buf.p + chpos, "Layer      %d\r\n", mpg.getLayer() );
        chpos += sprintf((char*)buf.p + chpos, "Bitrate    %d\r\n", mpg.getBitrate() );
        chpos += sprintf((char*)buf.p + chpos, "Frequency  %d\r\n", mpg.getFrequency() );
        chpos += sprintf((char*)buf.p + chpos, "Mode       %s\r\n", (LPCTSTR)mpg.getMode() );
		out += buf.p;     
    } else {
        sprintf(buf.p, "No valid mp3 headers found\r\nDoes not appear to be an mp3 file.\r\n");
		out += buf.p;     
    }
	
    return out;
}
BOOL
ReadAllTags(ID3_Tag *id3, MBTag * tag) {


  ID3_Tag::ConstIterator* iter = (ID3_Tag::ConstIterator *)id3->CreateIterator();
  const ID3_Frame* frame = NULL;

  while (NULL != (frame = iter->GetNext()))
  {
	CString key,val;

    const char* desc = frame->GetDescription();
    if (!desc) desc = "";

	
    CString keylabel = (char*)frame->GetTextID();
    CString commkey;
    key = keylabel;
    if (keylabel == "COMM") {
        commkey = keylabel;
    }// else {
//        out += keylabel;
//        out += " ";
//    }
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
		val = sText;
        ID3_FreeString(sText);
        break;
      }
//#ifdef asdf
      case ID3FID_USERTEXT:
      {
        char 
        *sText = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);
		val = sDesc + CS(": ") + sText;
//        out += sDesc;
//        out += ": ";
//        out += sText;
//        out += "\r\n";
//		delete sText;
//		delete sDesc;
         ID3_FreeString(sText);
         ID3_FreeString(  sDesc);
        break;
      }
      case ID3FID_COMMENT:
      case ID3FID_UNSYNCEDLYRICS:
      {
        char 
        *sText = ID3_GetString(frame, ID3FN_TEXT), 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sLang = ID3_GetString(frame, ID3FN_LANGUAGE);
//        CString text = sText;
//        if (commkey == "COMM" && text.Left(4) == "FILE") {
//			file = text.Right(text.GetLength()-5);
//            filelabel = "\r\nOther info:\r\n" + text;
//        } else {
//            out += commkey ; out += " ";
//            out += sDesc;
//           out += " ";
//            out += sText;
//            out += "\r\n";
//        }
		val = sText;

         ID3_FreeString(  sText);
         ID3_FreeString(  sDesc);
         ID3_FreeString(  sLang);
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
        val = sURL;
//        out += "\r\n";
         ID3_FreeString(  sURL);
//		delete sURL;
        break;
      }
      case ID3FID_WWWUSER:
      {
        char 
        *sURL = ID3_GetString(frame, ID3FN_URL),
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION);
        val = "(";
        val += sDesc; val += "): "; val += sURL;
         ID3_FreeString(  sURL);
         ID3_FreeString(  sDesc);
//		delete sURL;
//		delete sDesc;
        break;
      }
      case ID3FID_INVOLVEDPEOPLE:
      {
        size_t nItems = frame->GetField(ID3FN_TEXT)->GetNumTextItems();
        for (size_t nIndex = 0; nIndex < nItems; nIndex++)
        {
          char *sPeople = ID3_GetString(frame, ID3FN_TEXT, nIndex);
          val = sPeople;
           ID3_FreeString(  sPeople);
//		  delete sPeople;
          if (nIndex + 1 < nItems)
          {
            val += ", ";
          }
        }
//        out += "\r\n";
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
        val = "("; val += sDesc; val += ")["; val += sFormat; val += ", ";
        val += nPicType; val += "]: ";val += sMimeType;val += ", ";val += nDataSize;
        val += " bytes";

         ID3_FreeString(  sMimeType);
         ID3_FreeString(  sDesc);
         ID3_FreeString(  sFormat);
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
        val = "(";
		val += sDesc;
		val += ")[" ;
		val += sFileName;val += "]: "; val += sMimeType;val += ", ";val += nDataSize;
        val += " bytes";
         ID3_FreeString(  sMimeType);
         ID3_FreeString(  sDesc);
         ID3_FreeString(  sFileName);

        break;
      }

      case ID3FID_UNIQUEFILEID:
      {
        CString sOwner = id3_GetStringUFID(frame, ID3FN_TEXT);
        if (sOwner.GetLength()) {
            size_t nDataSize = frame->GetField(ID3FN_DATA)->Size();
              val = sOwner;
        }
        break;
      }
      case ID3FID_PLAYCOUNTER:
      {
        size_t nCounter = frame->GetField(ID3FN_COUNTER)->Get();
        val = nCounter ;
        break;
      }
      case ID3FID_POPULARIMETER:
      {
        char *sEmail = ID3_GetString(frame, ID3FN_EMAIL);
        size_t
        nCounter = frame->GetField(ID3FN_COUNTER)->Get(),
        nRating = frame->GetField(ID3FN_RATING)->Get();
        val += sEmail;val += ", counter="; 
		val += nCounter; val += " rating=";val += nRating;
        ID3_FreeString(  sEmail);
        break;
      }
      case ID3FID_CRYPTOREG:
      case ID3FID_GROUPINGREG:
      {
        char *sOwner = ID3_GetString(frame, ID3FN_OWNER);
        size_t 
        nSymbol = frame->GetField(ID3FN_ID)->Get(),
        nDataSize = frame->GetField(ID3FN_DATA)->Size();
        val = "("; val += nSymbol;val += "): "; val += sOwner;
		val += ", "; val += nDataSize; val += " bytes";
        break;
      }
      case ID3FID_SYNCEDLYRICS:
      {
        char 
        *sDesc = ID3_GetString(frame, ID3FN_DESCRIPTION), 
        *sLang = ID3_GetString(frame, ID3FN_LANGUAGE);
        size_t nTimestamp = frame->GetField(ID3FN_TIMESTAMPFORMAT)->Get();
        size_t nRating = frame->GetField(ID3FN_CONTENTTYPE)->Get();
        const char* format = (2 == nTimestamp) ? "ms" : "frames";
        val = "(";val += sDesc;val += ")["; val += sLang; val += "]: ";
        switch (nRating)
        {
          case ID3CT_OTHER:    val += "Other"; break;
          case ID3CT_LYRICS:   val += "Lyrics"; break;
          case ID3CT_TEXTTRANSCRIPTION:     val += "Text transcription"; break;
          case ID3CT_MOVEMENT: val += "Movement/part name"; break;
          case ID3CT_EVENTS:   val += "Events"; break;
          case ID3CT_CHORD:    val += "Chord"; break;
          case ID3CT_TRIVIA:   val += "Trivia/'pop up' information"; break;
        }

        ID3_Field* fld = frame->GetField(ID3FN_DATA);
        if (fld)
        {
          ID3_MemoryReader mr(fld->GetRawBinary(), fld->BinSize());
          while (!mr.atEnd())
          {
            val += io::readString(mr).c_str();
            val += " ["; val += io::readBENumber(mr, sizeof(uint32)); val += " " ;
			val += format; val += "] ";
          }
        }

         ID3_FreeString(  sDesc);
         ID3_FreeString(  sLang);
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
        val = "(unimplemented)";
        break;
      }
      default:
      {
			if ("RVA2" == key) {
				size_t s = frame->GetDataSize();
				if (7 != s) {// not a Media Monkey rva2
					logger.log("found RVA2 but unknown format");
					break;
				}
				unsigned char * data = new BYTE[ s ];
				memcpy(data, frame->GetField(ID3FN_DATA)->GetRawBinary(),s);
//				BYTE * buf = new BYTE[ (s * 3)+1 ];
//				char * p = (char*)buf;
//				for (int i = 0 ; i < s; ++i) {
//					sprintf(p,"%02x ",data[i]);
//					p += 3;
//				}
//				buf[(s*3)] = 0;
//				val = "hex: "+CS(buf);
//				delete buf;

				if (1 != data[1]) {
					delete data;
					logger.log("found RVA2 but unknown format");
					break;
				}

				int rgfixed;
				double rgfloat;
				rgfixed = (data[3] << 8) | (data[2] << 0);
				rgfixed |= -(rgfixed & 0x8000);
				rgfloat = (double) rgfixed / 512;

				val = NTS(rgfloat);

//				unsigned int peak_bits;
//				peak_bits = data[4];
//				if (16 != peak_bits) {
//					delete data;
//					break;
//				}
//				rgfixed = (data[6] << 8) | (data[5] << 0);
//				rgfixed |= -(rgfixed & 0x8000);
//				rgfloat = (double) rgfixed / 512;
//
//				val += " " + NTS(rgfloat);

				delete data;
			} else {
				val = NTS(eFrameID) + " unknown ";
				size_t s = frame->GetDataSize();
				val += NTS(s);
			}
        break;
      }
    }
	if (key.GetLength() && val.GetLength())
		tag->setVal(key,val);
  }
    delete iter;

#ifdef adsf
	AutoBuf buf(2000);
    Mp3Header mpg;
    if (mpg.mb_getFilePath(file) == TRUE) { 
		
		int chpos;
        chpos = sprintf(buf.p, "\r\nDuration    %d\r\n", mpg.getLengthInSeconds());
        chpos += sprintf((char*)buf.p + chpos, "Size       %d\r\n", mpg.getSize());
        chpos += sprintf((char*)buf.p + chpos, "Frames    %d\r\n", mpg.getNumberOfFrames());
        chpos += sprintf((char*)buf.p + chpos, "emphasis   %s\r\n", (LPCTSTR) mpg.getEmphasis());
        chpos += sprintf((char*)buf.p + chpos, "Original   %s\r\n", mpg.getOriginal() == TRUE ? "true" : "false" );
        chpos += sprintf((char*)buf.p + chpos, "Copyright  %s\r\n", mpg.getCopyright() == TRUE ? "true" : "false" );
        chpos += sprintf((char*)buf.p + chpos, "Protection %s\r\n", mpg.getProtection() == TRUE ? "true" : "false" );
        chpos += sprintf((char*)buf.p + chpos, "Version    %f\r\n", mpg.getVersion() );
        chpos += sprintf((char*)buf.p + chpos, "Layer      %d\r\n", mpg.getLayer() );
        chpos += sprintf((char*)buf.p + chpos, "Bitrate    %d\r\n", mpg.getBitrate() );
        chpos += sprintf((char*)buf.p + chpos, "Frequency  %d\r\n", mpg.getFrequency() );
        chpos += sprintf((char*)buf.p + chpos, "Mode       %s\r\n", (LPCTSTR)mpg.getMode() );
		out += buf.p;     
    } else {
        sprintf(buf.p, "No valid mp3 headers found\r\nDoes not appear to be an mp3 file.\r\n");
		out += buf.p;     
    }
#endif
	
    return TRUE;
}

