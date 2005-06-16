# Microsoft Developer Studio Project File - Name="config" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=config - Win32 DebugBC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "config.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "config.mak" CFG="config - Win32 DebugBC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "config - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "config - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "config - Win32 DebugBC" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "config - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\\" /I "..\..\util" /I "..\\" /I "..\controls" /I "..\irman" /I "..\..\3rdparty\id3lib-3.8.2\include" /I "..\id3utils" /I "..\Serial" /I "..\..\TestHarness" /I "..\..\md5" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D ID3LIB_LINKOPTION=1 /D "_WIN32_DCOM" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "config - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\..\\" /I "..\..\util" /I "..\\" /I "..\controls" /I "..\irman" /I "..\..\3rdparty\id3lib-3.8.2\include" /I "..\id3utils" /I "..\Serial" /I "..\..\TestHarness" /I "..\..\md5" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "config - Win32 DebugBC"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "config___Win32_DebugBC"
# PROP BASE Intermediate_Dir "config___Win32_DebugBC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugBC"
# PROP Intermediate_Dir "DebugBC"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\..\\" /I "..\..\util" /I "..\\" /I "..\controls" /I "..\irman" /I "..\..\3rdparty\id3lib-3.8.2\include" /I "..\id3utils" /I "..\Serial" /I "..\..\TestHarness" /I "..\..\md5" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /GZ /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\..\\" /I "..\..\util" /I "..\\" /I "..\controls" /I "..\irman" /I "..\..\3rdparty\id3lib-3.8.2\include" /I "..\id3utils" /I "..\Serial" /I "..\..\TestHarness" /I "..\..\md5" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
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

# Name "config - Win32 Release"
# Name "config - Win32 Debug"
# Name "config - Win32 DebugBC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ConfigDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigIrman.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\MBConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ConfigDisplay.h
# End Source File
# Begin Source File

SOURCE=.\ConfigFiles.h
# End Source File
# Begin Source File

SOURCE=.\ConfigIrman.h
# End Source File
# Begin Source File

SOURCE=.\ConfigPassword.h
# End Source File
# Begin Source File

SOURCE=.\MBConfig.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
