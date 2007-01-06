#ifndef _MBTAG_TYPES_WMA_H_
#define _MBTAG_TYPES_WMA_H_

#include "StdAfx.h"
#include "MBTag.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"
#include "MyLog.h"

#include <tchar.h>
#include <wmsdk.h>

// WMA attributes
// Title
// Author
// WM/TrackNumber
// WM/AlbumArtist
// WM/AlbumTitle
// WM/Genre
// WM/GenreID
// WM/Year
// Duration

class MBWmaTag: public MBTagType {
public:
	MBWmaTag() :m_DurationOnly(FALSE){
		m_convertKeys = TRUE;
		m_Wma2id3.setVal("WM/Genre","TCON");
		m_Wma2id3.setVal("WM/AlbumArtist","TPE1");
		m_Wma2id3.setVal("WM/AlbumTitle","TALB");
		m_Wma2id3.setVal("Title","TIT2");
		m_Wma2id3.setVal("WM/TrackNumber","TRCK");
		m_Wma2id3.setVal("WM/Year","TYER");
		m_Wma2id3.setVal("Duration","TLEN");

		m_id32Wma.setVal("TCON","WM/Genre");
		m_id32Wma.setVal("TPE1","WM/AlbumArtist");
		m_id32Wma.setVal("TALB","WM/AlbumTitle");
		m_id32Wma.setVal("TIT2","Title");
		m_id32Wma.setVal("TRCK","WM/TrackNumber");
		m_id32Wma.setVal("TYER","WM/Year");
		m_id32Wma.setVal("COMM","Description"); // for mbtag only
		m_id32Wma.setVal("TLEN",""); // NULL removes it so it doesn't go
		m_id32Wma.setVal("FILE",""); // into the actual Wma tag
	}
	virtual ~MBWmaTag() {};


	virtual BOOL read(MBTag & tags, const CString & file,
		const BOOL xvert);
	virtual BOOL write(MBTag & tags, const CString & file);
	virtual CString getComments(MBTag & tags, double & rggain, const CString & file);
	virtual CString getInfo(MBTag & tags, const CString & file);
	void SetReadDurationOnly() {
		m_DurationOnly = TRUE;
	}

	virtual CString NativeKey2Id3Key(const CString & Wma) {
		if (m_convertKeys && m_Wma2id3.contains(Wma))
			return m_Wma2id3.getVal(Wma);
		else
			return Wma;
	}
	virtual CString Id3Key2NativeKey(const CString & id3) {
		if (m_id32Wma.contains(id3))
			return m_id32Wma.getVal(id3);
		else
			return id3;
	}
private:
	BOOL DeleteTagField(MBTag & tags, const CString & file, const CString & key);
	MyHash m_Wma2id3;
	MyHash m_id32Wma;
	BOOL m_convertKeys;
	BOOL m_DurationOnly;
};

HRESULT EditorOpenFile( WCHAR* pwszInFile, IWMMetadataEditor ** ppEditor,IWMHeaderInfo ** ppHeaderInfo, IWMHeaderInfo3 ** ppHeaderInfo3 );
HRESULT AddAttrib( WCHAR * pwszInFile, WORD wStreamNum, WCHAR * pwszAttribName, WORD wAttribType, WCHAR * pwszAttribValue, WORD wLangIndex );
//HRESULT SetAttrib( WCHAR * pwszInFile, WORD wStreamNum, WCHAR * pwszAttribName, WORD wAttribType, WCHAR * pwszAttribValue );
HRESULT PrintAttributeString( WORD wIndex,WORD wStream,WCHAR * wszName,WMT_ATTR_DATATYPE AttribDataType,WORD wLangID,BYTE * pbValue,DWORD dwValueLen,CString & value);
#ifndef UNICODE
HRESULT ConvertMBtoWC( LPCTSTR ptszInString, LPWSTR *ppwszOutString );
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE( x ) if( NULL != x ){x->Release( );x = NULL;}
#endif // SAFE_RELEASE
#ifndef SAFE_DELETE
#define SAFE_DELETE( x ) if( NULL != x ){delete x;x = NULL;}
#endif // SAFE_DELETE
#ifndef SAFE_ARRAYDELETE
#define SAFE_ARRAYDELETE( x ) if( NULL != x ){delete [] x;x = NULL;}
#endif // SAFE_ARRAYDELETE

