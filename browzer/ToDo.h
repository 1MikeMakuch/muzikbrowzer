/*
-F Better looking Help for Remote Control
-B irman learning: wizard to determine interKeyDelay and/or # of pulses
-B irman: learning: have 'em hold down each button for a few seconds and get the code most
-B Remote Menu needs to pop up in center of dialog not center of screen.
-B Remote Menu should look like the main dialog
xB Store db version in the db file
-B put hostname & ip address in registry, password stuff
xB Assurance onthe web site about keeping their info private.
-B strip leading/trailing spaces on handling passwords and all data fields
-B make sure _DEBUG is defined everywhere and that macro
-Todo Insure info emailed to them. Purify bombs out.
-Todo upgrade redhat, gnats, customer database, regnow setup.
-B deal with hostid in registry - needs to stay there on uninstall.
-B put mb version# in password request.
-B deal with spaces in ID3 fields???
xF Beta agreement
-F license agreement
xF Trial Mode: only play 3 songs then stop
-B saved playlists become invalid if id3 editing is done.
-B need to only wakeup screensaver if it's running
xB check/test uninstall. It left the db & log files. It didn't remove the registry keys which, at this point, is what I want.
-B get gnats going
-B Why is DirectShow slower?
-B Figure out about DirectX, if it's installed and what I have to do to install it if it ain't.
-F delete key on genre, artist, album, song - remove from library
-F allow adjustment of sizes of genre, artist, album, song, playlist
-F allow startup with remote control - i.e. systray?
-F need a way to save msdev edited files, like /vibak
xB configirman lost comport in dialog
-Demo: for laptop - consider using all songs but only first 10 seconds
-F have a button to zip up everything and tell 'em to email it to me.
xB put registry settings on log file
xB Save volume level
xB figure out / fix volume level <==> 0 -> 10,000 dbs

-? Code review
-F goldwave.com - to encode mp3 they have you download lame or blade "due to license issues"
XB Consider appending to log file and trimming off the top at startup or shutdown. No just do single session logging.
xB Default dblocation to where we're running from.
xB Don't allow more than X songs to be queued up in one click. No, disallowing all but allowing all else.
xB Enhance MBMessageBox{Impl} for remote help. Not handled in Virtual Dialog correctly
-F Looks like I might need to make dialogs & controls inherit from VirtualDialog/VirtualControl
-F Note   You can capture from the CD-Audio input, but this audio stream has already gone through the digital-to-analog converter, so there will be a loss of sound quality from the original CD. For direct digital capture from the CD, see the Windows Media SDK.
xB Use of MBUNABLETOOPEN using it in too many places
xB memDb needs to validate the db on reading it in. Also need to do pointer checking. Check timestamp of db, of new then validate it.
xB need api for status messages, temp, permanent, multiple temps/permanents.
-F playing label - scroll it horizontally if it doesn't fit
xB loaded playlist becomes invalid if id3 editing is done. Need to modify playlists too. Atleast warn 'em about it. X Added to UserGuide.htm
-F Add "Windows Default" to Colors dialog???
-F Allow config set to windows default
-F Consider adding automated test harness to all code. Whew.
xF Eliminate reinitialize db upon modify id3 tags
-F Good looking "muzikbrowzer" logo on dialog
-F Need ability to add music without redoing the whole scan
-F Need icon on About dialog.
-F Remember position after modifying id3tags
-F add ripping!
-F consider managing v1 vs v2 id3tags. consider not releasing "id3tag editing".
-F id3: option to not modify mp3 files, rather store id3 tag info seperately
-F examine winamp/musicmatch for feature ideas
-F have a api for 'advisory' messages vs. errors (MBMessageBox). Advisories goes to temp playing status label
-F use RVA2 or RVAD?
-R All credits/copywrites in About
-R Do code review
-R Read licensing for all libraries for copywrite notice
-R get rid of Irman or desguise it
X Win 2k
X customize extendedlistbox borders scrollbar slider
X http://www.codeproject.com/combobox/listboxex.asp
x About dialog
x CONFIG property sheet, fonts, colors
x ConfigIR, won't accept Test signals until an Edit box is clicked. Fix: got rid of ContinueModal
x Determine longest names, for Genre, Artist, Album, Song and Playlist
x Display counts of genres, artists, albums, songs
x Enhance Playlist building
x Finish working on MptrDLL::moveUp/Down
x Font changes need to be used everywhere, i.e. LoadPlaylist
x Font sizes, small, medium, large.
x Gs albums not sorted! TYER is polluted with "unknown"
x IR Config, shows OK even when it's not!
x Load
x LoadPlaylist error dialog - need to cancel it with remote ctrl
x Make config dialog tabbed. Add files to look for music etc.
x Maximize window on startup - adjust controls sizes
x Menu - wasted space
x More efficient db.
x Need to add a "Delete Playlist" function
x Read music files and create db.
x Save
x Scan results: total mp3 files processed # doesn't agree with total # songs
x Scan: appears that sort is broken again - various artists comes up first
x Shuffle
x Size of dialog. Try on Win98!
x Windows XP
x XXX Font saving is fucked up again!!!!!!!!!!!!!!!!!!!!!!!!!!
x add .mp3 etc. extensions to config, for file types to scan for
x add all genre
x add id3 edit capability
x add randomize playlist
x addSongToPlaylist - getNode shouldn't add item if missing. Loadplaylist could potentially do that.
x another status line at bottom or somewhere?
x bug: deleting song from playlist screws it up
x bug: mdb.playlist isn't getting modified upon up/down modification
x dbl clicking on all works again. Leave it in!
x delete song from playlist needs to stop it from playing
x don't save playlist if there isn't one
x examne insertSort
x finish loadplaylist dlg controls
x genre list in modify dialog needs to have both Genres.cpp and memDb genres
x genres,artists,albums label is chopped off on bottom
x get rid of ace
x get rid of resize border when maximized. It's not there.
x give error if player/file not found
x id3 edit a song under all and don't get any values
x if noirman keep getting annoying error dialog
x implement source code control, cvs or 
x improve shuffle
x initial value of font size is 65555 or something!
x integral height of genres/artists/albums is monkeying with height of control
x make genres wider
x mdb, remove "@"'s from serialized data
x name playlist dlg doesn't default like it should
x need a cancel out of load playlist dialog
x need a default font instead of all zeros in configfonts.cpp
x pop up menu needs a better look
x put FILE in ID3 Dialog
x reload memdb when musicdb location is changed.
x right click broken
x save maximized window
x save window size and pos on exit, reuse on startup
x scan/init dialogs, progress meter, etc.
x select current playing song in playlist
x sort albums by year, songs by track
x sort as playlist is loaded to match viewed sort order, maybe solution is
x spruce up config dialog
x tab order gets messed up. SWP_NOZORDER!
x thread bug when you hold down on previous button
x user settable colors and aspects of dialog
x wakeup screensaver when receive ircodes. NT done. XP done.
x xp wakeup broken
x? get a 1 page "coming soon" web site up. Drive people to it via search engines,     113     Tirco site, mass emailings?
xB " also Right click on Edit mp3 info doesn't get the right one 
xB Allow setting of font/size for panel labels
xB Branding, all lower case, trademark.
xB Config: If bad port keep getting error dialog can't open port
xB Config: if port disabled then disable Initialize button
xB Deal with Volume buttons either disable or document that volume buttons don't work
xB Done for DirectShow. Need to postmessage to cplayerdlg to call playloop, from xaudio/directshow when receive notif that song ended. This could have been why it crashed sometimes.
xB Irman should be disable upon new installation.
xB MBbox after loadplaylist too wide
xB Need better scheme for indexing songs, i.e. two files with same title acts weird
xB Not allowing " all" to be clicked in
xB Options Dlg too wide
xB Put Volume slider next to position slider - replace unused status box
xB Replace Kooware with Pecan Ventures???
xB ReqDlls debug vs release
xB Scan Results Dlg too wide
xB ScanDirectories: if unknown song title, use filename?
xB ScanDirectories: make unknown " unknown". Add no tagged files to " unknown"
xB ScanDirectories: redo it!
xB Statically link windoz libs when possible. Search for "Redistributale Files" to find the version api, VerFindFile()
xB Talk about Volume buttons in Help. Suggest using something other than Vol buttons if necessary.
xB Update id3lib to current release. Export my mods out of it.
xB addSongToPlaylist needs to handle duplicate named songs, and songs not found
xB catch errors when couldn't read tlen in addsongtodb
xB default colors to white/black
xB disguise irman lib. No need, it's freeware
xB hook up progress slider to directshow
xB id3lib todo    misc_support.h    misc_support.cpp    tag_render.cpp
xB integral controls getting sized differently need to set them explicitly all equal. Seems to be ok.
xB look at Nick Cave & the bad seeds - crashes mb when right clicking
xB make testharness only pop up if errors found
xB need to examine race conditions on m_Playlist etc.
xB not saving font on elm
xB purify, MusicDb::getSongs NameNums looks like a leak
xB right click in playlist and it edits the selected song in Songlist
xB upper left corner icon doesnt' show up on xp
xB use TLEN
xF Add SavePlaylist to remote control menu - just date/time stamp it.
xF Add play times to display
xF Allow to set color of cursor
xF Desktop shortcut at installation
xF add mp3header info to id3edit display
xF allow adjustment of label font sizes
xF get a 1 page "coming soon" web site up. Drive people to it via search engines,
xF icons, icons, icons
xF jazz up dialog, colors, logo something
xF remember browsing positions and return back to them
xF startup
xF systray icon?
xF winzip self extractor - $49.00
xF wise installer or installshield or at least a zip
xR Copy protection http://www.sampson-multimedia.com/copy-protection.html
xR Help
xR look in IE Favorites for list of free/cheap installers
xR uninstall
xR website kooware.com/MuzikBrowzer
*/
