

#ifndef _MYID3LIB_MISC_SUPPORT_H_
#define _MYID3LIB_MISC_SUPPORT_H_

#include "stdafx.h"
#include <id3/tag.h>

CString id3_GetStringUFID(const ID3_Frame *, ID3_FieldID);
CString id3_GetString(const ID3_Frame * frame, ID3_FieldID);
CString id3_GetString(const ID3_Frame * frame, ID3_FieldID fldName,
                      size_t nIndex);
CString id3_GetArtist(const ID3_Tag * tag);
CString id3_GetAlbum(const ID3_Tag * tag);
CString id3_GetTitle(const ID3_Tag * tag);
CString id3_GetYear(const ID3_Tag * tag);
CString id3_GetComment(const ID3_Tag * tag, const char * desc);
CString id3_GetTrack(const ID3_Tag * tag);
CString id3_GetGenre(const ID3_Tag * tag);
CString id3_GetLyrics(const ID3_Tag * tag);
CString id3_GetLyricist(const ID3_Tag * tag);
CString id3_GetTLEN(const ID3_Tag * tag);
CString id3_GetRVA2(const ID3_Tag * tag);


void Genre_init();

CString Genre_normalize(const CString genre);

int Genre_getInt(CString genre);

void Genre_getGenres(CStringArray &);
void Genre_addGenre(ID3_Tag & id3, const char * genre);

class MBTag;

CString
displayTag(ID3_Tag *id3, BOOL showLabels=FALSE, CString file = "");

BOOL ReadAllTags(ID3_Tag *id3, MBTag * tag);


#endif /* _ID3LIB_MISC_SUPPORT_H_ */