BOOL
MBWmaTag::read(MBTag & tags, const CString & file, const BOOL xvert) {
	m_convertKeys = xvert;
	
	CString error="WmaTag::read error ";
	CString out;

	WORD				wStreamNum			= 0;
	WCHAR				* pwszInFile		= (WCHAR*)(LPCTSTR)file;
    HRESULT             hr                  = S_OK;

    IWMMetadataEditor   * pEditor           = NULL;
    IWMHeaderInfo*      pHeaderInfo         = NULL;
	IWMHeaderInfo3*      pHeaderInfo3         = NULL;

    WCHAR               * pwszAttribName    = NULL;
    WORD                wAttribNameLen      = 0;
    WMT_ATTR_DATATYPE   wAttribType;
    BYTE                * pbAttribValue     = NULL;
    DWORD                wAttribValueLen     = 0;
	WORD				pwLangIndex			= 0;

	char buf[500];
	strcpy(buf,(LPCTSTR)file);
    LPTSTR				ptszInFile			= buf;;

	CString val;
    do
    {
#ifndef UNICODE
        hr = ConvertMBtoWC( ptszInFile, &pwszInFile );
        if( FAILED( hr ) )
        {
			out = "ConvertMBtoWC";
            break;
        }
#endif
        hr = EditorOpenFile( pwszInFile, &pEditor, NULL, &pHeaderInfo3);
        if(FAILED( hr ) )
        {
			out = "EditorOpenFile";
            break;
        }

        WORD wAttributeCount = 0;

        hr = pHeaderInfo3->GetAttributeCountEx( wStreamNum, &wAttributeCount );
        if(FAILED( hr ) )
        {
            _stprintf(buf, _T( "WMA:GetAttributeCount failed for stream = %d ( hr=0x%08x ).\n" ), 
                wStreamNum, hr );
			logger.log(buf);
			out = buf;
            break;
        }

        for( WORD wAttribIndex = 0; wAttribIndex < wAttributeCount; wAttribIndex++ )
        {
            SAFE_ARRAYDELETE( pwszAttribName );
            SAFE_ARRAYDELETE( pbAttribValue );

            hr = pHeaderInfo3->GetAttributeByIndexEx(
												   wStreamNum,
												   wAttribIndex,
                                                   pwszAttribName,
                                                   &wAttribNameLen,
                                                   &wAttribType,
												   &pwLangIndex,
                                                   pbAttribValue,
                                                   &wAttribValueLen );
            if( FAILED( hr ) )
            {
                _stprintf(buf, _T( "WMA:GetAttributeByIndex failed for index = %d ( hr=0x%08x ).\n" ), 
                    wAttribIndex, hr );
				logger.log(buf);
				out = buf;
                break;
            }

            pwszAttribName = new WCHAR[ wAttribNameLen ];
            if( NULL == pwszAttribName )
            {
                hr = E_OUTOFMEMORY;
                break;
            }

            pbAttribValue = new BYTE[ wAttribValueLen ];
            if( NULL == pbAttribValue )
            {
                hr = E_OUTOFMEMORY;
                break;
            }

            hr = pHeaderInfo3->GetAttributeByIndexEx(
												   wStreamNum,
												   wAttribIndex,
                                                   pwszAttribName,
                                                   &wAttribNameLen,
                                                   &wAttribType,
												   &pwLangIndex,
                                                   pbAttribValue,
                                                   &wAttribValueLen );


            if( FAILED( hr ) )
            {
                _stprintf(buf, _T( "WMA:GetAttributeByIndex failed for index = %d ( hr=0x%08x ).\n" ), 
                    wAttribIndex, hr );
				logger.log(buf);
				out = buf;
                break;
            }
			CString key = pwszAttribName;

            hr = PrintAttributeString( wAttribIndex, 
                                 wStreamNum, 
                                 pwszAttribName, 
                                 wAttribType, 
                                 0, 
                                 pbAttribValue, 
                                 wAttribValueLen, val);
            if( FAILED( hr ) )
            {
				out = "PrintAttributeString";
                break;
            }
			if ("Duration" == key && m_convertKeys) {
				if (val.GetLength()) {
					float d = atof(val);
					if (d > 10000) {
						d = d / 10000; // milliseconds
						int d2 = (int) d;
						val = numToString(d2);
					}
				}
			}
			key = NativeKey2Id3Key(key);
			tags.setVal(key, val);
			if (m_DurationOnly && "TLEN" == key) {
				break;
			}
			if (tags.m_KeyCounter && tags.IsAnMBKey(key)) {
				CString tmp = tags.m_KeyCounter->getVal(key);
				int c = atoi(tmp);
				c++;
				tags.m_KeyCounter->setVal(key,NTS(c));
			}
			if (tags.GettingInfo())
				tags.AppendInfo(key,val);
			if (tags.GettingComments() 
				&& (0 == key.CompareNoCase("DESCRIPTION")
				|| 0 == key.CompareNoCase("COMMENTS")))
				tags.AppendComments(val);
        }
        
        hr = pEditor->Close();
        if( FAILED( hr ) )
        {
            _stprintf(buf, _T( "WMA:Could not close the file %ws ( hr=0x%08x ).\n" ), pwszInFile, hr );
			logger.log(buf);
            break;
        }
    }
    while( FALSE );
    
    SAFE_RELEASE( pHeaderInfo );
    SAFE_RELEASE( pEditor );

    SAFE_ARRAYDELETE( pwszAttribName );
    SAFE_ARRAYDELETE( pbAttribValue );
	SAFE_ARRAYDELETE(pwszInFile );

	m_convertKeys = TRUE;
	m_DurationOnly = FALSE;
	return TRUE;
}

