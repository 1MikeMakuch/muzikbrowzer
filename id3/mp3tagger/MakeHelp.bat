@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by ID3TAGGER.HPJ. >"hlp\id3tagger.hm"
echo. >>"hlp\id3tagger.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\id3tagger.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\id3tagger.hm"
echo. >>"hlp\id3tagger.hm"
echo // Prompts (IDP_*) >>"hlp\id3tagger.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\id3tagger.hm"
echo. >>"hlp\id3tagger.hm"
echo // Resources (IDR_*) >>"hlp\id3tagger.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\id3tagger.hm"
echo. >>"hlp\id3tagger.hm"
echo // Dialogs (IDD_*) >>"hlp\id3tagger.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\id3tagger.hm"
echo. >>"hlp\id3tagger.hm"
echo // Frame Controls (IDW_*) >>"hlp\id3tagger.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\id3tagger.hm"
REM -- Make help for Project ID3TAGGER


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\id3tagger.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\id3tagger.hlp" goto :Error
if not exist "hlp\id3tagger.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\id3tagger.hlp" Debug
if exist Debug\nul copy "hlp\id3tagger.cnt" Debug
if exist Release\nul copy "hlp\id3tagger.hlp" Release
if exist Release\nul copy "hlp\id3tagger.cnt" Release
echo.
goto :done

:Error
echo hlp\id3tagger.hpj(1) : error: Problem encountered creating help file

:done
echo.
