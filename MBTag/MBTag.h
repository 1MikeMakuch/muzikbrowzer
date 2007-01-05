#ifndef __MBTAG__H__
#define __MBTAG__H__

#include "StdAfx.h"
#include "MyString.h"

class MBTagType;
class FExtension;
class AudioTypeHash;

// MBTag's are created any time you need to read/write tags.
// They're only held for a short period of time.
class MBTag : public MyHash {

	public:
		MBTag();
		MBTag(const CString & file);
		~MBTag() {}
		static AudioTypeHash * m_types;
		static int addType(const CString fext, MBTagType *);
		MBTagType * getType(FExtension & fext);

		BOOL read(const CString & file="", const BOOL xvert=TRUE);
		BOOL write();

		virtual void setVal(const CString & key, const CString & val);
		virtual CString getVal(const CString & key);

		CString getComments(double & rggain, const CString & file="");
		CString getInfo(const CString & file="");
		BOOL getArt(
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album);
		void SetType(const CString & type);
		BOOL ReadAllTags() { return m_ReadAllTags;}
		void SetReadAllTags(const BOOL r) { m_ReadAllTags=r;}
		
		void setValId3Key(const CString & key, const CString & val);
		CString getValId3Key(const CString & key);
		BOOL GetExtensions(CStringList & list);
		BOOL HasMultiVals(const CString & file);
		MyHash * m_KeyCounter;
		BOOL IsAnMBKey(const CString & key);
	private:
		CString m_file;
		MBTagType * m_tagobj;
		BOOL m_ReadAllTags;
		
		
};

//
// MBTagType is only for read/write, there is only 1 instance per type,
// not per file. These are instantiated at startup - pre-main, not
// when reading tags from files.
//
class MBTagType {
public:
	MBTagType(){};
	virtual ~MBTagType(){};
	
	// pure virtuals
	virtual BOOL read(MBTag & tags, const CString & file,
						const BOOL xvert=TRUE)=0;
	virtual BOOL write(MBTag & tags, const CString & file)=0;
	
	virtual CString getComments(MBTag & tags, double & rggain,
		const CString & file)=0;
	virtual CString getInfo(MBTag & tags, const CString & file)=0;
	virtual CString Id3Key2NativeKey(const CString & key)=0;
	virtual CString NativeKey2Id3Key(const CString & key)=0;
	
	// optional
	virtual BOOL getArt(
			MBTag & tags,
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album);
};

#endif
