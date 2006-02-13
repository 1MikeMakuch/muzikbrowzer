# Microsoft Developer Studio Project File - Name="muzikbrowzer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=muzikbrowzer - Win32 DebugBC
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "muzikbrowzer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "muzikbrowzer.mak" CFG="muzikbrowzer - Win32 DebugBC"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "muzikbrowzer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "muzikbrowzer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "muzikbrowzer - Win32 DebugBC" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "muzikbrowzer - Win32 Release"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "../" /I "../3rdparty/ogg" /I "../3rdparty/ogg/libvorbis-1.0/include" /I "../3rdparty/ogg/libogg-1.0/include" /I "c:\DXSDK\include" /I "..\3rdparty/id3lib-3.8.2\include" /I "config" /I "controls" /I "../id3/id3libutils" /I "irman" /I "../util" /I "Serial" /I "../WmaEdit" /I "../fractal" /I "../3rdparty/Wmp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D ID3LIB_LINKOPTION=1 /D "_WIN32_DCOM" /Fr /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../WmaEdit/Release/WmaTagg.lib Version.lib ../3rdparty/ogg/libogg-1.0/win32/Release/ogg_static.lib ../oggtagger/Release/oggtagger.lib ../3rdparty/ogg/libvorbis-1.0/win32/Release/vorbis_static.lib ../3rdparty/ogg/libvorbis-1.0/win32/Release/vorbisfile_static.lib rpcrt4.lib ..\3rdparty/id3lib-3.8.2\libprj\Release\id3lib.lib ..\3rdparty/id3lib-3.8.2\zlib\prj\Release\zlib.lib ..\TestHarness\Release\TestHarness.lib Help\HtmlHelp.lib strmiids.lib config\Release\config.lib controls\Release\controls.lib ..\id3\id3libutils\Release\id3utils.lib irman\Release\irman.lib Serial\Release\Serial.lib ..\util\Release\util.lib dxguid.lib Quartz.lib wmvcore.lib ../3rdparty/Wmp/Release/Wmp.lib /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrtd" /nodefaultlib:"msvcrt" /nodefaultlib:"nafxcwd" /nodefaultlib:"libcmtd" /libpath:"..\3rdparty\WMFSDK9\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "muzikbrowzer - Win32 Debug"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "../" /I "../3rdparty/ogg" /I "../3rdparty/ogg/libvorbis-1.0/include" /I "../3rdparty/ogg/libogg-1.0/include" /I "c:\DXSDK\include" /I "..\3rdparty/id3lib-3.8.2\include" /I "config" /I "controls" /I "../id3/id3libutils" /I "irman" /I "../util" /I "Serial" /I "../WmaEdit" /I "../fractal" /I "../3rdparty/Wmp" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../3rdparty/Wmp/Debug/Wmp.lib Version.lib config\Debug\config.lib ../3rdparty/ogg/libogg-1.0/win32/Debug/ogg_static_d.lib ../oggtagger/Debug/oggtagger.lib ../WmaEdit/Debug/WmaTagg.lib ../3rdparty/ogg/libvorbis-1.0/win32/Debug/vorbis_static_d.lib ../3rdparty/ogg/libvorbis-1.0/win32/Debug/vorbisfile_static_d.lib rpcrt4.lib ..\3rdparty/id3lib-3.8.2\libprj\Debug\id3libD.lib ..\3rdparty/id3lib-3.8.2\zlib\prj\Debug\zlibD.lib ..\TestHarness\Debug\TestHarness.lib Help\HtmlHelp.lib strmiids.lib config\Debug\config.lib controls\Debug\controls.lib ..\id3\id3libutils\Debug\id3utils.lib irman\Debug\irman.lib Serial\Debug\Serial.lib ..\util\Debug\util.lib dxguid.lib Quartz.lib ..\md5\Debug\md5.lib wmvcore.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib:"msvcrt" /nodefaultlib:"libcmt" /nodefaultlib:"nafxcw" /out:"Debug/muzikbrowzerD.exe" /pdbtype:sept /libpath:"..\3rdparty\WMFSDK9\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "muzikbrowzer - Win32 DebugBC"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "muzikbrowzer___Win32_DebugBC"
# PROP BASE Intermediate_Dir "muzikbrowzer___Win32_DebugBC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugBC"
# PROP Intermediate_Dir "DebugBC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "../" /I "../3rdparty/ogg" /I "../3rdparty/ogg/libvorbis-1.0/include" /I "../3rdparty/ogg/libogg-1.0/include" /I "c:\DXSDK\include" /I "..\3rdparty/id3lib-3.8.2\include" /I "config" /I "controls" /I "../id3/id3libutils" /I "irman" /I "../util" /I "Serial" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "../" /I "../3rdparty/ogg" /I "../3rdparty/ogg/libvorbis-1.0/include" /I "../3rdparty/ogg/libogg-1.0/include" /I "c:\DXSDK\include" /I "..\3rdparty/id3lib-3.8.2\include" /I "config" /I "controls" /I "../id3/id3libutils" /I "irman" /I "../util" /I "Serial" /I "../WmaEdit" /I "../fractal" /I "../3rdparty/Wmp" /D ID3LIB_LINKOPTION=1 /D "TEST_HARNESS" /D "WIN32" /D "_DEBUG" /D "_DEBUGBC" /D "_WINDOWS" /D "_WIN32_DCOM" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Version.lib config\Debug\config.lib ../3rdparty/ogg/libogg-1.0/win32/Debug/ogg_static_d.lib ../oggtagger/Debug/oggtagger.lib ../3rdparty/ogg/libvorbis-1.0/win32/Debug/vorbis_static_d.lib ../3rdparty/ogg/libvorbis-1.0/win32/Debug/vorbisfile_static_d.lib rpcrt4.lib ..\3rdparty/id3lib-3.8.2\libprj\Debug\id3libD.lib ..\3rdparty/id3lib-3.8.2\zlib\prj\Debug\zlibD.lib ..\TestHarness\Debug\TestHarness.lib Help\HtmlHelp.lib strmiids.lib config\Debug\config.lib controls\Debug\controls.lib ..\id3\id3libutils\Debug\id3utils.lib irman\Debug\irman.lib Serial\Debug\Serial.lib ..\util\Debug\util.lib dxguid.lib Quartz.lib ..\md5\Debug\md5.lib /nologo /subsystem:windows /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"msvcrt" /nodefaultlib:"libcmt" /nodefaultlib:"nafxcw" /out:"Debug/muzikbrowzerD.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Version.lib config\DebugBC\config.lib ../3rdparty/ogg/libogg-1.0/win32/DebugBC/ogg_static_d.lib ../oggtagger/DebugBC/oggtagger.lib ../3rdparty/ogg/libvorbis-1.0/win32/DebugBC/vorbis_static_d.lib ../3rdparty/ogg/libvorbis-1.0/win32/DebugBC/vorbisfile_static_d.lib rpcrt4.lib ..\3rdparty/id3lib-3.8.2\libprj\DebugBC\id3libD.lib ..\3rdparty/id3lib-3.8.2\zlib\prj\DebugBC\zlibD.lib ..\TestHarness\DebugBC\TestHarness.lib Help\HtmlHelp.lib strmiids.lib config\Debug\config.lib controls\DebugBC\controls.lib ..\id3\id3libutils\DebugBC\id3utils.lib irman\DebugBC\irman.lib Serial\DebugBC\Serial.lib ..\util\DebugBC\util.lib dxguid.lib Quartz.lib ..\md5\DebugBC\md5.lib wmvcore.lib ..\3rdparty\Wmp\DebugBC\Wmp.lib /nologo /subsystem:windows /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"msvcrt" /nodefaultlib:"libcmt" /nodefaultlib:"nafxcw" /out:"DebugBC/muzikbrowzerD.exe" /pdbtype:sept /libpath:"..\3rdparty\WMFSDK9\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "muzikbrowzer - Win32 Release"
# Name "muzikbrowzer - Win32 Debug"
# Name "muzikbrowzer - Win32 DebugBC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\About.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialogSK.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectShow.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectShowGlobals.cpp
# End Source File
# Begin Source File

