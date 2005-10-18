/******************************************************************************

  Yet another registry access class

  Fairly restricted - I didn't bother with enumerating or anything beyond
  simple data types.

                      Copyright 1999 Gavin Smyth
                 http://www.beesknees.freeserve.co.uk

******************************************************************************/

#ifndef _h_registry_h_
#define _h_registry_h_
#include "MBGlobals.h"
#include "StdAfx.h"

const static TCHAR RegKeyIrman[] = _T(MBREGKEYIRMAN);
const static TCHAR RegKey[] = _T(MBREGKEY);
const static TCHAR RegKeyPrevVals[] = _T(MBREGKEYLAST);

const static TCHAR RegDbLocation[] = _T( "DbLocation" );
const static TCHAR RegDirKey[] = _T("Dir_");
const static TCHAR RegNumDirs[] = _T("NumDirs");
const static TCHAR RegNumMp3Extensions[] = _T("NumMp3Extensions");
const static TCHAR RegMp3ExtensionsKey[] = _T("Mp3Extension_");

const static TCHAR RegWindowX1[] = _T("TopLeft_x");
const static TCHAR RegWindowY1[] = _T("TopLeft_y");
const static TCHAR RegWindowX2[] = _T("BottomRight_x");
const static TCHAR RegWindowY2[] = _T("BottomRight_y");
const static TCHAR RegWindowMaximized[] = _T("Maximized");
const static TCHAR RegWindowsFontTitles[] = _T("FontTitles");
const static TCHAR RegWindowsFontPanel[] = _T("FontPanel");
const static TCHAR RegWindowsFontColHdr[] = _T("FontColHdr");
const static TCHAR RegWindowsFontCurPlay[] = _T("FontCurPlay");

const static TCHAR RegWindowsColorBkPanel[] = _T("ColorBkPanel");
const static TCHAR RegWindowsColorBkColHdr[] = _T("ColorBkColHdr");
const static TCHAR RegWindowsColorBkNormal[] = _T("ColorBkNormal");
const static TCHAR RegWindowsColorBkHigh[] = _T("ColorBkHigh");
const static TCHAR RegWindowsColorBkSel[] = _T("ColorBkSel");
const static TCHAR RegWindowsColorBkCurPlay[] = _T("ColorBkCurPlay");
const static TCHAR RegWindowsColorTxCurPlay[] = _T("ColorTxCurPlay");
const static TCHAR RegWindowsColorTxPanel[] = _T("ColorTxPanel");
const static TCHAR RegWindowsColorTxColHdr[] = _T("ColorTxColHdr");
const static TCHAR RegWindowsColorTxNormal[] = _T("ColorTxNormal");
const static TCHAR RegWindowsColorTxHigh[] = _T("ColorTxHigh");
const static TCHAR RegWindowsColorTxSel[] = _T("ColorTxSel");
const static TCHAR RegWindowsColorBorder[] = _T("ColorBorder");
const static TCHAR RegWindowsColorBkCtrls[] = _T("ColorBkCtrls");
const static TCHAR RegWindowsColorTxCtrls[] = _T("ColorTxCtrls");
const static TCHAR RegWindowsColorShadowDark[] = _T("ColorShadowDark");
const static TCHAR RegWindowsColorShadowLight[] = _T("ColorShadowLight");
const static TCHAR RegWindowsColorBkPanel2[] = _T("ColorBkPanel2");
const static TCHAR RegRunAtStartup[] =_T("RunAtStartup");
const static TCHAR RegUseGenre[] =_T("UseGenre");
const static TCHAR RegAlbumSort[] = _T("AlbumSort");
const static TCHAR RegWindowsRoundCorners[] = _T("RoundCorners");
const static TCHAR RegWindowsThemeName[] = _T("ThemeName");
const static TCHAR RegWindowsSkinName[] = _T("SkinName");
const static TCHAR RegWindowsBorderWidth[] = _T("BorderWidth");
const static TCHAR RegWindowsPanelWidth[] = _T("PanelWidth");
const static TCHAR RegWindowsPlaylistHeightPct[] = _T("PlaylistHeightPct");
const static TCHAR RegWindowsGenreWidthPct[] = _T("GenreWidthPct");
const static TCHAR RegWindowsBorderHorz[] = _T("BorderHorz");
const static TCHAR RegWindowsBorderVert[] = _T("BorderVert");

class RegistryKey
{
public:
  // Open (or create, if necessary) the key below the specified handle
  RegistryKey( HKEY base, const TCHAR* keyName );
  RegistryKey(CString filename);

  // Close the key
  ~RegistryKey();

  // The read routines below take a default parameter, used if the
  // value isn't found in the registry (or there was an error in
  // opening the key - for my app that's more convenient than bothering
  // to handle that error condition).

  // Read and write character strings
  void Read( const TCHAR* value, TCHAR* data, unsigned long maxSize,
      const TCHAR* deflt  ) const;
  void Write( const TCHAR* value, const TCHAR* data ) const;

  // Read and write long integers
  unsigned long Read( const TCHAR* value, unsigned long deflt ) const;
  void Write( const TCHAR* value, unsigned long data ) const;

  // Read and write binary data
  void Read( const TCHAR* value, void* data, unsigned long size,
      const void* deflt ) const;
  void Write( const TCHAR* value, const void* data, unsigned long size ) const;
  BOOL WriteFile();
  BOOL ReadFile();

private:
  HKEY key;
  CString mFileName;
  CMapStringToString * mKeyValPairs;

  // Return true if successful - used by the public read/write routines above
  bool ReadData( const TCHAR* value, void* data, unsigned long& size,
	  unsigned long type ) const;
  bool WriteData( const TCHAR* value, const void* data, unsigned long size,
	  unsigned long type ) const;

  // Disable copying
  RegistryKey( const RegistryKey& );
  RegistryKey& operator=( const RegistryKey& );
};

#endif // _h_registry_h_
