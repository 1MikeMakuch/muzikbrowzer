# Microsoft Developer Studio Project File - Name="MBTag" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MBTag - Win32 DebugBC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MBTag.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MBTag.mak" CFG="MBTag - Win32 DebugBC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MBTag - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MBTag - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "MBTag - Win32 DebugBC" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MBTag - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\3rdparty\WMFSDK9\include" /I "..\3rdparty\ogg\vorbis-tools-1.0\include" /I "..\3rdparty\ogg\vorbis-tools-1.0\vorbiscomment" /I "../3rdparty/ogg/libogg-1.0/include" /I "../3rdparty/ogg/libvorbis-1.0/include" /I "../id3/id3libutils" /I "..\3rdparty/id3lib-3.8.3\include" /I "..\3rdparty\flac-1.1.3\include" /I "..\3rdparty\flac-1.1.3\src\metaflac" /I "..\WmaEdit" /I "..\util" /I "..\\" /I "../" /I "../util" /D ID3LIB_LINKOPTION=1 /D "FLAC__NO_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "MBTag - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\3rdparty\WMFSDK9\include" /I "..\3rdparty\ogg\vorbis-tools-1.0\include" /I "..\3rdparty\ogg\vorbis-tools-1.0\vorbiscomment" /I "../3rdparty/ogg/libogg-1.0/include" /I "../3rdparty/ogg/libvorbis-1.0/include" /I "../id3/id3libutils" /I "..\3rdparty/id3lib-3.8.3\include" /I "..\3rdparty\flac-1.1.3\include" /I "..\3rdparty\flac-1.1.3\src\metaflac" /I "..\WmaEdit" /I "..\util" /I "..\\" /I "../" /I "../util" /D ID3LIB_LINKOPTION=1 /D "FLAC__NO_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "MBTag - Win32 DebugBC"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MBTag___Win32_DebugBC"
# PROP BASE Intermediate_Dir "MBTag___Win32_DebugBC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugBC"
# PROP Intermediate_Dir "DebugBC"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\3rdparty\flac-1.1.3\include" /I "..\3rdparty\flac-1.1.3\src\metaflac" /I "..\WmaEdit" /I "..\util" /I "..\\" /I "." /I "../3rdparty/ogg/libogg-1.0/include" /I "../3rdparty/ogg/libvorbis-1.0/include" /I "../" /I "../util" /D "FLAC__NO_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\3rdparty\WMFSDK9\include" /I "..\3rdparty\ogg\vorbis-tools-1.0\include" /I "..\3rdparty\ogg\vorbis-tools-1.0\vorbiscomment" /I "../3rdparty/ogg/libogg-1.0/include" /I "../3rdparty/ogg/libvorbis-1.0/include" /I "../id3/id3libutils" /I "..\3rdparty/id3lib-3.8.3\include" /I "..\3rdparty\flac-1.1.3\include" /I "..\3rdparty\flac-1.1.3\src\metaflac" /I "..\WmaEdit" /I "..\util" /I "..\\" /I "../" /I "../util" /D ID3LIB_LINKOPTION=1 /D "FLAC__NO_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "MBTag - Win32 Release"
# Name "MBTag - Win32 Debug"
# Name "MBTag - Win32 DebugBC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MBTag.cpp
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\operations.c"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\operations_shorthand_cuesheet.c"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\operations_shorthand_picture.c"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\operations_shorthand_seektable.c"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\operations_shorthand_streaminfo.c"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\operations_shorthand_vorbiscomment.c"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\options.c"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\usage.c"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\utils.c"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\ogg\vorbis-tools-1.0\vorbiscomment\vcedit.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\types\Flac.h
# End Source File
# Begin Source File

SOURCE=.\MBTag.h
# End Source File
# Begin Source File

SOURCE=.\types\Mp3.h
# End Source File
# Begin Source File

SOURCE=.\types\Ogg.h
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\operations.h"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\options.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\usage.h"
# End Source File
# Begin Source File

SOURCE="..\3rdparty\flac-1.1.3\src\metaflac\utils.h"
# End Source File
# Begin Source File

SOURCE=.\types\Wma.h
# End Source File
# End Group
# End Target
# End Project