SOURCE=.\Fractal.cpp
# End Source File
# Begin Source File

SOURCE=.\getdxver.cpp
# End Source File
# Begin Source File

SOURCE=.\ID3Display.cpp
# End Source File
# Begin Source File

SOURCE=.\InitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadPlaylistDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MBMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyIDThree.cpp
# End Source File
# Begin Source File

SOURCE=.\MPtr.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicDb.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicPlayerDS.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicPlayerWMP.cpp
# End Source File
# Begin Source File

SOURCE=.\Help\muzikbrowzer.hhp
# End Source File
# Begin Source File

SOURCE=.\PicCache.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerApp.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RMenuDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SavePlaylistDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\About.h
# End Source File
# Begin Source File

SOURCE=.\CDialogSK.h
# End Source File
# Begin Source File

SOURCE=.\DirectShow.h
# End Source File
# Begin Source File

SOURCE=.\DirectShowMediaTypes.h
# End Source File
# Begin Source File

SOURCE=.\Fractal.h
# End Source File
# Begin Source File

SOURCE=.\ID3Display.h
# End Source File
# Begin Source File

SOURCE=.\InitDlg.h
# End Source File
# Begin Source File

SOURCE=.\IRCodes.h
# End Source File
# Begin Source File

SOURCE=.\LoadPlaylistDlg.h
# End Source File
# Begin Source File