BOOL
MBWmaTag::DeleteTagField(MBTag & tags, const CString &file, const CString &key) {
	WORD				wStreamNum			= 0;
	WCHAR				* pwszInFile		= (WCHAR*)(LPCTSTR)file;
    HRESULT             hr                  = S_OK;

    IWMMetadataEditor   * pEditor           = NULL;
    IWMHeaderInfo*      pHeaderInfo         = NULL;
	IWMHeaderInfo3*      pHeaderInfo3         = NULL;

    WCHAR               * pwszAttribName    = NULL;
    WORD                wAttribNameLen      = 0;
    BYTE                * pbAttribValue     = NULL;
    DWORD                wAttribValueLen     = 0;
	WORD				pwLangIndex			= 0;
	WORD				pwCount				= 0;
	WORD				* pwIndices			= 0;

	char buf[500];
	strcpy(buf,(LPCTSTR)file);
    LPTSTR				ptszInFile			= buf;;

	CString val,out;

	do {
		hr = ConvertMBtoWC( ptszInFile, &pwszInFile );
		if( FAILED( hr ) )
		{
			out = "ConvertMBtoWC";
			break;
		}

		hr = EditorOpenFile( pwszInFile, &pEditor, NULL, &pHeaderInfo3);
		if(FAILED( hr ) )
		{
			out = "EditorOpenFile";
			break;
		}

		LPTSTR  ptszAttribName  = (char*)(LPCTSTR)key;
		ConvertMBtoWC(ptszAttribName, &pwszAttribName);

		hr = pHeaderInfo3->GetAttributeIndices(
								wStreamNum,
								pwszAttribName,
								NULL,
								NULL,
								&pwCount);
		if (FAILED(hr)) {
			break;
		}
		pwIndices = new WORD[ pwCount ];
		hr = pHeaderInfo3->GetAttributeIndices(
								wStreamNum,
								pwszAttribName,
								NULL,
								pwIndices,
								&pwCount);
		if (FAILED(hr)) {
			break;
		}
		SAFE_ARRAYDELETE( pwszAttribName );

		WORD idx;
		int i;
		for(i = pwCount-1 ; i >= 0 ; --i) {
			idx = pwIndices[i];
			hr = pHeaderInfo3->DeleteAttribute(wStreamNum, idx);
			if (FAILED(hr)) {
				break;
			}
		}
		SAFE_ARRAYDELETE( pwIndices );
		hr = pEditor->Flush();
		if( FAILED( hr ) )
        {
            _stprintf(buf, _T( "WMA:Could not close the file %ws ( hr=0x%08x ).\n" ), pwszInFile, hr );
			logger.log(buf);
            break;
        }
        hr = pEditor->Close();
        if( FAILED( hr ) )
        {
            _stprintf(buf, _T( "WMA:Could not close the file %ws ( hr=0x%08x ).\n" ), pwszInFile, hr );
			logger.log(buf);
            break;
        }
	} while (FALSE);

	pEditor->Close();
	SAFE_ARRAYDELETE( pwIndices );
    SAFE_RELEASE( pHeaderInfo );
    SAFE_RELEASE( pEditor );

    SAFE_ARRAYDELETE( pwszAttribName );
    SAFE_ARRAYDELETE( pbAttribValue );
	SAFE_ARRAYDELETE(pwszInFile );

	return (! FAILED(hr));

}









