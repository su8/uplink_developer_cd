# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=mikdx - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to mikdx - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "mikdx - Win32 Release" && "$(CFG)" != "mikdx - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "mikdx.mak" CFG="mikdx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mikdx - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "mikdx - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "mikdx - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "mikdx - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\mikmod.exe"

CLEAN : 
	-@erase ".\mikmod.exe"
	-@erase ".\Release\Mloader.obj"
	-@erase ".\Release\drv_ds.obj"
	-@erase ".\Release\Load_mtm.obj"
	-@erase ".\Release\Load_uni.obj"
	-@erase ".\Release\Mdriver.obj"
	-@erase ".\Release\Getopt.obj"
	-@erase ".\Release\Load_m15.obj"
	-@erase ".\Release\Wildfile.obj"
	-@erase ".\Release\Load_xm.obj"
	-@erase ".\Release\Load_ult.obj"
	-@erase ".\Release\Load_mod.obj"
	-@erase ".\Release\Munitrk.obj"
	-@erase ".\Release\Mwav.obj"
	-@erase ".\Release\Load_stm.obj"
	-@erase ".\Release\Drv_nos.obj"
	-@erase ".\Release\Mikmod.obj"
	-@erase ".\Release\Load_s3m.obj"
	-@erase ".\Release\Mplayer.obj"
	-@erase ".\Release\Mmio.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "dx3" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "__VISUALC__" /D "__NT__" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "dx3" /D "NDEBUG" /D "WIN32" /D "_CONSOLE"\
 /D "__VISUALC__" /D "__NT__" /Fp"$(INTDIR)/mikdx.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mikdx.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dx3\dsound.lib dx3\dxguid.lib winmm.lib /nologo /subsystem:console /machine:I386 /out:"mikmod.exe"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib dx3\dsound.lib dx3\dxguid.lib winmm.lib /nologo /subsystem:console\
 /incremental:no /pdb:"$(OUTDIR)/mikmod.pdb" /machine:I386 /out:"mikmod.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Mloader.obj" \
	"$(INTDIR)/drv_ds.obj" \
	"$(INTDIR)/Load_mtm.obj" \
	"$(INTDIR)/Load_uni.obj" \
	"$(INTDIR)/Mdriver.obj" \
	"$(INTDIR)/Getopt.obj" \
	"$(INTDIR)/Load_m15.obj" \
	"$(INTDIR)/Wildfile.obj" \
	"$(INTDIR)/Load_xm.obj" \
	"$(INTDIR)/Load_ult.obj" \
	"$(INTDIR)/Load_mod.obj" \
	"$(INTDIR)/Munitrk.obj" \
	"$(INTDIR)/Mwav.obj" \
	"$(INTDIR)/Load_stm.obj" \
	"$(INTDIR)/Drv_nos.obj" \
	"$(INTDIR)/Mikmod.obj" \
	"$(INTDIR)/Load_s3m.obj" \
	"$(INTDIR)/Mplayer.obj" \
	"$(INTDIR)/Mmio.obj"