SOURCE=.\MBGlobals.h
# End Source File
# Begin Source File

SOURCE=.\MBMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\ModifyIDThree.h
# End Source File
# Begin Source File

SOURCE=.\MPtr.h
# End Source File
# Begin Source File

SOURCE=.\MusicDb.h
# End Source File
# Begin Source File

SOURCE=.\MusicPlayer.h
# End Source File
# Begin Source File

SOURCE=.\MusicPlayerDS.h
# End Source File
# Begin Source File

SOURCE=.\MusicPlayerWMP.h
# End Source File
# Begin Source File

SOURCE=.\muzikbrowzerVersion.h
# End Source File
# Begin Source File

SOURCE=.\PicCache.h
# End Source File
# Begin Source File

SOURCE=.\PlayerApp.h
# End Source File
# Begin Source File

SOURCE=.\PlayerCallbacks.h
# End Source File
# Begin Source File

SOURCE=.\PlayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RMenuDlg.h
# End Source File
# Begin Source File

SOURCE=.\SavePlaylistDlg.h
# End Source File
# Begin Source File

SOURCE=.\SkinDefs.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToDo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\app.gif
# End Source File
# Begin Source File

SOURCE=.\res\app_icon1_iconwkshp.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_labe.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_labelD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_labelF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_labelU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\app_labelX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_exit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_exitD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_exitF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_exitU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_exitX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_maximize.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_maximizeD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_maximizeF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_maximizeU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_maximizeX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_minimize.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_minimizeD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_minimizeF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_minimizeU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_minimizeX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_restore.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_restoreD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_restoreF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_restoreU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_restoreX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonPauseD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonPauseF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonPauseU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonPauseX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonPlayD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonPlayF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonPlayU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonPlayX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonStopD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonStopF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonStopU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonStopX.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor4.cur
# End Source File
# Begin Source File

SOURCE=.\res\group_te.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\IconV2_01.ico
# End Source File
# Begin Source File

SOURCE=.\res\image1.bin
# End Source File
# Begin Source File

SOURCE=.\res\MBIcon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MBIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\MBIcon1616.ico
# End Source File
# Begin Source File

SOURCE=.\res\mblogo.ico
# End Source File
# Begin Source File

SOURCE=.\res\mblogo1616.ico
# End Source File
# Begin Source File

SOURCE=.\res\mblogo3.ico
# End Source File
# Begin Source File

SOURCE=.\res\mblogo3232.ico
# End Source File
# Begin Source File

SOURCE=.\res\mtitle3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle3D.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle3F.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle3U.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle3X.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle4D.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle4F.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle4U.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mtitle4X.bmp
# End Source File
# Begin Source File

SOURCE=.\res\muzikbrowzer.bmp
# End Source File
# Begin Source File

SOURCE=.\muzikbrowzer.rc
# End Source File
# Begin Source File

SOURCE=.\res\OBM_DNARROW.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OBM_SCROLLBG.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OBM_SCROLLBUTTON.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OBM_UPARROW.bmp
# End Source File
# Begin Source File

SOURCE=.\res\printer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\screenshot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ScrollButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ScrollDownArrowButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ScrollUpArrowButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\slidevert.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\test01.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tick_vert.bmp
# End Source File
# Begin Source File

SOURCE=.\res\web.ico
# End Source File
# Begin Source File