BOOL
MBWmaTag::write(MBTag & tags, const CString &file) {

	if (tags.IsDeleteTag()) {
		return FALSE; // can't do it with wma's.
	} else if (tags.IsDeleteKeys()) {
		CStringList dkeys;
		tags.GetDeleteKeys(dkeys);
		BOOL r = TRUE;
		for(POSITION pos = dkeys.GetHeadPosition(); pos != NULL; dkeys.GetNext(pos)) {
			r &= DeleteTagField(tags, file, dkeys.GetAt(pos));
		}
		return r;
	}

	CString error="WmaTag::write error ";
	CString out;
	char buf[500];

	WORD				wStreamNum			= 0;
	WCHAR				* pwszInFile		= NULL;//(WCHAR*)(LPCTSTR)m_file;
    HRESULT             hr                  = S_OK;

    IWMMetadataEditor   * pEditor           = NULL;
    IWMHeaderInfo		* pHeaderInfo       = NULL;
    IWMHeaderInfo3      * pHeaderInfo3		= NULL;

    WCHAR               * pwszAttribName    = NULL;
    WORD                wAttribNameLen      = 0;
    WMT_ATTR_DATATYPE   wAttribType			= (WMT_ATTR_DATATYPE)1;
    BYTE                * pbAttribValue     = NULL;
    WORD                wAttribValueLen     = 0;
    DWORD               dwAttribValueLen	= 0;
    WORD				wLangIndex			= 0;

	strcpy(buf,(LPCTSTR)file);
    LPTSTR				ptszInFile			= buf;

	WCHAR   * pwszAttribValue   = NULL;
	WORD				pwCount				= 0;
	WORD				* pwIndices			= 0;

	MBTag oldtag;
	oldtag.read(file);

    do
    {
#ifndef UNICODE
        hr = ConvertMBtoWC( ptszInFile, &pwszInFile );
        if( FAILED( hr ) )
        {
			out += "ConvertMBtoWC";
            break;
        }
#endif
        hr = EditorOpenFile( pwszInFile, &pEditor, NULL,
			&pHeaderInfo3 );
        if(FAILED( hr ) )
        {
			out += "EditorOpenFile";
            break;
        }

		POSITION pos;
		CString key,val,oldkey,oldval;
		for(pos = tags.GetSortedHead(); pos != NULL;) {
			tags.GetNextAssoc(pos, key, val);
			oldval = oldtag.getVal(key);
			key = Id3Key2NativeKey(key);
			// Only modify fields the user has changed.
			if (key.GetLength() && oldval != val) {
				// set
				LPTSTR  ptszAttribName  = (char*)(LPCTSTR)key;
				LPTSTR  ptszAttribValue = (char*)(LPCTSTR)val;

				ConvertMBtoWC(ptszAttribName, &pwszAttribName);
				ConvertMBtoWC(ptszAttribValue, &pwszAttribValue);
				wAttribValueLen = ( wcslen( pwszAttribValue) + 1) *
					sizeof (WCHAR);
				pbAttribValue = (BYTE *)pwszAttribValue;
	
				hr = pHeaderInfo3->GetAttributeIndices(
										wStreamNum,
										pwszAttribName,
										NULL,
										NULL,
										&pwCount);
				if (FAILED(hr)) {
					break;
				}
				pwIndices = new WORD[ pwCount ];
				hr = pHeaderInfo3->GetAttributeIndices(
										wStreamNum,
										pwszAttribName,
										NULL,
										pwIndices,
										&pwCount);
				if (FAILED(hr)) {
					break;
				}
				WORD idx;
				int i;
				for(i = pwCount-1 ; i >= 0 ; --i) {
					idx = pwIndices[i];
					hr = pHeaderInfo3->DeleteAttribute(wStreamNum, idx);
					if (FAILED(hr)) {
						break;
					}
				}
				SAFE_ARRAYDELETE( pwIndices );
				WORD pwIdx=0;
				hr = pHeaderInfo3->AddAttribute(
					wStreamNum,
					pwszAttribName,
					&pwIdx,
					WMT_TYPE_STRING,
					0,
					pbAttribValue,
					wAttribValueLen);

				if( FAILED( hr ) )
				{
					out += "SetAttribute:"+key+":"+val;
					break;
				}
			}
#ifndef UNICODE
			SAFE_ARRAYDELETE( pwszAttribName );
			SAFE_ARRAYDELETE( pwszAttribValue );
#endif
		}
		hr = pEditor->Flush();
		if( FAILED( hr ) )
		{
			_stprintf(buf, _T( "WMA:Could not flush the file %ws ( hr=0x%08x ).\n" ), 
				pwszInFile, hr );
			logger.log(buf);
			out += buf;
			break;
		}

		hr = pEditor->Close();
		if( FAILED( hr ) )
		{
			_stprintf(buf, _T( "WMA:Could not close the file %ws ( hr=0x%08x ).\n" ), 
				pwszInFile, hr );
			logger.log(buf);
			out += buf;
			break;
		}
	} while (FALSE);
#ifndef UNICODE
	SAFE_ARRAYDELETE( pwszInFile );
#endif
	if (out != "") {
		logger.log("MBWmaTag::write " + error + out);
		return FALSE;
	}
	return TRUE;
}
CString
MBWmaTag::getComments(MBTag & tags, double & rggain, const CString & file) {
	tags.GettingComments(TRUE);
	read(tags,file,FALSE); // FALSE = don't convert keys
	tags.GettingComments(FALSE);
	CString rg = tags.getVal("REPLAYGAIN_TRACK_GAIN");
	if (rg.GetLength())
		rggain = atof(rg);
	return tags.GetComments();
}
CString
MBWmaTag::getInfo(MBTag & tags, const CString & file) {
	tags.GettingInfo(TRUE);
	read(tags,file,FALSE); // FALSE = don't convert keys
	tags.GettingInfo(FALSE);
	return tags.GetInfo();
	
	CString key,val,comments;
	for(POSITION pos = tags.GetSortedHead(); pos != NULL;) {
		tags.GetNextAssoc(pos,key,val);
		key = Id3Key2NativeKey(key);
		if (key.GetLength())
			comments += key + "=" + val + "\r\n";
	}
	return comments;
}





