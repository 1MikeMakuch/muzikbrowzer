#ifndef _GENRE_H_
#define _GENRE_H_


#include "MyString.h"
#include "id3/tag.h"

typedef CMapStringToString GenreTable;

void
Genre_init();

CString
Genre_normalize(const CString genre);

int
Genre_getInt(CString genre);

void Genre_getGenres(CStringList &);
void Genre_addGenre(ID3_Tag & id3, const char * genre);

#endif
