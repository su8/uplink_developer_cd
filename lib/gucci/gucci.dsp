# Microsoft Developer Studio Project File - Name="gucci" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gucci - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gucci.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gucci.mak" CFG="gucci - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gucci - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gucci - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gucci - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "win32"
# PROP Intermediate_Dir "win32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /w /W0 /GX /O2 /Ob2 /I "..\tosser" /I "..\..\contrib\gltt-2.5" /I "..\..\contrib\tiff-3.4" /I "..\..\contrib\glut-3.7" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"gucci.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"gucci.lib"

!ELSEIF  "$(CFG)" == "gucci - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "win32"
# PROP Intermediate_Dir "win32"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /w /W0 /GX /ZI /Od /I "..\tosser" /I "..\..\contrib\gltt-2.5" /I "..\..\contrib\tiff-3.4" /I "..\..\contrib\glut-3.7" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"gucci.lib"

!ENDIF 

# Begin Target

# Name "gucci - Win32 Release"
# Name "gucci - Win32 Debug"
# Begin Source File

SOURCE=.\Gucci.cpp
# End Source File
# Begin Source File

SOURCE=.\gucci.h
# End Source File
# Begin Source File

SOURCE=.\gucci_glut.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\resize_win32.cpp
# End Source File
# Begin Source File

SOURCE="..\..\..\Contrib\tiff-3.4\libtiff.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\Contrib\freetype-1.3.1\freetype.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\Contrib\gltt-2.5\gltt.lib"
# End Source File
# End Target
# End Project
