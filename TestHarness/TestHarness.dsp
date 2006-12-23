# Microsoft Developer Studio Project File - Name="TestHarness" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TestHarness - Win32 DebugBC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TestHarness.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TestHarness.mak" CFG="TestHarness - Win32 DebugBC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TestHarness - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TestHarness - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "TestHarness - Win32 DebugBC" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TestHarness - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\\" /I "..\controls" /I "..\util" /I "..\irman" /I "..\..\id3lib-3.8.3\include" /I "..\id3utils" /I "..\Serial" /I "..\config" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D ID3LIB_LINKOPTION=1 /D "_WIN32_DCOM" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "TestHarness - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "..\\" /I "..\controls" /I "..\util" /I "..\irman" /I "..\..\id3lib-3.8.3\include" /I "..\id3utils" /I "..\Serial" /I "..\config" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "TestHarness - Win32 DebugBC"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestHarness___Win32_DebugBC"
# PROP BASE Intermediate_Dir "TestHarness___Win32_DebugBC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugBC"
# PROP Intermediate_Dir "DebugBC"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\\" /I "..\controls" /I "..\util" /I "..\irman" /I "..\..\id3lib-3.8.3\include" /I "..\id3utils" /I "..\Serial" /I "..\config" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "..\\" /I "..\controls" /I "..\util" /I "..\irman" /I "..\..\id3lib-3.8.3\include" /I "..\id3utils" /I "..\Serial" /I "..\config" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
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

# Name "TestHarness - Win32 Release"
# Name "TestHarness - Win32 Debug"
# Name "TestHarness - Win32 DebugBC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Test.cpp
# End Source File
# Begin Source File

SOURCE=.\TestRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\TestResult.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Failure.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Test.h
# End Source File
# Begin Source File

SOURCE=.\TestHarness.h
# End Source File
# Begin Source File

SOURCE=.\TestRegistry.h
# End Source File
# Begin Source File

SOURCE=.\TestResult.h
# End Source File
# End Group
# End Target
# End Project
