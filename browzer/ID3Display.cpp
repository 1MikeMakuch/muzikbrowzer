// ID3Display.cpp : implementation file
//

#include "stdafx.h"
#include "id3/tag.h"
#include "ID3Display.h"
//#include "ace/SString.h"
#include <id3/utils.h>
#include <id3/misc_support.h>
#include <id3/readers.h>
#include <id3/io_helpers.h>
#include "MyID3LibMiscSupport.h"
#include "Mp3Header.h"
#include "MyString.h"
#include "FExtension.h"
#include "oggtagger/oggtagger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dami;

/////////////////////////////////////////////////////////////////////////////
// ID3Display dialog


ID3Display::ID3Display(Song song, CWnd* pParent /*=NULL*/)
	: CDialog(ID3Display::IDD, pParent), m_Song(song)
{
	//{{AFX_DATA_INIT(ID3Display)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ID3Display::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ID3Display)
	DDX_Control(pDX, IDC_ID3_DISPLAY, m_ID3Display);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ID3Display, CDialog)
	//{{AFX_MSG_MAP(ID3Display)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ID3Display message handlers

BOOL ID3Display::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ID3Display.SetWindowText(displayTag(m_Song));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString
ID3Display::displayTag(Song song) {
	CString file = song->getId3(CS("FILE"));
	FExtension fext(file);
	if (fext.ext() == "mp3") {
		ID3_Tag id3;
		id3.Link(file, ID3TT_ALL);
		CString msg = displayTag(&id3);
		msg += "\r\n";
		msg += file;
		return msg;
	} else if (fext.ext() == "ogg") {
		OggTag ogg(file);
		return ogg.getInfo();
	}
	return "";
}

CString
ID3Display::displayTag(ID3_Tag *id3) {

  CString out,filelabel,file;
  ID3_Tag::ConstIterator* iter = (ID3_Tag::ConstIterator *)id3->CreateIterator();
  const ID3_Frame* frame = NULL;
  out = "Id3 tag:\r\n";
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
    if (mpg.getFilePath(file) == TRUE) { 
     
		int chpos;
        chpos = sprintf(buf.p, "\r\nDuration    %d\r\n", mpg.getLengthInSeconds());
        chpos += sprintf((char*)buf.p + chpos, "Size       %d\r\n", mpg.getSize());
        chpos += sprintf((char*)buf.p + chpos, "Frames    %d\r\n", mpg.getNumberOfFrames());
//        chpos += sprintf((char*)buf.p + chpos, "emphasis   %s\r\n", (LPCTSTR) mpg.getEmphasis());
//        chpos += sprintf((char*)buf.p + chpos, "Original   %s\r\n", mpg.getOriginal() == TRUE ? "true" : "false" );
//        chpos += sprintf((char*)buf.p + chpos, "Copyright  %s\r\n", mpg.getCopyright() == TRUE ? "true" : "false" );
//        chpos += sprintf((char*)buf.p + chpos, "Protection %s\r\n", mpg.getProtection() == TRUE ? "true" : "false" );
        chpos += sprintf((char*)buf.p + chpos, "Version    %f\r\n", mpg.getVersion() );
        chpos += sprintf((char*)buf.p + chpos, "Layer      %d\r\n", mpg.getLayer() );
        chpos += sprintf((char*)buf.p + chpos, "Bitrate    %d\r\n", mpg.getBitrate() );
        chpos += sprintf((char*)buf.p + chpos, "Frequency  %d\r\n", mpg.getFrequency() );
        chpos += sprintf((char*)buf.p + chpos, "Mode       %s\r\n", (LPCTSTR)mpg.getMode() );
		out += buf.p;     
#ifdef _DEBUG
    } else {
        sprintf(buf.p, "No valid mp3 headers found\r\nDoes not appear to be an mp3 file.\r\n");
		out += buf.p;     
#endif
    }
	
    return out;
}
