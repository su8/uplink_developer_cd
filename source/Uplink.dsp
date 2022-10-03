# Microsoft Developer Studio Project File - Name="Uplink" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Uplink - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Uplink.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Uplink.mak" CFG="Uplink - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Uplink - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Uplink - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Uplink - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "obj\win32"
# PROP Intermediate_Dir "obj\win32"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /GX /O2 /Ob2 /I "C:\Development\UplinkDevCD\source" /I "../lib/bungle" /I "../lib/eclipse" /I "../lib/gucci" /I "../lib/redshirt" /I "../lib/slasher" /I "../lib/soundgarden" /I "../lib/tosser" /I "../lib/vanbakel" /I "../contrib/irclib" /I "../contrib/tcp4u.330/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "RELEASE" /FR /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /map:"../Uplink.map" /machine:I386 /out:"..\Uplink.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Uplink - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "obj\win32\"
# PROP Intermediate_Dir "obj\win32\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /Gm /GX /ZI /Od /I "C:\Development\UplinkDevCD\source" /I "../lib/bungle" /I "../lib/eclipse" /I "../lib/gucci" /I "../lib/redshirt" /I "../lib/slasher" /I "../lib/soundgarden" /I "../lib/tosser" /I "../lib/vanbakel" /I "../contrib/irclib" /I "../contrib/tcp4u.330/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr /YX /FD /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:0.7 /subsystem:console /profile /map /debug /machine:I386 /out:"..\Uplink.exe"

!ENDIF 

# Begin Target

# Name "Uplink - Win32 Release"
# Name "Uplink - Win32 Debug"
# Begin Group "Lib"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\lib\eclipse\eclipse.lib
# End Source File
# Begin Source File

SOURCE=..\lib\gucci\gucci.lib
# End Source File
# Begin Source File

SOURCE=..\lib\soundgarden\soundgarden.lib
# End Source File
# Begin Source File

SOURCE=..\lib\vanbakel\vanbakel.lib
# End Source File
# Begin Source File

SOURCE=..\lib\redshirt\redshirt.lib
# End Source File
# Begin Source File

SOURCE=..\lib\bungle\bungle.lib
# End Source File
# Begin Source File

SOURCE=..\contrib\tcp4u.330\tcp4w32\tcp4w32.lib
# End Source File
# Begin Source File

SOURCE=..\contrib\irclib\irclib.lib
# End Source File
# Begin Source File

SOURCE="..\..\..\Program Files\Microsoft Visual Studio\VC98\Lib\GLU32.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\..\Program Files\Microsoft Visual Studio\VC98\Lib\OPENGL32.LIB"
# End Source File
# Begin Source File

SOURCE="..\contrib\glut-3.7\glut32.lib"
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Game\Data\data.cpp
# End Source File
# Begin Source File

SOURCE=.\Game\Data\data.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\game\game.cpp
# End Source File
# Begin Source File

SOURCE=.\game\game.h
# End Source File
# Begin Source File

SOURCE=.\Game\gameobituary.cpp
# End Source File
# Begin Source File

SOURCE=.\Game\gameobituary.h
# End Source File
# Begin Source File

SOURCE=.\game\ScriptLibrary.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ScriptLibrary.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Group "Computer"

# PROP Default_Filter ""
# Begin Group "ComputerScreen"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\World\Computer\computerscreen\bbsscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\bbsscreen.h
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\ComputerScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\ComputerScreen.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\cypherscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\cypherscreen.h
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\DialogScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\DialogScreen.h
# End Source File
# Begin Source File

SOURCE=.\WORLD\COMPUTER\computerscreen\disconnectedscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\WORLD\COMPUTER\computerscreen\disconnectedscreen.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\genericscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\genericscreen.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\highsecurityscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\highsecurityscreen.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\linksscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\linksscreen.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\logscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\logscreen.h
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\MenuScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\MenuScreen.h
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\MessageScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\MessageScreen.h
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\PasswordScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\computer\ComputerScreen\PasswordScreen.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\shareslistscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\shareslistscreen.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\useridscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\computerscreen\useridscreen.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\World\Computer\bankaccount.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\bankaccount.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\bankcomputer.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\bankcomputer.h
# End Source File
# Begin Source File

SOURCE=.\world\computer\Computer.cpp
# End Source File
# Begin Source File