SOURCE=.\res\xaudio.ico
# End Source File
# End Group
# End Target
# End Project
# Section muzikbrowzer : {6BF52A52-394A-11D3-B153-00C04F79FAA6}
# 	2:21:DefaultSinkHeaderFile:wmpplayer4.h
# 	2:16:DefaultSinkClass:CWMPPlayer4
# End Section
# Section muzikbrowzer : {40897764-CEAB-47BE-AD4A-8E28537F9BBF}
# 	2:5:Class:CWMPPlayerApplication
# 	2:10:HeaderFile:wmpplayerapplication.h
# 	2:8:ImplFile:wmpplayerapplication.cpp
# End Section
# Section muzikbrowzer : {9104D1AB-80C9-4FED-ABF0-2E6417A6DF14}
# 	2:5:Class:CWMPSettings
# 	2:10:HeaderFile:wmpsettings.h
# 	2:8:ImplFile:wmpsettings.cpp
# End Section
# Section muzikbrowzer : {A12DCF7D-14AB-4C1B-A8CD-63909F06025B}
# 	2:5:Class:CWMPError
# 	2:10:HeaderFile:wmperror.h
# 	2:8:ImplFile:wmperror.cpp
# End Section
# Section muzikbrowzer : {D5F0F4F1-130C-11D3-B14E-00C04F79FAA6}
# 	2:5:Class:CWMPPlaylist
# 	2:10:HeaderFile:wmpplaylist.h
# 	2:8:ImplFile:wmpplaylist.cpp
# End Section
# Section muzikbrowzer : {4F2DF574-C588-11D3-9ED0-00C04FB6E937}
# 	2:5:Class:CWMPClosedCaption
# 	2:10:HeaderFile:wmpclosedcaption.h
# 	2:8:ImplFile:wmpclosedcaption.cpp
# End Section
# Section muzikbrowzer : {EE4C8FE2-34B2-11D3-A3BF-006097C9B344}
# 	2:5:Class:CWMPCdromCollection
# 	2:10:HeaderFile:wmpcdromcollection.h
# 	2:8:ImplFile:wmpcdromcollection.cpp
# End Section
# Section muzikbrowzer : {94D55E95-3FAC-11D3-B155-00C04F79FAA6}
# 	2:5:Class:CWMPMedia
# 	2:10:HeaderFile:wmpmedia.h
# 	2:8:ImplFile:wmpmedia.cpp
# End Section
# Section muzikbrowzer : {3614C646-3B3B-4DE7-A81E-930E3F2127B3}
# 	2:5:Class:CWMPErrorItem
# 	2:10:HeaderFile:wmperroritem.h
# 	2:8:ImplFile:wmperroritem.cpp
# End Section
# Section muzikbrowzer : {EC21B779-EDEF-462D-BBA4-AD9DDE2B29A7}
# 	2:5:Class:CWMPNetwork
# 	2:10:HeaderFile:wmpnetwork.h
# 	2:8:ImplFile:wmpnetwork.cpp
# End Section
# Section muzikbrowzer : {10A13217-23A7-439B-B1C0-D847C79B7774}
# 	2:5:Class:CWMPPlaylistCollection
# 	2:10:HeaderFile:wmpplaylistcollection.h
# 	2:8:ImplFile:wmpplaylistcollection.cpp
# End Section
# Section muzikbrowzer : {8363BC22-B4B4-4B19-989D-1CD765749DD1}
# 	2:5:Class:CWMPMediaCollection
# 	2:10:HeaderFile:wmpmediacollection.h
# 	2:8:ImplFile:wmpmediacollection.cpp
# End Section
# Section muzikbrowzer : {4A976298-8C0D-11D3-B389-00C04F68574B}
# 	2:5:Class:CWMPStringCollection
# 	2:10:HeaderFile:wmpstringcollection.h
# 	2:8:ImplFile:wmpstringcollection.cpp
# End Section
# Section muzikbrowzer : {679409C0-99F7-11D3-9FB7-00105AA620BB}
# 	2:5:Class:CWMPPlaylistArray
# 	2:10:HeaderFile:wmpplaylistarray.h
# 	2:8:ImplFile:wmpplaylistarray.cpp
# End Section
# Section muzikbrowzer : {CFAB6E98-8730-11D3-B388-00C04F68574B}
# 	2:5:Class:CWMPCdrom
# 	2:10:HeaderFile:wmpcdrom.h
# 	2:8:ImplFile:wmpcdrom.cpp
# End Section
# Section muzikbrowzer : {74C09E02-F828-11D2-A74B-00A0C905F36E}
# 	2:5:Class:CWMPControls
# 	2:10:HeaderFile:wmpcontrols.h
# 	2:8:ImplFile:wmpcontrols.cpp
# End Section
# Section muzikbrowzer : {6C497D62-8919-413C-82DB-E935FB3EC584}
# 	2:5:Class:CWMPPlayer4
# 	2:10:HeaderFile:wmpplayer4.h
# 	2:8:ImplFile:wmpplayer4.cpp
# End Section
# Section muzikbrowzer : {8DA61686-4668-4A5C-AE5D-803193293DBE}
# 	2:5:Class:CWMPDVD
# 	2:10:HeaderFile:wmpdvd.h
# 	2:8:ImplFile:wmpdvd.cpp
# End Section
