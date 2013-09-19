#include "StdAfx.h"
//#include "utf8.h"
//#include "util/MyString.h"
#include "TestHarness/TestHarness.h"
#include "WmaTagger.h"
#include "MyLog.h"
#include <afxtempl.h>
#include "SortedArray.h"

//******************************************************************************
//
// Microsoft Windows Media
// Copyright ( C) Microsoft Corporation. All rights reserved.
//
// FileName: main.cpp
//
// Abstract: Sample to edit and view metadata from a windows media file.
//
//******************************************************************************

#include <tchar.h>
//#include <stdio.h>
//#include <string.h>
#include <wmsdk.h>

//------------------------------------------------------------------------------
// Macros
// ------------------------------------------------------------------------------
#ifndef SAFE_RELEASE

#define SAFE_RELEASE( x )       \
    if( NULL != x )             \
    {                           \
        x->Release( );          \
        x = NULL;               \
    }

#endif // SAFE_RELEASE

#ifndef SAFE_DELETE

#define SAFE_DELETE( x )        \
    if( NULL != x )             \
    {                           \
        delete x;               \
        x = NULL;               \
    }

#endif // SAFE_DELETE

#ifndef SAFE_ARRAYDELETE

#define SAFE_ARRAYDELETE( x )   \
    if( NULL != x )             \
    {                           \
        delete [] x;            \
        x = NULL;               \
    }

#endif // SAFE_ARRAYDELETE

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

WmaTag::WmaTag() {}
WmaTag::WmaTag(const CString & file) : m_file(file) {
	read(file);
}

CString
WmaTag::read(const CString & file, const BOOL durationOnly) {
//	logger.logd("WmaTag::read:",file);
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
			setVal(key, val);
			if (durationOnly && "Duration" == key) {
				break;
			}
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

//	if (out != "") {
//		return CString(error + out);
//	} else {
		return "";
//	}
}



CString
WmaTag::getInfo() {
	CSortedArray<CString, CString &> tagsList;
	POSITION pos;
	CString out;
	
	CString key,val;
	for(pos = m_tags.GetStartPosition(); pos != NULL; ) {
		m_tags.GetNextAssoc(pos, key, val);
		tagsList.Add(key);
	}
	tagsList.SetCompareFunction(String::CompareCase);
	tagsList.Sort();

	for (int i=0; i < tagsList.GetSize(); i++) {
		CString& rkey = tagsList.ElementAt(i);
		CString val = getVal(rkey);
		out += rkey + " = " + val + "\r\n";
	}


//	POSITION pos;
//	CString key,val;
//	for(pos = m_tags.GetStartPosition(); pos != NULL; ) {
//		m_tags.GetNextAssoc(pos, key, val);
//		out += key;
//		out += "=";
//		out += val;
//		out += "\r\n";
//	}
	return out;
}


CString
WmaTag::getVal(const CString & srcKey) {

	CString val;
	CString key(srcKey);
	if (m_tags.Lookup(key, val) != 0) {

		return val;
	} else {
		return "";
	}
}
BOOL
WmaTag::exists(const CString & srcKey) {
	CString val;
	CString key(srcKey);
	if (m_tags.Lookup(key, val) != 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

// WMA attributes
// Title
// Author
// WM/TrackNumber
// WM/AlbumTitle
// WM/Genre
// WM/GenreID

void
WmaTag::setVal(const CString & srcKey, const CString & srcVal) {
	if (srcKey.GetLength() && srcVal.GetLength()) {
		CString key(srcKey);
		CString val(srcVal);

		// Not sure the best thing to do here, so I'm getting the COMM
		// (or any other tag) with the most info in it!
		if (exists(key)) {
			CString tmp = getVal(key);
			if (tmp.GetLength() < val.GetLength())
			m_tags.SetAt(key, val);
		} else {
			if ("Duration" == key) {
				if (val.GetLength()) {
					float d = atof(val);
					if (d > 10000) {
						d = d / 10000;
						int d2 = (int)d;
						val = numToString(d2);
					}
				}
			}
			m_tags.SetAt(key, val);
		}
	}
}

CString
WmaTag::write() {
	CString error="WmaTag::write error ";
	CString out;
	char buf[500];
	WmaTag old(m_file);

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

	strcpy(buf,(LPCTSTR)m_file);
    LPTSTR				ptszInFile			= buf;

	WCHAR   * pwszAttribValue   = NULL;

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
        hr = EditorOpenFile( pwszInFile, &pEditor, &pHeaderInfo,
			&pHeaderInfo3 );
        if(FAILED( hr ) )
        {
			out += "EditorOpenFile";
            break;
        }

		POSITION pos;
		CString key,val,oldval;
		for(pos = m_tags.GetStartPosition(); pos != NULL;) {
			m_tags.GetNextAssoc(pos, key, val);
			oldval = old.getVal(key);
			if (val != oldval) {
				if (old.exists(key)) {
					// set
					LPTSTR  ptszAttribName  = (char*)(LPCTSTR)key;
					LPTSTR  ptszAttribValue = (char*)(LPCTSTR)val;

					ConvertMBtoWC(ptszAttribName, &pwszAttribName);
					ConvertMBtoWC(ptszAttribValue, &pwszAttribValue);
					wAttribValueLen = ( wcslen( pwszAttribValue) + 1) *
						sizeof (WCHAR);
					pbAttribValue = (BYTE *)pwszAttribValue;
		
					hr = pHeaderInfo->SetAttribute( wStreamNum,
													pwszAttribName,
													wAttribType,                                        
													pbAttribValue,
													wAttribValueLen );

					if( FAILED( hr ) )
					{
						out += "SetAttribute";
						break;
					}
				} else {
					// add
					LPTSTR  ptszAttribName  = (char*)(LPCTSTR)key;
					LPTSTR  ptszAttribValue = (char*)(LPCTSTR)val;
					ConvertMBtoWC(ptszAttribName, &pwszAttribName);
					ConvertMBtoWC(ptszAttribValue, &pwszAttribValue);
					dwAttribValueLen = ( wcslen( pwszAttribValue) + 1) *
						sizeof (WCHAR);
					pbAttribValue = (BYTE *)pwszAttribValue;
		
					hr = pHeaderInfo3->AddAttribute( wStreamNum,
													 pwszAttribName,
													 NULL,
													 wAttribType, 
													 wLangIndex,
													 pbAttribValue,
													 dwAttribValueLen );
					if( FAILED( hr ) )
					{
						out += "AddAttribute";
//						break;
					}
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
		return CString(error + out);
	} else {
		return "";
	}
}
