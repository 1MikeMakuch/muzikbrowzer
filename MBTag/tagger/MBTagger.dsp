# Microsoft Developer Studio Project File - Name="MBTagger" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MBTagger - Win32 DebugBC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MBTagger.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MBTagger.mak" CFG="MBTagger - Win32 DebugBC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MBTagger - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MBTagger - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "MBTagger - Win32 DebugBC" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MBTagger - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".." /I "..\..\util" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 nafxcw.lib libcmt.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  ..\..\3rdparty\\WMFSDK9\lib\wmvcore.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "MBTagger - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I ".." /I "..\..\util" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "FLAC__NO_DLL" /FR /Yu"stdafx.h" /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nafxcwd.lib libcmtd.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\replaygain_analysis_static.lib ..\..\TestHarness\Debug\TestHarness.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\libFLAC_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\libFLAC++_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\getopt_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\grabbag_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\utf8_static.lib ..\Debug\MBTag.lib Version.lib ..\..\3rdparty\ogg\libogg-1.0\win32\Debug\ogg_static_d.lib ..\..\3rdparty\ogg\libvorbis-1.0\win32\Debug\vorbis_static_d.lib ..\..\3rdparty\ogg\libvorbis-1.0\win32\Debug\vorbisfile_static_d.lib rpcrt4.lib ..\..\3rdparty\id3lib-3.8.3\libprj\Debug\id3libD.lib ..\..\3rdparty\id3lib-3.8.3\zlib\prj\Debug\zlibD.lib strmiids.lib ..\..\id3\id3libutils\Debug\id3utils.lib ..\..\util\Debug\util.lib dxguid.lib Quartz.lib ..\..\3rdparty\\WMFSDK9\lib\wmvcore.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /nodefaultlib:"msvcrt" /nodefaultlib:"libcmt" /nodefaultlib:"nafxcw" /out:"c:\mkm\bin/mbtag.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MBTagger - Win32 DebugBC"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugBC"
# PROP BASE Intermediate_Dir "DebugBC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugBC"
# PROP Intermediate_Dir "DebugBC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I ".." /I "..\..\util" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "FLAC__NO_DLL" /FR /Yu"stdafx.h" /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I ".." /I "..\..\util" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "FLAC__NO_DLL" /FR /Yu"stdafx.h" /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 nafxcwd.lib libcmtd.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\replaygain_analysis_static.lib ..\..\TestHarness\Debug\TestHarness.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\libFLAC_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\libFLAC++_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\getopt_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\grabbag_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\utf8_static.lib ..\Debug\MBTag.lib Version.lib ..\..\3rdparty\ogg\libogg-1.0\win32\Debug\ogg_static_d.lib ..\..\3rdparty\ogg\libvorbis-1.0\win32\Debug\vorbis_static_d.lib ..\..\3rdparty\ogg\libvorbis-1.0\win32\Debug\vorbisfile_static_d.lib rpcrt4.lib ..\..\3rdparty\id3lib-3.8.3\libprj\Debug\id3libD.lib ..\..\3rdparty\id3lib-3.8.3\zlib\prj\Debug\zlibD.lib strmiids.lib ..\..\id3\id3libutils\Debug\id3utils.lib ..\..\util\Debug\util.lib dxguid.lib Quartz.lib ..\..\3rdparty\\WMFSDK9\lib\wmvcore.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /nodefaultlib:"msvcrt" /nodefaultlib:"libcmt" /nodefaultlib:"nafxcw" /out:"c:\mkm\bin/mbtag.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 nafxcwd.lib libcmtd.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\replaygain_analysis_static.lib ..\..\TestHarness\Debug\TestHarness.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\libFLAC_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\libFLAC++_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\getopt_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\grabbag_static.lib ..\..\3rdparty\flac-1.1.3\obj\debug\lib\utf8_static.lib ..\Debug\MBTag.lib Version.lib ..\..\3rdparty\ogg\libogg-1.0\win32\Debug\ogg_static_d.lib ..\..\3rdparty\ogg\libvorbis-1.0\win32\Debug\vorbis_static_d.lib ..\..\3rdparty\ogg\libvorbis-1.0\win32\Debug\vorbisfile_static_d.lib rpcrt4.lib ..\..\3rdparty\id3lib-3.8.3\libprj\Debug\id3libD.lib ..\..\3rdparty\id3lib-3.8.3\zlib\prj\Debug\zlibD.lib strmiids.lib ..\..\id3\id3libutils\Debug\id3utils.lib ..\..\util\Debug\util.lib dxguid.lib Quartz.lib ..\..\3rdparty\\WMFSDK9\lib\wmvcore.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /nodefaultlib:"msvcrt" /nodefaultlib:"libcmt" /nodefaultlib:"nafxcw" /out:"c:\mkm\bin/mbtag.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "MBTagger - Win32 Release"
# Name "MBTagger - Win32 Debug"
# Name "MBTagger - Win32 DebugBC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MBTagger.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
