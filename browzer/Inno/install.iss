; -- Sample2.iss --
; Same as Sample1.iss, but creates its icon in the Programs folder of the
; Start Menu instead of in a subfolder, and also creates a desktop icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=muzikbrowzer
AppVerName=muzikbrowzer 1.0b
AppCopyright=Copyright (C) 2001 Pecan Ventures, LLC
DefaultDirName={pf}\muzikbrowzer
;DisableProgramGroupPage=yes
; ^ since no icons will be created in "{group}", we don't need the wizard
;   to ask for a group name.
UninstallDisplayIcon={app}\muzikbrowzer.exe
MinVersion=4.1.1998,4
OutputBaseFilename=muzikbrowzer_setup10b

[Files]
Source: "..\Debug\muzikbrowzer.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "..\Debug\muzikbrowzer.pdb"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "..\Debug\vc60.pdb"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "..\Help\muzikbrowzer.chm"; DestDir: "{app}"
;Source: "Readme.txt"; DestDir: "{app}"; Flags: isreadme
;Source: "..\ReqDlls\MFC42D.DLL"; DestDir: "{app}"
;Source: "..\ReqDlls\MSVCP60D.DLL"; DestDir: "{app}"
;Source: "..\ReqDlls\MSVCRTD.DLL"; DestDir: "{app}"
;Source: "..\ReqDlls\id3lib.dll"; DestDir: "{app}"
;Source: "..\ReqDlls\msvcp60.dll"; DestDir: "{app}"
;Source: "..\ReqDlls\msvcrt.dll"; DestDir: "{app}"
;Source: "..\ReqDlls\xaudio.dll"; DestDir: "{app}"

[Icons]
Name: "{commonprograms}\muzikbrowzer"; Filename: "{app}\muzikbrowzer.exe"
Name: "{userdesktop}\muzikbrowzer"; Filename: "{app}\muzikbrowzer.exe"

[Registry]
Root: HKLM; Subkey: "Software\muzikbrowzer"; Flags: uninsdeletekey
