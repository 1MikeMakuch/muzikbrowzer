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
#include "MyID3LibMiscSupport.h"
#include <id3/field.h>
#include <id3/utils.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//using namespace dami;

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


