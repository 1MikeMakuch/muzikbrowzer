#ifndef _PICCACHE_H_
#define _PICCACHE_H_

#include "stdafx.h"

class PicCache;

class PicObj {
	friend PicCache;
public:
	PicObj(unsigned char * pic, size_t size);
	~PicObj();
protected:
	unsigned char * m_pic;
	size_t m_size;

};


class PicCache {
public:
	PicCache(int max = 1000);
	~PicCache();

	void write(const CString & file, unsigned char * pic, size_t size);
	BOOL read(const CString & file, unsigned char *& pic, size_t & size);
private:
	CMapStringToPtr m_cache;
	int m_max;
};

#endif