SOURCE=.\world\computer\Computer.h
# End Source File
# Begin Source File

SOURCE=.\world\computer\DataBank.cpp
# End Source File
# Begin Source File

SOURCE=.\world\computer\DataBank.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\gateway.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\gateway.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\gatewaydef.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\gatewaydef.h
# End Source File
# Begin Source File

SOURCE=.\world\computer\lancomputer.cpp
# End Source File
# Begin Source File

SOURCE=.\world\computer\lancomputer.h
# End Source File
# Begin Source File

SOURCE=.\world\computer\lanmonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\world\computer\lanmonitor.h
# End Source File
# Begin Source File

SOURCE=.\World\computer\LogBank.cpp
# End Source File
# Begin Source File

SOURCE=.\World\computer\LogBank.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\recordbank.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\recordbank.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\security.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\security.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\securitymonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\securitymonitor.h
# End Source File
# Begin Source File

SOURCE=.\World\Computer\securitysystem.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Computer\securitysystem.h
# End Source File
# End Group
# Begin Group "Company"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\world\company\Company.cpp
# End Source File
# Begin Source File

SOURCE=.\world\company\Company.h
# End Source File
# Begin Source File

SOURCE=.\World\company\CompanyUplink.cpp
# End Source File
# Begin Source File

SOURCE=.\World\company\CompanyUplink.h
# End Source File
# Begin Source File

SOURCE=.\World\company\mission.cpp
# End Source File
# Begin Source File

SOURCE=.\World\company\mission.h
# End Source File
# Begin Source File

SOURCE=.\World\Company\news.cpp
# End Source File
# Begin Source File

SOURCE=.\World\Company\news.h
# End Source File
# Begin Source File

SOURCE=.\World\company\Sale.cpp
# End Source File
# Begin Source File

SOURCE=.\World\company\Sale.h
# End Source File
# End Group
# Begin Group "Generator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\World\generator\consequencegenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\consequencegenerator.h
# End Source File
# Begin Source File

SOURCE=.\world\generator\demoplotgenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\demoplotgenerator.h
# End Source File
# Begin Source File

SOURCE=.\world\generator\langenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\world\generator\langenerator.h
# End Source File
# Begin Source File

SOURCE=.\World\generator\MissionGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\MissionGenerator.h
# End Source File
# Begin Source File

SOURCE=.\World\generator\namegenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\namegenerator.h
# End Source File
# Begin Source File

SOURCE=.\World\generator\newsgenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\newsgenerator.h
# End Source File
# Begin Source File

SOURCE=.\World\generator\numbergenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\numbergenerator.h
# End Source File
# Begin Source File

SOURCE=.\world\generator\plotgenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\plotgenerator.h
# End Source File
# Begin Source File

SOURCE=.\World\generator\recordgenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\recordgenerator.h
# End Source File
# Begin Source File

SOURCE=.\World\generator\WorldGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\World\generator\WorldGenerator.h
# End Source File
# End Group
# Begin Group "Scheduler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\World\scheduler\arrestevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\arrestevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\attemptmissionevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\attemptmissionevent.h
# End Source File
# Begin Source File

SOURCE=.\world\scheduler\bankrobberyevent.cpp
# End Source File
# Begin Source File

SOURCE=.\world\scheduler\bankrobberyevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\changegatewayevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\changegatewayevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\eventscheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\eventscheduler.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\installhardwareevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\installhardwareevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\notificationevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\notificationevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\runplotsceneevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\runplotsceneevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\seizegatewayevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\seizegatewayevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\shotbyfedsevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\shotbyfedsevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\uplinkevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\uplinkevent.h
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\warningevent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\scheduler\warningevent.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\World\agent.cpp
# End Source File
# Begin Source File

SOURCE=.\World\agent.h
# End Source File
# Begin Source File

SOURCE=.\world\connection.cpp
# End Source File
# Begin Source File

SOURCE=.\world\connection.h
# End Source File
# Begin Source File

SOURCE=.\world\Date.cpp
# End Source File
# Begin Source File

SOURCE=.\world\Date.h
# End Source File
# Begin Source File

SOURCE=.\world\Message.cpp
# End Source File
# Begin Source File

SOURCE=.\world\Message.h
# End Source File
# Begin Source File

SOURCE=.\world\Person.cpp
# End Source File
# Begin Source File

