; -- Sample2.iss --
; Same as Sample1.iss, but creates its icon in the Programs folder of the
; Start Menu instead of in a subfolder, and also creates a desktop icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=mp3tagger
AppVerName=mp3tagger 1.0.1
AppCopyright=Copyright (C) 2001 Pecan Ventures, LLC
DefaultDirName={pf}\mp3tagger
DefaultGroupName=mp3tagger
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\mp3tagger.exe
MinVersion=4.1.1998,4
OutputBaseFilename=mp3tagger_setup_1_0_1

[Files]
Source: "..\Release\mp3tagger.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite

[Icons]
Name: "{commonprograms}\mp3tagger"; Filename: "{app}\mp3tagger.exe"; WorkingDir: "{app}"
Name: "{userdesktop}\mp3tagger"; Filename: "{app}\mp3tagger.exe"; WorkingDir: "{app}"