"$(OUTDIR)\mikmod.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\mikmod.exe" "$(OUTDIR)\Debug\mikdx.bsc"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\mikdx.bsc"
	-@erase ".\Debug\Load_s3m.sbr"
	-@erase ".\Debug\Drv_nos.sbr"
	-@erase ".\Debug\Mplayer.sbr"
	-@erase ".\Debug\Mwav.sbr"
	-@erase ".\Debug\Load_mod.sbr"
	-@erase ".\Debug\Load_uni.sbr"
	-@erase ".\Debug\Mloader.sbr"
	-@erase ".\Debug\Load_m15.sbr"
	-@erase ".\Debug\Mikmod.sbr"
	-@erase ".\Debug\Wildfile.sbr"
	-@erase ".\Debug\Load_ult.sbr"
	-@erase ".\Debug\Mdriver.sbr"
	-@erase ".\Debug\Mmio.sbr"
	-@erase ".\Debug\Load_xm.sbr"
	-@erase ".\Debug\drv_ds.sbr"
	-@erase ".\Debug\Load_stm.sbr"
	-@erase ".\Debug\Load_mtm.sbr"
	-@erase ".\Debug\Munitrk.sbr"
	-@erase ".\Debug\Getopt.sbr"
	-@erase ".\mikmod.exe"
	-@erase ".\Debug\Load_stm.obj"
	-@erase ".\Debug\Load_mtm.obj"
	-@erase ".\Debug\Munitrk.obj"
	-@erase ".\Debug\Getopt.obj"
	-@erase ".\Debug\Load_s3m.obj"
	-@erase ".\Debug\Drv_nos.obj"
	-@erase ".\Debug\Mplayer.obj"
	-@erase ".\Debug\Mwav.obj"
	-@erase ".\Debug\Load_mod.obj"
	-@erase ".\Debug\Load_uni.obj"
	-@erase ".\Debug\Mloader.obj"
	-@erase ".\Debug\Load_m15.obj"
	-@erase ".\Debug\Mikmod.obj"
	-@erase ".\Debug\Wildfile.obj"
	-@erase ".\Debug\Load_ult.obj"
	-@erase ".\Debug\Mdriver.obj"
	-@erase ".\Debug\Mmio.obj"
	-@erase ".\Debug\Load_xm.obj"
	-@erase ".\Debug\drv_ds.obj"
	-@erase ".\mikmod.ilk"
	-@erase ".\Debug\mikmod.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "dx3" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "__VISUALC__" /D "__NT__" /FR /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "dx3" /D "_DEBUG" /D "WIN32" /D\
 "_CONSOLE" /D "__VISUALC__" /D "__NT__" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/mikdx.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mikdx.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/Load_s3m.sbr" \
	"$(INTDIR)/Drv_nos.sbr" \
	"$(INTDIR)/Mplayer.sbr" \
	"$(INTDIR)/Mwav.sbr" \
	"$(INTDIR)/Load_mod.sbr" \
	"$(INTDIR)/Load_uni.sbr" \
	"$(INTDIR)/Mloader.sbr" \
	"$(INTDIR)/Load_m15.sbr" \
	"$(INTDIR)/Mikmod.sbr" \
	"$(INTDIR)/Wildfile.sbr" \
	"$(INTDIR)/Load_ult.sbr" \
	"$(INTDIR)/Mdriver.sbr" \
	"$(INTDIR)/Mmio.sbr" \
	"$(INTDIR)/Load_xm.sbr" \
	"$(INTDIR)/drv_ds.sbr" \
	"$(INTDIR)/Load_stm.sbr" \
	"$(INTDIR)/Load_mtm.sbr" \
	"$(INTDIR)/Munitrk.sbr" \
	"$(INTDIR)/Getopt.sbr"

"$(OUTDIR)\Debug\mikdx.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dx3\dsound.lib dx3\dxguid.lib winmm.lib /nologo /subsystem:console /debug /machine:I386 /out:"mikmod.exe"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib dx3\dsound.lib dx3\dxguid.lib winmm.lib /nologo /subsystem:console\
 /incremental:yes /pdb:"$(OUTDIR)/mikmod.pdb" /debug /machine:I386\
 /out:"mikmod.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Load_stm.obj" \
	"$(INTDIR)/Load_mtm.obj" \
	"$(INTDIR)/Munitrk.obj" \
	"$(INTDIR)/Getopt.obj" \
	"$(INTDIR)/Load_s3m.obj" \
	"$(INTDIR)/Drv_nos.obj" \
	"$(INTDIR)/Mplayer.obj" \
	"$(INTDIR)/Mwav.obj" \
	"$(INTDIR)/Load_mod.obj" \
	"$(INTDIR)/Load_uni.obj" \
	"$(INTDIR)/Mloader.obj" \
	"$(INTDIR)/Load_m15.obj" \
	"$(INTDIR)/Mikmod.obj" \
	"$(INTDIR)/Wildfile.obj" \
	"$(INTDIR)/Load_ult.obj" \
	"$(INTDIR)/Mdriver.obj" \
	"$(INTDIR)/Mmio.obj" \
	"$(INTDIR)/Load_xm.obj" \
	"$(INTDIR)/drv_ds.obj"

