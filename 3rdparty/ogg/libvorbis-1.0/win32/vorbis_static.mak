# Microsoft Developer Studio Generated NMAKE File, Based on vorbis_static.dsp
!IF "$(CFG)" == ""
CFG=vorbis_static - Win32 Debug
!MESSAGE No configuration specified. Defaulting to vorbis_static - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "vorbis_static - Win32 Release" && "$(CFG)" != "vorbis_static - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vorbis_static.mak" CFG="vorbis_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vorbis_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "vorbis_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "vorbis_static - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\vorbis_static.lib"


CLEAN :
	-@erase "$(INTDIR)\analysis.obj"
	-@erase "$(INTDIR)\bitrate.obj"
	-@erase "$(INTDIR)\block.obj"
	-@erase "$(INTDIR)\codebook.obj"
	-@erase "$(INTDIR)\envelope.obj"
	-@erase "$(INTDIR)\floor0.obj"
	-@erase "$(INTDIR)\floor1.obj"
	-@erase "$(INTDIR)\info.obj"
	-@erase "$(INTDIR)\lpc.obj"
	-@erase "$(INTDIR)\lsp.obj"
	-@erase "$(INTDIR)\mapping0.obj"
	-@erase "$(INTDIR)\mdct.obj"
	-@erase "$(INTDIR)\psy.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\res0.obj"
	-@erase "$(INTDIR)\sharedbook.obj"
	-@erase "$(INTDIR)\smallft.obj"
	-@erase "$(INTDIR)\synthesis.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\window.obj"
	-@erase "$(OUTDIR)\vorbis_static.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W2 /GX /O2 /I "..\include" /I "..\..\ogg\include" /I "..\..\libogg-1.0\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\vorbis_static.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vorbis_static.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\vorbis_static.lib" 
LIB32_OBJS= \
	"$(INTDIR)\analysis.obj" \
	"$(INTDIR)\bitrate.obj" \
	"$(INTDIR)\block.obj" \
	"$(INTDIR)\codebook.obj" \
	"$(INTDIR)\envelope.obj" \
	"$(INTDIR)\floor0.obj" \
	"$(INTDIR)\floor1.obj" \
	"$(INTDIR)\info.obj" \
	"$(INTDIR)\lpc.obj" \
	"$(INTDIR)\lsp.obj" \
	"$(INTDIR)\mapping0.obj" \
	"$(INTDIR)\mdct.obj" \
	"$(INTDIR)\psy.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\res0.obj" \
	"$(INTDIR)\sharedbook.obj" \
	"$(INTDIR)\smallft.obj" \
	"$(INTDIR)\synthesis.obj" \
	"$(INTDIR)\window.obj"

"$(OUTDIR)\vorbis_static.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "vorbis_static - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\vorbis_static_d.lib"


CLEAN :
	-@erase "$(INTDIR)\analysis.obj"
	-@erase "$(INTDIR)\bitrate.obj"
	-@erase "$(INTDIR)\block.obj"
	-@erase "$(INTDIR)\codebook.obj"
	-@erase "$(INTDIR)\envelope.obj"
	-@erase "$(INTDIR)\floor0.obj"
	-@erase "$(INTDIR)\floor1.obj"
	-@erase "$(INTDIR)\info.obj"
	-@erase "$(INTDIR)\lpc.obj"
	-@erase "$(INTDIR)\lsp.obj"
	-@erase "$(INTDIR)\mapping0.obj"
	-@erase "$(INTDIR)\mdct.obj"
	-@erase "$(INTDIR)\psy.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\res0.obj"
	-@erase "$(INTDIR)\sharedbook.obj"
	-@erase "$(INTDIR)\smallft.obj"
	-@erase "$(INTDIR)\synthesis.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\window.obj"
	-@erase "$(OUTDIR)\vorbis_static_d.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\include" /I "..\..\ogg\include" /I "..\..\libogg-1.0\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vorbis_static.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\vorbis_static_d.lib" 
LIB32_OBJS= \
	"$(INTDIR)\analysis.obj" \
	"$(INTDIR)\bitrate.obj" \
	"$(INTDIR)\block.obj" \
	"$(INTDIR)\codebook.obj" \
	"$(INTDIR)\envelope.obj" \
	"$(INTDIR)\floor0.obj" \
	"$(INTDIR)\floor1.obj" \
	"$(INTDIR)\info.obj" \
	"$(INTDIR)\lpc.obj" \
	"$(INTDIR)\lsp.obj" \
	"$(INTDIR)\mapping0.obj" \
	"$(INTDIR)\mdct.obj" \
	"$(INTDIR)\psy.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\res0.obj" \
	"$(INTDIR)\sharedbook.obj" \
	"$(INTDIR)\smallft.obj" \
	"$(INTDIR)\synthesis.obj" \
	"$(INTDIR)\window.obj"

"$(OUTDIR)\vorbis_static_d.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("vorbis_static.dep")
!INCLUDE "vorbis_static.dep"
!ELSE 
!MESSAGE Warning: cannot find "vorbis_static.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "vorbis_static - Win32 Release" || "$(CFG)" == "vorbis_static - Win32 Debug"
SOURCE=..\lib\analysis.c

"$(INTDIR)\analysis.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\bitrate.c

"$(INTDIR)\bitrate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\block.c

"$(INTDIR)\block.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\codebook.c

"$(INTDIR)\codebook.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\envelope.c

"$(INTDIR)\envelope.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\floor0.c

"$(INTDIR)\floor0.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\floor1.c

"$(INTDIR)\floor1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\info.c

"$(INTDIR)\info.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\lpc.c

"$(INTDIR)\lpc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\lsp.c

"$(INTDIR)\lsp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\mapping0.c

"$(INTDIR)\mapping0.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\mdct.c

"$(INTDIR)\mdct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\psy.c

"$(INTDIR)\psy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\registry.c

"$(INTDIR)\registry.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\res0.c

"$(INTDIR)\res0.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\sharedbook.c

"$(INTDIR)\sharedbook.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\smallft.c

"$(INTDIR)\smallft.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\synthesis.c

"$(INTDIR)\synthesis.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\lib\window.c

"$(INTDIR)\window.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

