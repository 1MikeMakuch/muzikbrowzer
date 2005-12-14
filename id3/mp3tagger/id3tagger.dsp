# Microsoft Developer Studio Project File - Name="id3tagger" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=id3tagger - Win32 DebugBC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "id3tagger.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "id3tagger.mak" CFG="id3tagger - Win32 DebugBC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "id3tagger - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "id3tagger - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "id3tagger - Win32 DebugBC" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "id3tagger - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\..\3rdparty\id3lib-3.8.2\include" /I "..\id3libutils" /I "..\..\util" /I "..\..\browzer\controls" /I "..\..\browzer" /D ID3LIB_LINKOPTION=1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\3rdparty\id3lib-3.8.2\libprj\Release\id3lib.lib ..\..\3rdparty\id3lib-3.8.2\zlib\prj\Release\zlib.lib ..\..\util\Release\util.lib ..\..\TestHarness\Release\TestHarness.lib ..\id3libutils\Release\id3utils.lib ..\..\browzer\controls\Release\controls.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"nafxcwd" /nodefaultlib:"libcmtd" /out:"Release/mp3tagger.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "id3tagger - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /w /W0 /Gi /GX /Zi /Od /I "..\\" /I "..\..\3rdparty\id3lib-3.8.2\include" /I "..\id3libutils" /I "..\..\util" /I "..\..\browzer\controls" /I "..\..\browzer" /D ID3LIB_LINKOPTION=1 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\3rdparty\id3lib-3.8.2\libprj\Debug\id3libD.lib ..\..\3rdparty\id3lib-3.8.2\zlib\prj\Debug\zlibD.lib ..\..\util\Debug\util.lib ..\..\TestHarness\Debug\TestHarness.lib ..\id3libutils\Debug\id3utils.lib ..\..\browzer\controls\Debug\controls.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"Debug/mp3tagger.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "id3tagger - Win32 DebugBC"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "id3tagger___Win32_DebugBC"
# PROP BASE Intermediate_Dir "id3tagger___Win32_DebugBC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugBC"
# PROP Intermediate_Dir "DebugBC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /I "..\\" /I "..\..\3rdparty\id3lib-3.8.2\include" /I "..\id3libutils" /I "..\..\util" /I "..\..\browzer\controls" /I "..\..\browzer" /D ID3LIB_LINKOPTION=1 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GX /Zi /Od /I "..\\" /I "..\..\3rdparty\id3lib-3.8.2\include" /I "..\id3libutils" /I "..\..\util" /I "..\..\browzer\controls" /I "..\..\browzer" /D ID3LIB_LINKOPTION=1 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\3rdparty\id3lib-3.8.2\libprj\Debug\id3libD.lib ..\..\3rdparty\id3lib-3.8.2\zlib\prj\Debug\zlibD.lib ..\..\util\Debug\util.lib ..\..\TestHarness\Debug\TestHarness.lib ..\id3libutils\Debug\id3utils.lib ..\..\browzer\controls\Debug\controls.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/mp3tagger.exe" /pdbtype:sept
# ADD LINK32 ..\..\3rdparty\id3lib-3.8.2\libprj\Debug\id3libD.lib ..\..\3rdparty\id3lib-3.8.2\zlib\prj\Debug\zlibD.lib ..\..\util\Debug\util.lib ..\..\TestHarness\Debug\TestHarness.lib ..\id3libutils\Debug\id3utils.lib ..\..\browzer\controls\Debug\controls.lib /nologo /subsystem:windows /debug /machine:I386 /out:"DebugBC/mp3tagger.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "id3tagger - Win32 Release"
# Name "id3tagger - Win32 Debug"
# Name "id3tagger - Win32 DebugBC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CheckResult.cpp
# End Source File
# Begin Source File

SOURCE=.\id3tagger.cpp
# End Source File
# Begin Source File

SOURCE=.\id3tagger.odl
# End Source File
# Begin Source File

SOURCE=.\id3tagger.rc
# End Source File
# Begin Source File

SOURCE=.\id3taggerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Progress.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CheckResult.h
# End Source File
# Begin Source File

SOURCE=.\help.h
# End Source File
# Begin Source File

SOURCE=.\id3tagger.h
# End Source File
# Begin Source File

SOURCE=.\id3taggerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Progress.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\id3tagger.ico
# End Source File
# Begin Source File

SOURCE=.\res\id3tagger.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
