#ifndef _SOURCELINE_H_MYDIFF
#define _SOURCELINE_H_MYDIFF

class SrcLine : public CObject {
	public:
		SrcLine(const CString & line, int boldf, int numf, int diff);
		const CString & line() { return m_line; }
		BOOL bold() { return (m_bold == 1); }
		BOOL number() { return (m_number == 1); }
		BOOL diff() { return (m_diff == 1); }
		void setBN(const int b, const int n, const int d) {
			m_bold = b;
			m_number = n;
			m_diff = d;
		}
	private:
		CString m_line;
		int m_bold;
		int m_number;
		int m_diff;

};


class SrcLineList : public CObList {
	public:
		SrcLineList();
		~SrcLineList();
		void RemoveAll();
		void insertAt(const int idx, const CString &, int bold
			, int number, int diff);
		void removeAt(const int idx);
		SrcLine * getAt(const int idx);
};

class DiffOb : public CObject {
public:
	int m_line;
	int m_diffNum;
};

#endif
