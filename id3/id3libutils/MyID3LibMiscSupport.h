

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


//void id3_Free(void *p);

//ID3_C_EXPORT ID3_Frame* ID3_AddTrack(ID3_Tag*, uchar ucTrack, uchar ucTotal = 0,
//                                     bool replace = false);
//ID3_C_EXPORT ID3_Frame* ID3_AddTrack(ID3_Tag*, uchar ucTrack, bool replace);

#endif /* _ID3LIB_MISC_SUPPORT_H_ */