//------------------------------------------------------------------------------
// Name: EditorOpenFile()
// Desc: Creates metadata editor, opens file, and gets header info interface.
//------------------------------------------------------------------------------
HRESULT EditorOpenFile( WCHAR* pwszInFile, 
                        IWMMetadataEditor ** ppEditor,
                        IWMHeaderInfo ** ppHeaderInfo, 
                        IWMHeaderInfo3 ** ppHeaderInfo3 )
{
    if( ( NULL == pwszInFile ) ||
        ( NULL == ppEditor ) )
    {
        return( E_INVALIDARG );
    }

    HRESULT hr = S_OK;
	char buf[500];

    do
    {
        hr = WMCreateEditor( ppEditor );
        if( FAILED( hr ) )
        {
            _stprintf(buf, _T( "WMA:Could not create Metadata Editor ( hr=0x%08x ).\n" ), hr );
			logger.log(buf);
            break;
        }

        hr = ( *ppEditor )->Open( pwszInFile );
        if( FAILED ( hr ) )
        {
            _stprintf(buf, _T( "WMA:Could not open the file %ws ( hr=0x%08x ).\n" ), 
                pwszInFile, hr );
			logger.log(buf);
            break;
        }

        if( NULL != ppHeaderInfo )
        {
            hr = ( *ppEditor )->QueryInterface( IID_IWMHeaderInfo, 
                                                (void **)ppHeaderInfo );
            if( FAILED( hr ) )
            {
                _stprintf(buf, _T( "WMA:Could not QI for IWMHeaderInfo ( hr=0x%08x ).\n" ), hr );
				logger.log(buf);
                break;
            }
        }

        if( NULL != ppHeaderInfo3 )
        {
            hr = ( *ppEditor )->QueryInterface( IID_IWMHeaderInfo3, 
                                                (void **)ppHeaderInfo3 );
            if( FAILED( hr ) )
            {
                _stprintf(buf, _T( "WMA:Could not QI for IWMHeaderInfo3 ( hr=0x%08x ).\n" ), hr );
				logger.log(buf);
                break;
            }
        }
    }
    while( FALSE );

    return( hr );
}

