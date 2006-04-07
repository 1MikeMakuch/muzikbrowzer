; -- Sample2.iss --
; Same as Sample1.iss, but creates its icon in the Programs folder of the
; Start Menu instead of in a subfolder, and also creates a desktop icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[InstallDelete]
Type: files; Name: "{commonprograms}\muzikbrowzer.lnk";
;Type: files; Name: "{commonprograms}\BitmapCutter.lnk";


[Setup]
AppMutex=MuzikbrowzerAppMutex,Global\MuzikbrowzerAppMutex
;AppMutex=MuzikbrowzerAppMutex,Global\MuzikbrowzerAppMutex,MuzikbrowzerBitMapCutterAppMutex,Global\MuzikbrowzerBitMapCutterAppMutex
AppName=muzikbrowzer
AppVerName=muzikbrowzer 2.0.0
AppCopyright=Copyright (C) 2001-2006 Pecan Ventures, LLC
DefaultDirName={pf}\muzikbrowzer
DefaultGroupName=muzikbrowzer
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\muzikbrowzer.exe
MinVersion=4.1.1998,4
OutputBaseFilename=muzikbrowzer_setup_2_0_0
LicenseFile=mb_license.txt

[Files]
Source: "..\Release\muzikbrowzer.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite
;Source: "..\res\muzikbrowzer.wav"; DestDir: "{app}"; CopyMode: alwaysoverwrite

Source: "..\Help\muzikbrowzer.chm"; DestDir: "{app}"; CopyMode: alwaysoverwrite

Source: "..\Skins\*.bmp"; DestDir: "{app}/skins"; CopyMode: alwaysoverwrite; Flags: recursesubdirs 
Source: "..\Skins\*.mbsd"; DestDir: "{app}/skins"; CopyMode: alwaysoverwrite; Flags: recursesubdirs 


;Source: "..\..\ButtonSkinner\Release\BitmapCutter.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite

;Source: "..\Help\KeyHH.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite

[Icons]
Name: "{commonprograms}\Muzikbrowzer\muzikbrowzer"; Filename: "{app}\muzikbrowzer.exe"; WorkingDir: "{app}"
;Name: "{commonprograms}\Muzikbrowzer\BitmapCutter"; Filename: "{app}\BitmapCutter.exe"; WorkingDir: "{app}"
Name: "{commonprograms}\Muzikbrowzer\muzikbrowzer Help"; Filename: "{app}\muzikbrowzer.chm"; WorkingDir: "{app}"
Name: "{userdesktop}\muzikbrowzer"; Filename: "{app}\muzikbrowzer.exe"; WorkingDir: "{app}"
;Name: "{userdesktop}\BitmapCutter"; Filename: "{app}\BitmapCutter.exe"; WorkingDir: "{app}"

[Registry]
Root: HKLM; Subkey: "Software\muzikbrowzer"; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: string; ValueName: "DbLocation"; ValueData: "{app}"  ; Flags: createvalueifdoesntexist uninsdeletekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: string; ValueName: "SkinName"; ValueData: "MBClassic"  ; Flags: createvalueifdoesntexist uninsdeletekey

; Keys to delete
Root: HKLM; Subkey: "Software\muzikbrowzer\PreviousValues"; ValueType: none; Flags: deletekey dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "ColorBkHigh"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "ColorBkNormal"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "ColorBkPanel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "ColorBkSel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "ColorTxHigh"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "ColorTxNormal"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "ColorTxPanel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "ColorTxSel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "NumMp3Extensions"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_00"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_01"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_02"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_03"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_04"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "Mp3Extension_05"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "FontPanel"; Flags: deletevalue dontcreatekey
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: none; ValueName: "FontTitles"; Flags: deletevalue dontcreatekey