SOURCE=.\world\Person.h
# End Source File
# Begin Source File

SOURCE=.\World\player.cpp
# End Source File
# Begin Source File

SOURCE=.\world\Player.h
# End Source File
# Begin Source File

SOURCE=.\world\rating.cpp
# End Source File
# Begin Source File

SOURCE=.\world\rating.h
# End Source File
# Begin Source File

SOURCE=.\world\VLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\world\VLocation.h
# End Source File
# Begin Source File

SOURCE=.\world\World.cpp
# End Source File
# Begin Source File

SOURCE=.\world\World.h
# End Source File
# End Group
# Begin Group "View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\view\fps.cpp
# End Source File
# Begin Source File

SOURCE=.\view\fps.h
# End Source File
# Begin Source File

SOURCE=.\view\View.cpp
# End Source File
# Begin Source File

SOURCE=.\view\View.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Group "LocalInterface"

# PROP Default_Filter ""
# Begin Group "Worldmap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\interface\localinterface\worldmap\rectangle.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\worldmap\worldmap_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\worldmap\worldmap_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\worldmap\worldmap_layout.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\worldmap\worldmap_layout.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\interface\localinterface\analyser_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\analyser_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\cheat_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\cheat_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\email_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\email_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\evtqueue_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\evtqueue_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\finance_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\finance_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\gateway_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\gateway_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\hud_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\hud_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\hw_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\hw_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\irc_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\irc_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\keyboardinterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\LocalInterface\KeyboardInterface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\lan_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\lan_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\localinterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\LocalInterface\LocalInterface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\localinterfacescreen.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\LocalInterface\LocalInterfaceScreen.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\memory_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\memory_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\mission_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\mission_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\phonedialler.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\LocalInterface\phonedialler.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\sendmail_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\sendmail_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\status_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\status_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\SW_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\localinterface\SW_Interface.h
# End Source File
# End Group
# Begin Group "RemoteInterface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\interface\remoteinterface\academicscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\academicscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\accountscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\accountscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\bbsscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\bbsscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\changegatewayscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\changegatewayscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\codecardscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\codecardscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\companyinfoscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\companyinfoscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\consolescreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\consolescreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\contactscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\contactscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\criminalscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\criminalscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\cypherscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\cypherscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\dialogscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\dialogscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\disconnectedscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\disconnectedscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\faithscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\faithscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\fileserverscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\fileserverscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\highsecurityscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\highsecurityscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\hwsalesscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\hwsalesscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\linksscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\linksscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\loansscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\loansscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\logscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\logscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\menuscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\menuscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\messagescreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\messagescreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\nearestgatewayscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\nearestgatewayscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\newsscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\newsscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\nuclearwarscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\nuclearwarscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\passwordscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\passwordscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\protovisionscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\protovisionscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\radiotransmitterscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\radiotransmitterscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\rankingscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\rankingscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\recordscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\recordscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\remoteinterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\RemoteInterface\remoteinterface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\remoteinterfacescreen.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\RemoteInterface\RemoteInterfaceScreen.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\securityscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\securityscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\shareslistscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\shareslistscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\sharesviewscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\sharesviewscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\socialsecurityscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\socialsecurityscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\swsalesscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\swsalesscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\useridscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\useridscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\voiceanalysisscreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\voiceanalysisscreen_interface.h
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\voicephonescreen_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\remoteinterface\voicephonescreen_interface.h
# End Source File
# End Group
# Begin Group "TaskManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\interface\taskmanager\decrypter.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\decrypter.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\decypher.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\decypher.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\defrag.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\defrag.h
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\dictionaryhacker.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\dictionaryhacker.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\faith.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\faith.h
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\filecopier.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\filecopier.h
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\filedeleter.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\filedeleter.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\firewalldisable.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\firewalldisable.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\gatewaynuke.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\gatewaynuke.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\iplookup.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\iplookup.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\ipprobe.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\ipprobe.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\lanforce.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\lanforce.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\lanprobe.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\lanprobe.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\lanscan.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\lanscan.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\lanspoof.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\lanspoof.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\logdeleter.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\logdeleter.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\logmodifier.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\logmodifier.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\logundeleter.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\logundeleter.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\motionsensor.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\motionsensor.h
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\passwordbreaker.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\passwordbreaker.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\proxydisable.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\proxydisable.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\revelation.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\revelation.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\revelationtracker.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\revelationtracker.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\securitybypass.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\securitybypass.h
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\taskmanager.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\taskmanager.h
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\TraceTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\TraceTracker.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\tutorial.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\tutorial.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\uplinkagentlist.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\uplinkagentlist.h
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\UplinkTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\TaskManager\UplinkTask.h
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\voiceanalyser.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\taskmanager\voiceanalyser.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Interface\Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\Interface.h
# End Source File
# Begin Source File

