; -- Sample2.iss --
; Same as Sample1.iss, but creates its icon in the Programs folder of the
; Start Menu instead of in a subfolder, and also creates a desktop icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=muzikbrowzer
AppVerName=Muzikbrowzer 2.0.3
AppCopyright=Copyright (C) 2001-Present Pecan Ventures, LLC
DefaultDirName={pf}\muzikbrowzer
DefaultGroupName=muzikbrowzer
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\muzikbrowzer.exe
MinVersion=4.1.1998,4
OutputBaseFilename=Muzikbrowzer_setup_2_0_3_D
LicenseFile=mb_license.txt

[Files]
Source: "..\Release\muzikbrowzer.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "..\Help\muzikbrowzer.chm"; DestDir: "{app}"; CopyMode: alwaysoverwrite
;Source: "..\Help\KeyHH.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite

[Icons]
Name: "{commonprograms}\muzikbrowzer"; Filename: "{app}\muzikbrowzer.exe"; WorkingDir: "{app}"
Name: "{userdesktop}\muzikbrowzer"; Filename: "{app}\muzikbrowzer.exe"; WorkingDir: "{app}"

[Registry]
Root: HKLM; Subkey: "Software\muzikbrowzer";
Root: HKLM; Subkey: "Software\muzikbrowzer"; ValueType: string; ValueName: "DbLocation"; ValueData: "{app}"