//------------------------------------------------------------------------------
// Name: AddAttrib()
// Desc: Adds an attribute by using IWMHeaderInfo3::AddAttribute.
//------------------------------------------------------------------------------
HRESULT AddAttrib( WCHAR * pwszInFile, 
                   WORD wStreamNum, 
                   WCHAR * pwszAttribName, 
                   WORD wAttribType, 
                   WCHAR * pwszAttribValue, 
                   WORD wLangIndex )
{
    if( ( NULL == pwszInFile ) ||
        ( NULL == pwszAttribName ) ||
        ( NULL == pwszAttribValue ) )
    {
        return( E_INVALIDARG );
    }
	char buf[500];

    HRESULT hr = S_OK;

    IWMMetadataEditor   * pEditor       = NULL;
    IWMHeaderInfo3      * pHeaderInfo3  = NULL;

    BYTE*               pbAttribValue   = NULL;
    DWORD               dwAttribValueLen = 0;

    WMT_ATTR_DATATYPE   AttribDataType  = ( WMT_ATTR_DATATYPE) wAttribType;

    DWORD               dwAttribValue   = 0;
    WORD                wAttribValue    = 0;
    QWORD               qwAttribValue   = 0;
    BOOL                fAttribValue    = 0;

    do
    {
        hr = EditorOpenFile( pwszInFile, &pEditor, NULL, &pHeaderInfo3 );
        if(FAILED( hr ) )
        {
            break;
        }

        switch( AttribDataType )
        {
        case WMT_TYPE_DWORD:
            dwAttribValue = _wtoi( pwszAttribValue );                               
            dwAttribValueLen = sizeof( DWORD );
            pbAttribValue = (BYTE *)&dwAttribValue;

            break;

        case WMT_TYPE_WORD:
            wAttribValue = (WORD)_wtoi( pwszAttribValue );                                
            dwAttribValueLen = sizeof(WORD);
            pbAttribValue = (BYTE *)&wAttribValue;

            break;

        case WMT_TYPE_QWORD:
            qwAttribValue = _wtoi64( pwszAttribValue );                             
            dwAttribValueLen = sizeof(QWORD );
            pbAttribValue = (BYTE *)&qwAttribValue;

            break;
                    
        case WMT_TYPE_STRING:
            dwAttribValueLen = ( wcslen( pwszAttribValue ) + 1 )* sizeof( WCHAR );
            pbAttribValue = (BYTE *)pwszAttribValue;

            break;

        case WMT_TYPE_BOOL:
            fAttribValue = (BOOL)_wtoi( pwszAttribValue );                              
            dwAttribValueLen = sizeof(BOOL);
            pbAttribValue = (BYTE *)&fAttribValue;

            break;

        default:
            _stprintf(buf,buf, _T( "WMA:Unsupported data type for SetAttribute\n" ) );
			logger.log(buf);
            hr = E_INVALIDARG;
            break;
        }

        hr = pHeaderInfo3->AddAttribute( wStreamNum,
                                         pwszAttribName,
                                         NULL,
                                         AttribDataType, 
                                         wLangIndex,
                                         pbAttribValue,
                                         dwAttribValueLen );
        if( FAILED( hr ) )
        {
            _stprintf(buf,buf, _T( "WMA:AddAttribute failed for Attribute name %ws ( hr=0x%08x ).\n" ), 
                pwszAttribName, hr );
			logger.log(buf);
            break;
        }

        hr = pEditor->Flush();
        if( FAILED( hr ) )
        {
            _stprintf(buf,buf, _T( "WMA:Could not flush the file %ws ( hr=0x%08x ).\n" ), 
                pwszInFile, hr );
			logger.log(buf);
            break;
        }

        hr = pEditor->Close();
        if( FAILED( hr ) )
        {
            _stprintf(buf,buf, _T( "WMA:Could not close the file %ws ( hr=0x%08x ).\n" ), 
                pwszInFile, hr );
			logger.log(buf);
            break;
        }
    }
    while( FALSE );
    
    SAFE_RELEASE( pHeaderInfo3 );
    SAFE_RELEASE( pEditor );

    return( hr );
}
#ifdef asdf
//------------------------------------------------------------------------------
// Name: SetAttrib()
// Desc: Adds an attribute by using IWMHeaderInfo::SetAttribute.
//------------------------------------------------------------------------------
HRESULT SetAttrib( WCHAR * pwszInFile, 
                   WORD wStreamNum, 
                   WCHAR * pwszAttribName, 
                   WORD wAttribType, 
                   WCHAR * pwszAttribValue )
{
    if( ( NULL == pwszInFile ) ||
        ( NULL == pwszAttribName ) ||
        ( NULL == pwszAttribValue ) )
    {
        return( E_INVALIDARG );
    }
	char buf[500];

    HRESULT hr = S_OK;

    IWMMetadataEditor   * pEditor       = NULL;
    IWMHeaderInfo       * pHeaderInfo   = NULL;

    BYTE*               pbAttribValue   = NULL;
    WORD                wAttribValueLen = 0;

    WMT_ATTR_DATATYPE   AttribDataType  = ( WMT_ATTR_DATATYPE ) wAttribType;

    DWORD               dwAttribValue   = 0;
    WORD                wAttribValue    = 0;
    QWORD               qwAttribValue   = 0;
    BOOL                fAttribValue    = 0;

    do
    {
        hr = EditorOpenFile( pwszInFile, &pEditor, &pHeaderInfo, NULL);
        if(FAILED( hr ) )
        {
            break;
        }

        switch( AttribDataType )
        {
        case WMT_TYPE_DWORD:
            dwAttribValue = _wtoi( pwszAttribValue );                               
            wAttribValueLen = sizeof( DWORD );
            pbAttribValue = (BYTE *)&dwAttribValue;

            break;

        case WMT_TYPE_WORD:
            wAttribValue = (WORD)_wtoi( pwszAttribValue );                                
            wAttribValueLen = sizeof(WORD);
            pbAttribValue = (BYTE *)&wAttribValue;

            break;

        case WMT_TYPE_QWORD:
            qwAttribValue = _wtoi64( pwszAttribValue );                             
            wAttribValueLen = sizeof(QWORD );
            pbAttribValue = (BYTE *)&qwAttribValue;

            break;
                    
        case WMT_TYPE_STRING:
            wAttribValueLen = ( wcslen( pwszAttribValue ) + 1 )* sizeof( WCHAR );
            pbAttribValue = (BYTE *)pwszAttribValue;

            break;

        case WMT_TYPE_BOOL:
            fAttribValue = (BOOL)_wtoi( pwszAttribValue );                              
            wAttribValueLen = sizeof(BOOL);
            pbAttribValue = (BYTE *)&fAttribValue;

            break;

        default:
            _stprintf(buf, _T( "WMA:Unsupported data type for SetAttribute\n" ) );
			logger.log(buf);
            hr = E_INVALIDARG;
            break;
        }

        hr = pHeaderInfo->SetAttribute( wStreamNum,
                                        pwszAttribName,
                                        AttribDataType,                                        
                                        pbAttribValue,
                                        wAttribValueLen );
        if( FAILED( hr ) )
        {
            _stprintf(buf, _T( "WMA:SetAttribute failed for Attribute name %ws ( hr=0x%08x ).\n" ), 
                pwszAttribName, hr );
			logger.log(buf);
            break;
        }

        hr = pEditor->Flush();
        if( FAILED( hr ) )
        {
            _stprintf(buf, _T( "WMA:Could not flush the file %ws ( hr=0x%08x ).\n" ), 
                pwszInFile, hr );
			logger.log(buf);
            break;
        }

        hr = pEditor->Close();
        if( FAILED( hr ) )
        {
            _stprintf(buf, _T( "WMA:Could not close the file %ws ( hr=0x%08x ).\n" ), 
                pwszInFile, hr );
			logger.log(buf);
            break;
        }
    }
    while( FALSE );
    
    SAFE_RELEASE( pHeaderInfo );
    SAFE_RELEASE( pEditor );

    return( hr );
}
#endif

