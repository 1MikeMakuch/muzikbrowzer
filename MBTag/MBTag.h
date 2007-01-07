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
		void init();
		~MBTag() {}
		static AudioTypeHash * m_types;
		static int addType(const CString fext, MBTagType *);
		MBTagType * getType(FExtension & fext);
		CString getType();

		BOOL read(const CString & file="", const BOOL xvert=TRUE);
		BOOL write();

		virtual void setVal(const CString & key, const CString & val);
		virtual CString getVal(const CString & key);
		void setValId3Key(const CString & key, const CString & val);
		CString getValId3Key(const CString & key);
		void setValNativeKey(const CString & key, const CString & val);
		CString getValNativeKey(const CString & key);
		CString Id3Key2NativeKey(const CString & key);

		void SetDeleteTag() { m_DeleteTag = TRUE;}
		void SetDeleteKey(const CString & key){m_DeleteKeys.setVal(key,"1");}
		BOOL IsDeleteTag() { return m_DeleteTag; }
		BOOL IsDeleteKey(const CString & key) {
			return m_DeleteKeys.contains(key);
		}
		BOOL IsDeleteKeys() { return m_DeleteKeys.GetCount(); }
		void GetDeleteKeys(CStringList & list);

		CString getComments(double & rggain, const CString & file="");
		CString getInfo(const CString & file="");
		BOOL getArt(
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize, 
			const CString & album);
		BOOL setArt(
			const CString & file, 
			unsigned char *& rawdata, 
			size_t & nDataSize);
		void SetType(const CString & type);
		BOOL ReadAllTags() { return m_ReadAllTags;}
		void SetReadAllTags(const BOOL r) { m_ReadAllTags=r;}
		

		BOOL GetExtensions(CStringList & list);
		BOOL HasMultiVals(const CString & file);
		MyHash * m_KeyCounter;
		BOOL IsAnMBKey(const CString & key);
		
		BOOL GettingInfo() { return m_GetInfo;}
		void GettingInfo(const BOOL getit) { m_GetInfo = getit; }
		void AppendInfo(const CString & key, const CString & val) {
			m_info += key + " = " + val + "\r\n" ;
		}
		void ClearInfo() { m_info="";}
		CString GetInfo() { return m_info; }

		BOOL GettingComments() { return m_GetComments;}
		void GettingComments(const BOOL getit) { m_GetComments = getit; }
		void AppendComments(const CString & val) {
			m_comments += val + " ";
		}
		void ClearComments() { m_comments="";}
		CString GetComments() { return m_comments; }

		BOOL VerCompProb(){return m_VerCompProb;}
		CString VerCompDesc() { return m_VerCompDesc;}
		void VerCompDesc(const CString & desc);


	private:
		CString m_file;
		MBTagType * m_tagobj;
		BOOL m_ReadAllTags;
		BOOL m_GetInfo;
		BOOL m_GetComments;
		CString m_info;
		CString m_comments;
		BOOL m_DeleteTag;
		MyHash m_DeleteKeys;
		CString m_VerCompDesc;
		BOOL m_VerCompProb;
		
		
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
	virtual BOOL setArt(
		MBTag & tags,
		const CString & file,
		unsigned char *& rawdata, 
		size_t & nDataSize){return FALSE;};
};

#endif
