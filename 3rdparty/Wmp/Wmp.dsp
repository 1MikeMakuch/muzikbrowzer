# Microsoft Developer Studio Project File - Name="Wmp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Wmp - Win32 DebugBC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Wmp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Wmp.mak" CFG="Wmp - Win32 DebugBC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Wmp - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Wmp - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Wmp - Win32 DebugBC" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Wmp - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Wmp - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Wmp - Win32 DebugBC"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Wmp___Win32_DebugBC"
# PROP BASE Intermediate_Dir "Wmp___Win32_DebugBC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugBC"
# PROP Intermediate_Dir "DebugBC"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
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

# Name "Wmp - Win32 Release"
# Name "Wmp - Win32 Debug"
# Name "Wmp - Win32 DebugBC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpcdrom.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpcdromcollection.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpclosedcaption.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpcontrols.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpdvd.cpp
# End Source File
# Begin Source File

SOURCE=.\wmperror.cpp
# End Source File
# Begin Source File

SOURCE=.\wmperroritem.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpmedia.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpmediacollection.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpnetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpplayer4.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpplayerapplication.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpplaylist.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpplaylistarray.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpplaylistcollection.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpsettings.cpp
# End Source File
# Begin Source File

SOURCE=.\wmpstringcollection.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\wmpcdrom.h
# End Source File
# Begin Source File

SOURCE=.\wmpcdromcollection.h
# End Source File
# Begin Source File

SOURCE=.\wmpclosedcaption.h
# End Source File
# Begin Source File

SOURCE=.\wmpcontrols.h
# End Source File
# Begin Source File

SOURCE=.\wmpdvd.h
# End Source File
# Begin Source File

SOURCE=.\wmperror.h
# End Source File
# Begin Source File

SOURCE=.\wmperroritem.h
# End Source File
# Begin Source File

SOURCE=.\wmpmedia.h
# End Source File
# Begin Source File

SOURCE=.\wmpmediacollection.h
# End Source File
# Begin Source File

SOURCE=.\wmpnetwork.h
# End Source File
# Begin Source File

SOURCE=.\wmpplayer4.h
# End Source File
# Begin Source File

SOURCE=.\wmpplayerapplication.h
# End Source File
# Begin Source File

SOURCE=.\wmpplaylist.h
# End Source File
# Begin Source File

SOURCE=.\wmpplaylistarray.h
# End Source File
# Begin Source File

SOURCE=.\wmpplaylistcollection.h
# End Source File
# Begin Source File

SOURCE=.\wmpsettings.h
# End Source File
# Begin Source File

SOURCE=.\wmpstringcollection.h
# End Source File
# End Group
# End Target
# End Project
