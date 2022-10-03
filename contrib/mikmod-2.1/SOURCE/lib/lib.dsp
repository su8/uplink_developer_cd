# Microsoft Developer Studio Project File - Name="lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lib.mak" CFG="lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "lib - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "__VISUALC__" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\mikmod.lib"

!ELSEIF  "$(CFG)" == "lib - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "lib - Win32 Release"
# Name "lib - Win32 Debug"
# Begin Source File

SOURCE=..\drv_ds.c
# End Source File
# Begin Source File

SOURCE=..\DRV_NOS.c
# End Source File
# Begin Source File

SOURCE=..\dx3\Dsound.lib
# End Source File
# Begin Source File

SOURCE=..\dx3\Dxguid.lib
# End Source File
# Begin Source File

SOURCE=..\GETOPT.c
# End Source File
# Begin Source File

SOURCE=..\LOAD_M15.c
# End Source File
# Begin Source File

SOURCE=..\LOAD_MOD.c
# End Source File
# Begin Source File

SOURCE=..\LOAD_MTM.c
# End Source File
# Begin Source File

SOURCE=..\LOAD_S3M.c
# End Source File
# Begin Source File

SOURCE=..\LOAD_STM.c
# End Source File
# Begin Source File

SOURCE=..\LOAD_ULT.c
# End Source File
# Begin Source File

SOURCE=..\LOAD_UNI.c
# End Source File
# Begin Source File

SOURCE=..\LOAD_XM.c
# End Source File
# Begin Source File

SOURCE=..\MDRIVER.c
# End Source File
# Begin Source File

SOURCE=..\Mikmod.h
# End Source File
# Begin Source File

SOURCE=..\MLOADER.c
# End Source File
# Begin Source File

SOURCE=..\MMIO.c
# End Source File
# Begin Source File

SOURCE=..\Mplayer.c
# End Source File
# Begin Source File

SOURCE=..\Mtypes.h
# End Source File
# Begin Source File

SOURCE=..\MUNITRK.c
# End Source File
# Begin Source File

SOURCE=..\MWAV.c
# End Source File
# Begin Source File

SOURCE=..\Wildfile.c
# End Source File
# End Target
# End Project
