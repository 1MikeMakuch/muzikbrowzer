# Microsoft Developer Studio Project File - Name="mydiff" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=mydiff - Win32 DebugBC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mydiff.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mydiff.mak" CFG="mydiff - Win32 DebugBC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mydiff - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "mydiff - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "mydiff - Win32 DebugBC" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mydiff - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "../util" /I "../TestHarness" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
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
# ADD LINK32 ../util/Release/util.lib ../TestHarness/Release/TestHarness.lib /nologo /subsystem:windows /machine:I386 /out:"c:/mkm/bin/mydiff.exe"

!ELSEIF  "$(CFG)" == "mydiff - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "../util" /I "../TestHarness" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
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
# ADD LINK32 ../util/Debug/util.lib ../TestHarness/Debug/TestHarness.lib /nologo /subsystem:windows /debug /machine:I386 /out:"c:/mkm/bin/mydiff.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "mydiff - Win32 DebugBC"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mydiff___Win32_DebugBC"
# PROP BASE Intermediate_Dir "mydiff___Win32_DebugBC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugBC"
# PROP Intermediate_Dir "DebugBC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "../util" /I "../TestHarness" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "../util" /I "../TestHarness" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../util/Debug/util.lib ../TestHarness/Debug/TestHarness.lib /nologo /subsystem:windows /debug /machine:I386 /out:"c:/mkm/bin/mydiff.exe" /pdbtype:sept
# ADD LINK32 ../util/Debug/util.lib ../TestHarness/Debug/TestHarness.lib /nologo /subsystem:windows /debug /machine:I386 /out:"c:/mkm/bin/mydiffBC.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mydiff - Win32 Release"
# Name "mydiff - Win32 Debug"
# Name "mydiff - Win32 DebugBC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cverror.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtendedListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\mydiff.cpp
# End Source File
# Begin Source File

SOURCE=.\mydiff.rc
# End Source File
# Begin Source File

SOURCE=.\mydiffDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SourceLine.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cverror.h
# End Source File
# Begin Source File

SOURCE=.\ExtendedListBox.h
# End Source File
# Begin Source File

SOURCE=.\mydiff.h
# End Source File
# Begin Source File

SOURCE=.\mydiffDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SourceLine.h
# End Source File
# Begin Source File

SOURCE=.\StatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\mydiff.ico
# End Source File
# Begin Source File

SOURCE=.\res\mydiff.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\cstat
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
