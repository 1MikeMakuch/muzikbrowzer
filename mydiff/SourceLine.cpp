
#include "StdAfx.h"
#include "SourceLine.h"
#include "TestHarness.h"

SrcLine::SrcLine(const CString & line, int boldf, int numf, int diff)
			: m_line(line), m_bold(boldf), m_number(numf), m_diff(diff)
{}



SrcLineList::SrcLineList() {}
SrcLineList::~SrcLineList() {
	RemoveAll();
}

void
SrcLineList::RemoveAll() {
	POSITION pos;
	for(pos = GetHeadPosition(); pos != NULL ; ) {
		SrcLine * p = (SrcLine * ) GetAt(pos);
		delete p;
		GetNext(pos);
	}
	CObList::RemoveAll();
}

TEST(SrcLineList, slltest)
{
	SrcLine * sl = new SrcLine("line one", 0, 1, 0 );
	SrcLineList sll;
	sll.AddTail(sl);

	POSITION pos;
	for(pos = sll.GetHeadPosition(); pos != NULL;) {
		SrcLine *p = (SrcLine*)sll.GetAt(pos);
		p->line();
		p->bold();
		p->number();
		sll.GetNext(pos);
	}

	sll.RemoveAll();
}

void
SrcLineList::insertAt(const int idx, const CString & line,
						 int bold, int number, int d) {
	SrcLine * p = new SrcLine(line, bold, number, d);
	POSITION pos;
	int lctr = 0;
	for(pos = GetHeadPosition() ; pos != NULL && lctr++ != idx ;
		GetNext(pos) ) {}

	if (pos) {
		InsertBefore(pos, p);
	} else {
		AddTail(p);
	}

}
void
SrcLineList::removeAt(const int idx) {
	POSITION pos;
	int lctr = 0;
	for(pos = GetHeadPosition() ; pos != NULL && lctr++ != idx ;
		GetNext(pos) ) {}

	if (pos) {
		SrcLine * sl = (SrcLine*)GetAt(pos);
		RemoveAt(pos);
		delete sl;
	}

}
SrcLine *
SrcLineList::getAt(const int idx) {
	POSITION pos;
	int lctr = 0;
	for(pos = GetHeadPosition() ; pos != NULL && lctr++ != idx ;
		GetNext(pos) ) {}

	if (pos) {
		return (SrcLine*)GetAt(pos);
	}
	return NULL;
}


TEST(SrcLineList, insertAtTest)
{

	SrcLineList sll;
	sll.AddTail(new SrcLine("two", 0,0,0));
	sll.AddTail(new SrcLine("three", 0,0,0));
	sll.insertAt(0, "one", 0,0,0);

	POSITION pos;
	for(pos = sll.GetHeadPosition() ; pos != NULL;) {
		SrcLine * p = (SrcLine * ) sll.GetAt(pos);
		p->line();
		sll.GetNext(pos);
	}

	sll.removeAt(1);
	for(pos = sll.GetHeadPosition() ; pos != NULL;) {
		SrcLine * p = (SrcLine * ) sll.GetAt(pos);
		p->line();
		sll.GetNext(pos);
	}

	SrcLine * p = sll.getAt(1);
	p->line();
}

