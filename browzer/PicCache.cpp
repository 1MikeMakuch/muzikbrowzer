
#include "PicCache.h"
#include "TestHarness/TestHarness.h"

PicObj::PicObj(unsigned char * picp, size_t size)
	: m_pic(picp), m_size(size) {}

PicObj::~PicObj() {
	if (m_pic)
		delete m_pic;
}


PicCache::PicCache(int max) : m_max(max) {}

PicCache::~PicCache() {
   	POSITION pos = m_cache.GetStartPosition();
	while (pos)
	{
		CString file;
		PicObj * p;
		m_cache.GetNextAssoc(pos,file, (void *&)p);
		delete p;
	}
}

void
PicCache::write(const CString & file, unsigned char * pic,
				size_t size) {
	m_cache.SetAt(file, new PicObj(pic, size));
}

BOOL
PicCache::read(const CString & file, unsigned char *& pic, size_t & size) {
	PicObj * p;

	if (m_cache.Lookup(file, (void *&)p)) {
		pic = p->m_pic;
		size = p->m_size;
		return TRUE;
	}
	return FALSE;
}

TEST(PicCacheTest, PicTest)
{
	PicCache cache;
	CString file = "file1";
	unsigned char * buf = new unsigned char [10];
	strcpy((char*)buf, "data1");
	cache.write(file, buf, 5);

	size_t size;
	unsigned char * buf2;
	BOOL r = cache.read(file, (unsigned char *&)buf2, size);
	CHECK(size == 5);
	CString test = buf;
	CHECK(test == "data1");
}