//------------------------------------------------------------------------------
// Name: PrintAttribute()
// Desc: Display the specified attribute.
//------------------------------------------------------------------------------
HRESULT PrintAttributeString( WORD wIndex,
                        WORD wStream,
                        WCHAR * wszName,
                        WMT_ATTR_DATATYPE AttribDataType,
                        WORD wLangID,
                        BYTE * pbValue,
                        DWORD dwValueLen,
						CString & value)
{
//    WCHAR pwszValue[256];
//    WCHAR wszNum[256];
    WCHAR pwszValue[5000];
    WCHAR wszNum[5000];

    ZeroMemory( pwszValue, sizeof( pwszValue ) );
    ZeroMemory( wszNum, sizeof( wszNum ) );

    //
    // Make the data type string
    //
    WCHAR * pwszType = L"Unknown";
    WCHAR * pTypes[] = 
    { 
        L"DWORD", 
        L"STRING", 
        L"BINARY", 
        L"BOOL", 
        L"QWORD", 
        L"WORD", 
        L"GUID" 
    };

    if( ( sizeof( pTypes ) / sizeof( WCHAR ) ) > AttribDataType )
    {
        pwszType = pTypes[AttribDataType];
    }

    //
    // The attribute value.
    //

    switch ( AttribDataType )
    {
    //
    // string
    //
    case WMT_TYPE_STRING:
        if( 0 == dwValueLen )
        {
           wcscat( pwszValue, L"***** NULL *****" );
        }
        else
        {
            if( 0xFEFF == *(WCHAR*)pbValue )
            {
               wcscat( pwszValue, L"UTF-16LE BOM+" );
               wcscat( pwszValue, L"\"" );
               if( 4 <= dwValueLen )
               {
                    wcsncat( pwszValue, ( (WCHAR *)pbValue ) + 1, 
                        min( ( dwValueLen / sizeof( WCHAR ) ) - 2, ( sizeof( pwszValue ) / sizeof( WCHAR ) ) - 20 ) );
               }
               wcscat( pwszValue, L"\"" );
            }
            else if( 0xFFFE == *(WCHAR*)pbValue )
            {
               wcscat( pwszValue, L"UTF-16BE BOM+" );
               wcscat( pwszValue, L"\"" );
               if( 4 <= dwValueLen )
               {
                   wcsncat( pwszValue, ( (WCHAR *)pbValue ) + 1, 
                       min( ( dwValueLen / sizeof( WCHAR ) ) - 2, ( sizeof( pwszValue ) / sizeof( WCHAR ) ) - 20 ) );
               }
               wcscat( pwszValue, L"\"" );
            }
            else
            {
//               wcscat( pwszValue, L"\"" );
               if( 2 <= dwValueLen )
               {
                   wcsncat( pwszValue, (WCHAR *)pbValue, 
                       min( ( dwValueLen / sizeof( WCHAR ) ) - 1, ( sizeof( pwszValue ) / sizeof( WCHAR ) ) - 10 ) );
               }
//               wcscat( pwszValue, L"\"" );
            }
        }
        break;

    //
    // Binary
    //
    case WMT_TYPE_BINARY:
        wcscat( pwszValue, L"[" );
        _itow( dwValueLen, wszNum, 10 );
        wcscat( pwszValue, wszNum );
        wcscat( pwszValue, L" bytes]" );
        break;

    //
    // Boolean
    //
    case WMT_TYPE_BOOL:
        if( *( (BOOL* )pbValue ) == FALSE )
        {
            wcscat( pwszValue, L"False" );
        }
        else
        {
            wcscat( pwszValue, L"True" );
        }
        break;

    //
    // DWORD
    //
    case WMT_TYPE_DWORD:
//        swprintf( wszNum, L"%ld, 0x%08lx", 
//            ( (DWORD *)pbValue )[0], ( (DWORD *)pbValue )[0] );
        swprintf( wszNum, L"%ld", 
            ( (DWORD *)pbValue )[0] );
        wcscat( pwszValue, wszNum );
        break;

    //
    // QWORD
    //
    case WMT_TYPE_QWORD:
        _ui64tow( *( (QWORD* )pbValue ), wszNum, 10 );
        wcscat( pwszValue, wszNum );
//        swprintf( wszNum, L", 0x%08lx%08lx", 
//            ( (DWORD *)pbValue )[1], ( (DWORD *) pbValue )[0] );
//        wcscat( pwszValue, wszNum );
        break;

    //
    // WORD
    //
    case WMT_TYPE_WORD:
//        swprintf( wszNum, L"%d, 0x%04x", 
//            ( ( WORD* )pbValue )[0], ( ( WORD* ) pbValue )[0] );
        swprintf( wszNum, L"%d", 
            ( ( WORD* )pbValue )[0]);
        wcscat( pwszValue, wszNum );
        break;

    //
    // GUID
    //
    case WMT_TYPE_GUID:
        if( !StringFromGUID2( *(GUID *)pbValue, pwszValue, 256 ) )
        {
            wcscpy( pwszValue, L"ERROR" );
        }
        break;

    default:
        swprintf( pwszValue, L"Bad data type (%hu): value not displayed", 
            AttribDataType );
        break;
    }
    
    //
    // Dump the string to the screen.
    //
//     _stprintf(buf, _T( "WMA:* %3u  %-25ls %3hu  %3hu  %7ls  %ls\n" ), 
//         wIndex, wszName, wStream, wLangID, pwszType, pwszValue );

	value = pwszValue;

    return( S_OK );
}