SOURCE=.\Interface\InterfaceScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface\InterfaceScreen.h
# End Source File
# Begin Source File

SOURCE=.\interface\scrollbox.cpp
# End Source File
# Begin Source File

SOURCE=.\interface\scrollbox.h
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\Interfaces\ClientCommsInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Interfaces\ClientCommsInterface.h
# End Source File
# Begin Source File

SOURCE=.\Network\interfaces\clientstatusinterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\interfaces\clientstatusinterface.h
# End Source File
# Begin Source File

SOURCE=.\Network\Interfaces\NetworkScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Interfaces\NetworkScreen.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Network\clientconnection.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\clientconnection.h
# End Source File
# Begin Source File

SOURCE=.\network\Network.cpp
# End Source File
# Begin Source File

SOURCE=.\network\Network.h
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkClient.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkClient.h
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\NetworkServer.h
# End Source File
# End Group
# Begin Group "MainMenu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Mainmenu\closing_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\closing_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\connectionlost_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\connectionlost_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\demogameover_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\demogameover_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\disavowed_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\disavowed_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\firsttimeloading_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\firsttimeloading_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\genericoptions_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\genericoptions_interface.h
# End Source File
# Begin Source File

SOURCE=.\mainmenu\graphicoptions_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\mainmenu\graphicoptions_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\loading_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\loading_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\login_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\login_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\Mainmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\mainmenu\MainMenu.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\mainmenuscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\mainmenu\MainMenuScreen.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\networkoptions_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\networkoptions_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\obituary_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\obituary_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\options_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\options_interface.h
# End Source File
# Begin Source File

SOURCE=.\MAINMENU\revelationlost_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\MAINMENU\revelationlost_interface.h
# End Source File
# Begin Source File

SOURCE=.\MAINMENU\revelationwon_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\MAINMENU\revelationwon_interface.h
# End Source File
# Begin Source File

SOURCE=.\mainmenu\theme_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\mainmenu\theme_interface.h
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\theteam_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Mainmenu\theteam_interface.h
# End Source File
# Begin Source File

SOURCE=.\mainmenu\warezgameover_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\mainmenu\warezgameover_interface.h
# End Source File
# End Group
# Begin Group "App"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\app\app.cpp
# End Source File
# Begin Source File

SOURCE=.\app\app.h
# End Source File
# Begin Source File

SOURCE=.\app\decode.c
# End Source File
# Begin Source File

SOURCE=.\app\dos2unix.cpp
# End Source File
# Begin Source File

SOURCE=.\app\dos2unix.h
# End Source File
# Begin Source File

SOURCE=.\app\globals.h
# End Source File
# Begin Source File

SOURCE=.\App\miscutils.cpp
# End Source File
# Begin Source File

SOURCE=.\App\miscutils.h
# End Source File
# Begin Source File

SOURCE=.\app\opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\app\opengl.h
# End Source File
# Begin Source File

SOURCE=.\App\opengl_interface.cpp
# End Source File
# Begin Source File

SOURCE=.\App\opengl_interface.h
# End Source File
# Begin Source File

SOURCE=.\App\Probability.cpp
# End Source File
# Begin Source File

SOURCE=.\App\Probability.h
# End Source File
# Begin Source File

SOURCE=.\app\serialise.cpp
# End Source File
# Begin Source File

SOURCE=.\app\serialise.h
# End Source File
# Begin Source File

SOURCE=.\app\UplinkObject.cpp
# End Source File
# Begin Source File

SOURCE=.\app\UplinkObject.h
# End Source File
# End Group
# Begin Group "Options"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Options\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Options\Options.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resources.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\uplink.cpp
# End Source File
# End Target
# End Project
