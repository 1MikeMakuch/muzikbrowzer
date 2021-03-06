; -- Sample2.iss --
; Same as Sample1.iss, but creates its icon in the Programs folder of the
; Start Menu instead of in a subfolder, and also creates a desktop icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[InstallDelete]
Type: files; Name: "{commonprograms}\Muzikbrowzer.lnk";
;Type: files; Name: "{commonprograms}\BitmapCutter.lnk";

[Setup]
AppMutex=MuzikbrowzerAppMutex,Global\MuzikbrowzerAppMutex
;AppMutex=MuzikbrowzerAppMutex,Global\MuzikbrowzerAppMutex,MuzikbrowzerBitMapCutterAppMutex,Global\MuzikbrowzerBitMapCutterAppMutex
AppName=Muzikbrowzer
AppVerName=Muzikbrowzer 2.2.1 (built 2017-07-10 23:33:24)
AppCopyright=Copyright (C) 2001-Present Pecan Ventures, LLC
DefaultDirName={pf}\Muzikbrowzer
DefaultGroupName=Muzikbrowzer
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\Muzikbrowzer.exe
MinVersion=4.1.1998,4
OutputBaseFilename=Muzikbrowzer_setup_2_2_1
LicenseFile=mb_license.txt
WizardImageFile=../res/MBWizardImageFile.bmp
WizardSmallImageFile=../res/MBWizardSmallImageFile.bmp

; *** "Welcome" wizard page
[Messages]
WelcomeLabel1=Welcome to the [name] Setup Wizard
WelcomeLabel2=This will install [name/ver] on your computer.%n%nMuzikbrowzer requires Windows Media Player 9 or later.%n%nIt is recommended that you close all other applications before continuing.%n%nIf you are installing Muzikbrowzer over an existing version it is not necessary to first uninstall your existing version, simply proceed with this installation. You must first close any running instances of Muzikbrowzer before proceeding.

[UninstallDelete]
Type: files; Name: "{app}\MuzikbrowzerCachedDrives"
Type: files; Name: "{commonappdata}\Muzikbrowzer\Muzikbrowzer.log"
Type: files; Name: "{commonappdata}\Muzikbrowzer\Muzikbrowzer.mb"
Type: files; Name: "{commonappdata}\Muzikbrowzer\Muzikbrowzer.mb.txt"
Type: files; Name: "{commonappdata}\Muzikbrowzer\Muzikbrowzer.mbdirs"
Type: files; Name: "{commonappdata}\Muzikbrowzer\Muzikbrowzer.mbexs"
Type: files; Name: "{commonappdata}\Muzikbrowzer\Muzikbrowzer.mbfls"
Type: files; Name: "{commonappdata}\Muzikbrowzer\Muzikbrowzer.mblastsearch"
Type: files; Name: "{commonappdata}\Muzikbrowzer\Muzikbrowzer.mbtgs"
Type: files; Name: "{commonappdata}\Muzikbrowzer\skins"
Type: files; Name: "{app}\Muzikbrowzer\Muzikbrowzer.log"
Type: files; Name: "{app}\Muzikbrowzer\Muzikbrowzer.mb"
Type: files; Name: "{app}\Muzikbrowzer\Muzikbrowzer.mb.txt"
Type: files; Name: "{app}\Muzikbrowzer\Muzikbrowzer.mbdirs"
Type: files; Name: "{app}\Muzikbrowzer\Muzikbrowzer.mbexs"
Type: files; Name: "{app}\Muzikbrowzer\Muzikbrowzer.mbfls"
Type: files; Name: "{app}\Muzikbrowzer\Muzikbrowzer.mblastsearch"
Type: files; Name: "{app}\Muzikbrowzer\Muzikbrowzer.mbtgs"
Type: files; Name: "{app}\Muzikbrowzer\skins"

[Files]
Source: "..\Release\Muzikbrowzer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\MBTag\tagger\Release\mbtag.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "..\res\Muzikbrowzer.wav"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Help\Muzikbrowzer.chm"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Skins\MBClassic\*.bmp"; DestDir: "{commonappdata}\Muzikbrowzer\skins\MBClassic"; Flags: recursesubdirs ignoreversion
Source: "..\Skins\MBClassic\*.mbsd"; DestDir: "{commonappdata}\Muzikbrowzer\skins\MBClassic"; Flags: recursesubdirs ignoreversion
Source: "..\Skins\WinterFresh\*.bmp"; DestDir: "{commonappdata}\Muzikbrowzer\skins\WinterFresh"; Flags: recursesubdirs ignoreversion
Source: "..\Skins\WinterFresh\*.mbsd"; DestDir: "{commonappdata}\Muzikbrowzer\skins\WinterFresh"; Flags: recursesubdirs ignoreversion
Source: "..\Skins\YellowJacket\*.bmp"; DestDir: "{commonappdata}\Muzikbrowzer\skins\YellowJacket"; Flags: recursesubdirs ignoreversion
Source: "..\Skins\YellowJacket\*.mbsd"; DestDir: "{commonappdata}\Muzikbrowzer\skins\YellowJacket"; Flags: recursesubdirs ignoreversion


;Source: "..\..\ButtonSkinner\Release\BitmapCutter.exe"; DestDir: "{app}"; Flags: ignoreversion

;Source: "..\Help\KeyHH.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{commonprograms}\Muzikbrowzer\Muzikbrowzer"; Filename: "{app}\Muzikbrowzer.exe"; WorkingDir: "{app}"
;Name: "{commonprograms}\Muzikbrowzer\BitmapCutter"; Filename: "{app}\BitmapCutter.exe"; WorkingDir: "{app}"
Name: "{commonprograms}\Muzikbrowzer\Muzikbrowzer Help"; Filename: "{app}\Muzikbrowzer.chm"; WorkingDir: "{app}"
Name: "{userdesktop}\Muzikbrowzer"; Filename: "{app}\Muzikbrowzer.exe"; WorkingDir: "{app}"
;Name: "{userdesktop}\BitmapCutter"; Filename: "{app}\BitmapCutter.exe"; WorkingDir: "{app}"

[Registry]
Root: HKLM; Subkey: "Software\Muzikbrowzer"; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: string; ValueName: "DbLocation"; ValueData: "{commonappdata}\Muzikbrowzer"  ; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: string; ValueName: "SkinName"; ValueData: "MBClassic"  ; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: dword; ValueName: "NumDirs"; ValueData: "1"  ; Flags: createvalueifdoesntexist uninsdeletevalue
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: string; ValueName: "Dir_00"; ValueData: "{commondocs}\My Music"; Flags: createvalueifdoesntexist uninsdeletevalue
;Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: string; ValueName: "ExpHtmlTmpl"; ValueData: "{app}\MbHtmlTemplate.txt" ; Flags: createvalueifdoesntexist uninsdeletevalue

; Keys to delete
Root: HKLM; Subkey: "Software\Muzikbrowzer\PreviousValues"; ValueType: none; Flags: deletekey dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "ColorBkHigh"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "ColorBkNormal"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "ColorBkPanel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "ColorBkSel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "ColorTxHigh"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "ColorTxNormal"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "ColorTxPanel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "ColorTxSel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "NumMp3Extensions"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_00"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_01"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_02"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_03"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_04"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_05"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "FontPanel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\Muzikbrowzer"; ValueType: none; ValueName: "FontTitles"; Flags: deletevalue dontcreatekey