//------------------------------------------------------------------------------
// Name: ConvertMBtoWC()
// Desc: Converts a string from multibyte to wide character.
//------------------------------------------------------------------------------
#ifndef UNICODE

HRESULT ConvertMBtoWC( LPCTSTR ptszInString, LPWSTR *ppwszOutString )
{
    if( ptszInString == NULL || ppwszOutString == NULL )
    {
        return( E_INVALIDARG );
    }

    HRESULT hr          = S_OK;
    int     nSizeCount  = 0;

    *ppwszOutString     = NULL;
	char buf[500];

    do
    {
        //
        // Get the memory reqd for this string
        //
        nSizeCount = MultiByteToWideChar( CP_ACP, 0, ptszInString, -1, NULL, 0 );
        if( 0 ==  nSizeCount )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            break;
        }

        *ppwszOutString = new WCHAR[ nSizeCount ];
        if( NULL == *ppwszOutString )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            break;
        }

        if( 0 == MultiByteToWideChar( CP_ACP, 0, ptszInString, -1, *ppwszOutString, nSizeCount ) )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            break;
        }
    }
    while( FALSE );
    
    if( FAILED( hr ) )
    {
        SAFE_ARRAYDELETE( *ppwszOutString );
        _stprintf(buf, _T( "WMA:Internal error ( hr=0x%08x )\n" ), hr );
		logger.log(buf);
    }

    return( hr );
}

#endif // UNICODE

///////////////////////////////////////////////////////////////////////

#endif