"$(OUTDIR)\mikmod.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "mikdx - Win32 Release"
# Name "mikdx - Win32 Debug"

!IF  "$(CFG)" == "mikdx - Win32 Release"

!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Wildfile.h

!IF  "$(CFG)" == "mikdx - Win32 Release"

!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Drv_nos.c
DEP_CPP_DRV_N=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Drv_nos.obj" : $(SOURCE) $(DEP_CPP_DRV_N) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Drv_nos.obj" : $(SOURCE) $(DEP_CPP_DRV_N) "$(INTDIR)"

"$(INTDIR)\Drv_nos.sbr" : $(SOURCE) $(DEP_CPP_DRV_N) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Getopt.c

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Getopt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Getopt.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\Getopt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Load_m15.c
DEP_CPP_LOAD_=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Load_m15.obj" : $(SOURCE) $(DEP_CPP_LOAD_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Load_m15.obj" : $(SOURCE) $(DEP_CPP_LOAD_) "$(INTDIR)"

"$(INTDIR)\Load_m15.sbr" : $(SOURCE) $(DEP_CPP_LOAD_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Load_mod.c
DEP_CPP_LOAD_M=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Load_mod.obj" : $(SOURCE) $(DEP_CPP_LOAD_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Load_mod.obj" : $(SOURCE) $(DEP_CPP_LOAD_M) "$(INTDIR)"

"$(INTDIR)\Load_mod.sbr" : $(SOURCE) $(DEP_CPP_LOAD_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Load_mtm.c
DEP_CPP_LOAD_MT=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Load_mtm.obj" : $(SOURCE) $(DEP_CPP_LOAD_MT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Load_mtm.obj" : $(SOURCE) $(DEP_CPP_LOAD_MT) "$(INTDIR)"

"$(INTDIR)\Load_mtm.sbr" : $(SOURCE) $(DEP_CPP_LOAD_MT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Load_s3m.c
DEP_CPP_LOAD_S=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Load_s3m.obj" : $(SOURCE) $(DEP_CPP_LOAD_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Load_s3m.obj" : $(SOURCE) $(DEP_CPP_LOAD_S) "$(INTDIR)"

"$(INTDIR)\Load_s3m.sbr" : $(SOURCE) $(DEP_CPP_LOAD_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Load_stm.c
DEP_CPP_LOAD_ST=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Load_stm.obj" : $(SOURCE) $(DEP_CPP_LOAD_ST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Load_stm.obj" : $(SOURCE) $(DEP_CPP_LOAD_ST) "$(INTDIR)"

"$(INTDIR)\Load_stm.sbr" : $(SOURCE) $(DEP_CPP_LOAD_ST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Load_ult.c
DEP_CPP_LOAD_U=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Load_ult.obj" : $(SOURCE) $(DEP_CPP_LOAD_U) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Load_ult.obj" : $(SOURCE) $(DEP_CPP_LOAD_U) "$(INTDIR)"

"$(INTDIR)\Load_ult.sbr" : $(SOURCE) $(DEP_CPP_LOAD_U) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Load_uni.c
DEP_CPP_LOAD_UN=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Load_uni.obj" : $(SOURCE) $(DEP_CPP_LOAD_UN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Load_uni.obj" : $(SOURCE) $(DEP_CPP_LOAD_UN) "$(INTDIR)"

"$(INTDIR)\Load_uni.sbr" : $(SOURCE) $(DEP_CPP_LOAD_UN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Load_xm.c
DEP_CPP_LOAD_X=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Load_xm.obj" : $(SOURCE) $(DEP_CPP_LOAD_X) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Load_xm.obj" : $(SOURCE) $(DEP_CPP_LOAD_X) "$(INTDIR)"

"$(INTDIR)\Load_xm.sbr" : $(SOURCE) $(DEP_CPP_LOAD_X) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mdriver.c
DEP_CPP_MDRIV=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Mdriver.obj" : $(SOURCE) $(DEP_CPP_MDRIV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Mdriver.obj" : $(SOURCE) $(DEP_CPP_MDRIV) "$(INTDIR)"

"$(INTDIR)\Mdriver.sbr" : $(SOURCE) $(DEP_CPP_MDRIV) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mikmod.c
DEP_CPP_MIKMO=\
	".\Wildfile.h"\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Mikmod.obj" : $(SOURCE) $(DEP_CPP_MIKMO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Mikmod.obj" : $(SOURCE) $(DEP_CPP_MIKMO) "$(INTDIR)"

"$(INTDIR)\Mikmod.sbr" : $(SOURCE) $(DEP_CPP_MIKMO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mikmod.h

!IF  "$(CFG)" == "mikdx - Win32 Release"

!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mloader.c
DEP_CPP_MLOAD=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Mloader.obj" : $(SOURCE) $(DEP_CPP_MLOAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Mloader.obj" : $(SOURCE) $(DEP_CPP_MLOAD) "$(INTDIR)"

"$(INTDIR)\Mloader.sbr" : $(SOURCE) $(DEP_CPP_MLOAD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mmio.c
DEP_CPP_MMIO_=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Mmio.obj" : $(SOURCE) $(DEP_CPP_MMIO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Mmio.obj" : $(SOURCE) $(DEP_CPP_MMIO_) "$(INTDIR)"

"$(INTDIR)\Mmio.sbr" : $(SOURCE) $(DEP_CPP_MMIO_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mplayer.c
DEP_CPP_MPLAY=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Mplayer.obj" : $(SOURCE) $(DEP_CPP_MPLAY) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Mplayer.obj" : $(SOURCE) $(DEP_CPP_MPLAY) "$(INTDIR)"

"$(INTDIR)\Mplayer.sbr" : $(SOURCE) $(DEP_CPP_MPLAY) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mtypes.h

!IF  "$(CFG)" == "mikdx - Win32 Release"

!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Munitrk.c
DEP_CPP_MUNIT=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Munitrk.obj" : $(SOURCE) $(DEP_CPP_MUNIT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Munitrk.obj" : $(SOURCE) $(DEP_CPP_MUNIT) "$(INTDIR)"

"$(INTDIR)\Munitrk.sbr" : $(SOURCE) $(DEP_CPP_MUNIT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Wildfile.c
DEP_CPP_WILDF=\
	".\Wildfile.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Wildfile.obj" : $(SOURCE) $(DEP_CPP_WILDF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Wildfile.obj" : $(SOURCE) $(DEP_CPP_WILDF) "$(INTDIR)"

"$(INTDIR)\Wildfile.sbr" : $(SOURCE) $(DEP_CPP_WILDF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drv_ds.c
DEP_CPP_DRV_D=\
	".\Mikmod.h"\
	".\dx3\dsound.h"\
	".\Mtypes.h"\
	".\dx3\d3dtypes.h"\
	".\dx3\ddraw.h"\
	
NODEP_CPP_DRV_D=\
	".\dx3\subwtype.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\drv_ds.obj" : $(SOURCE) $(DEP_CPP_DRV_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\drv_ds.obj" : $(SOURCE) $(DEP_CPP_DRV_D) "$(INTDIR)"

"$(INTDIR)\drv_ds.sbr" : $(SOURCE) $(DEP_CPP_DRV_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mwav.c
DEP_CPP_MWAV_=\
	".\Mikmod.h"\
	".\Mtypes.h"\
	

!IF  "$(CFG)" == "mikdx - Win32 Release"


"$(INTDIR)\Mwav.obj" : $(SOURCE) $(DEP_CPP_MWAV_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mikdx - Win32 Debug"


"$(INTDIR)\Mwav.obj" : $(SOURCE) $(DEP_CPP_MWAV_) "$(INTDIR)"

"$(INTDIR)\Mwav.sbr" : $(SOURCE) $(DEP_CPP_MWAV_) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
