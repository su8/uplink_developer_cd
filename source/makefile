#makefile for Uplink

# Uncomment below for release builds
RELEASE=release
VERSION=1.31

# -include ../stlport.mk

GL_LIBDIR = /usr/X11R6/lib
#GL_LIBDIR = /usr/lib
CONTRIB_DIR:= $(shell pwd)/../../contrib
#/home/jak/devel/contrib
TCP4U_DIR = $(CONTRIB_DIR)/tcp4u.330
GLTT_DIR = $(CONTRIB_DIR)/install

FTGL_DIR_INC = $(CONTRIB_DIR)/ftgl_dist/src
FTGL_DIR_LIB = $(CONTRIB_DIR)/ftgl_dist/linux
FT2_DIR1 = $(CONTRIB_DIR)/install/include/freetype2
FT2_DIR2 = $(CONTRIB_DIR)/install/include/freetype2/freetype

# for GLTT
FONTINCS = -I$(CONTRIB_DIR)/install
STATIC_FONTLIBS = $(GLTT_DIR)/lib/libgltt.a  /usr/lib/libttf.a
#DYNAMIC_FONTLIBS = -lttf

# for FTGL
#FONTINCS = -I$(FTGL_DIR_INC) -I$(FT2_DIR1) -I$(FT2_DIR2)
#STATIC_FONTLIBS = -L$(FTGL_DIR_LIB) -lftgl -lfreetype
##$(CONTRIB_DIR)/install/lib/libfreetype.a
#DYNAMIC_FONTLIBS = 

#MikMod:
#MIKMOD_DIR = $(CONTRIB_DIR)/install
#STATIC_SOUNDLIBS = $(MIKMOD_DIR)/lib/libmikmod.a
#DYNAMIC_SOUNDLIBS =

#SDL_Mixer:

STATIC_SDLLIBS = $(CONTRIB_DIR)/install/lib/libSDL_mixer.a \
                   $(CONTRIB_DIR)/install/lib/libSDL.a \
		   /usr/lib/libasound.a \
		   /usr/lib/libesd.a

#DYNAMIC_SDLLIBS = $(CONTRIB_DIR)/install/lib/libSDL.la \
#	            $(CONTRIB_DIR)/install/lib/libSDL_mixer.la

DYNAMIC_SDLLIBS = $(CONTRIB_DIR)/install/lib/libSDL-1.2.so.0 \
		    $(CONTRIB_DIR)/install/lib/libSDL_mixer-1.2.so.0

#GLUT_LIB = -lglut
#GL_LIB = -L$(GL_LIBDIR) -lGL -lGLU 
GL_LIB = $(GL_LIBDIR)/libGL.so.1 $(GL_LIBDIR)/libGLU.so.1
#-lXmu 
#-lXxf86vm				

IRCLIB_DIR = $(CONTRIB_DIR)/irclib

LIBTOOL=libtool

INCLUDES = 	-I.					\
		-I/usr/X11R6/include/			\
		-I../lib/eclipse/			\
		-I../lib/tosser/			\
		-I../lib/soundgarden/			\
		-I../lib/vanbakel/			\
		-I../lib/gucci/				\
		-I../lib/bungle				\
		-I../lib/redshirt/			\
		-I../lib/slasher			\
		-I$(TCP4U_DIR)/Include			\
		$(FONTINCS)				\
		-I$(IRCLIB_DIR)				\
		-I$(IRCLIB_DIR)/linux

INTERNALLIBS = ../lib/gucci/libgucci.a			\
	       ../lib/eclipse/libeclipse.a		\
	       ../lib/vanbakel/libvanbakel.a		\
	       ../lib/soundgarden/libsoundgarden.a	\
	       ../lib/redshirt/libredshirt.a		\
	       ../lib/bungle/libbungle.a		
#	       ../lib/slasher/libslasher.a

STATICLIBS =    -Wl,-Bstatic			        \
		$(INTERNALLIBS)				\
		-L$(TCP4U_DIR)/src      		\
		-L$(IRCLIB_DIR)				\
		/usr/lib/libtiff.a			\
		/usr/lib/libjpeg.a -lz			\
		$(STATIC_FONTLIBS)			\
		-ltcp4ux				\
		-lirclib				

#		$(-lXxf86vm				


DYNAMICLIBS =   -Wl,-Bdynamic				\
		$(GLUT_LIB)  $(GL_LIB)	\
		$(DYNAMIC_FONTLIBS)			\
		-ldl 

#-lpthread

CXXLIB =	-Wl,-Bstatic $(STLPORTLIB) -lstdc++ 

MATHLIB =       -Wl,-Bdynamic -lm

ifndef RELEASE
#MALLOCLIB = -Wl,-Bstatic -L$(CONTRIB_DIR)/install/lib -ldmallocthcxx
endif

UPLINKSTATICLIBS = $(STATICLIBS) $(STATIC_SDLLIBS) $(DYNAMICLIBS) $(CXXLIB) $(MALLOCLIB) $(MATHLIB) 
UPLINKDYNAMICLIBS = $(STATICLIBS) $(DYNAMIC_SDLLIBS) $(DYNAMICLIBS) $(CXXLIB) $(MALLOCLIB) $(MATHLIB) 


CXX = g++
LINK = $(CC)

ifdef RELEASE
CXXFLAGS += -O2
STRIP = strip
UPX := upx
else
CXXFLAGS += -g -D_DEBUG
#CXXFLAGS += -DSLASHER
STRIP = :
UPX = :
endif

NM = nm

CXXFLAGS += $(INCLUDES) -w -D_REENTRANT 
OBJDIR = obj/linux

# To produce the source list, use:
# find . -name '*.cpp' | sed 's%^\./\(.*\)%\1 \\%'
# and remove the last backslash (\) by hand

SOURCES1= \
app/app.cpp \
app/dos2unix.cpp \
app/miscutils.cpp \
app/opengl.cpp \
app/opengl_interface.cpp \
app/probability.cpp \
app/serialise.cpp \
app/uplinkobject.cpp \
game/data/data.cpp \
game/game.cpp \
game/gameobituary.cpp \
game/scriptlibrary.cpp \
interface/interface.cpp \
interface/interfacescreen.cpp \
interface/localinterface/analyser_interface.cpp \
interface/localinterface/cheat_interface.cpp \
interface/localinterface/email_interface.cpp \
interface/localinterface/evtqueue_interface.cpp \
interface/localinterface/finance_interface.cpp \
interface/localinterface/gateway_interface.cpp \
interface/localinterface/hud_interface.cpp \
interface/localinterface/hw_interface.cpp \
interface/localinterface/irc_interface.cpp \
interface/localinterface/keyboardinterface.cpp \
interface/localinterface/lan_interface.cpp \
interface/localinterface/localinterface.cpp \
interface/localinterface/localinterfacescreen.cpp \
interface/localinterface/memory_interface.cpp \
interface/localinterface/mission_interface.cpp \
interface/localinterface/phonedialler.cpp \
interface/localinterface/sendmail_interface.cpp \
interface/localinterface/status_interface.cpp \
interface/localinterface/sw_interface.cpp \
interface/localinterface/worldmap/worldmap_interface.cpp \
interface/localinterface/worldmap/worldmap_layout.cpp \
interface/remoteinterface/academicscreen_interface.cpp \
interface/remoteinterface/accountscreen_interface.cpp \
interface/remoteinterface/bbsscreen_interface.cpp \
interface/remoteinterface/changegatewayscreen_interface.cpp \
interface/remoteinterface/codecardscreen_interface.cpp \
interface/remoteinterface/companyinfoscreen_interface.cpp \
interface/remoteinterface/consolescreen_interface.cpp \
interface/remoteinterface/contactscreen_interface.cpp \
interface/remoteinterface/criminalscreen_interface.cpp \
interface/remoteinterface/cypherscreen_interface.cpp \
interface/remoteinterface/dialogscreen_interface.cpp \
interface/remoteinterface/disconnectedscreen_interface.cpp \
interface/remoteinterface/faithscreen_interface.cpp \
interface/remoteinterface/fileserverscreen_interface.cpp \
interface/remoteinterface/highsecurityscreen_interface.cpp \
interface/remoteinterface/hwsalesscreen_interface.cpp \
interface/remoteinterface/linksscreen_interface.cpp \
interface/remoteinterface/loansscreen_interface.cpp \
interface/remoteinterface/logscreen_interface.cpp \
interface/remoteinterface/menuscreen_interface.cpp \
interface/remoteinterface/messagescreen_interface.cpp \
interface/remoteinterface/nearestgatewayscreen_interface.cpp \
interface/remoteinterface/newsscreen_interface.cpp \
interface/remoteinterface/nuclearwarscreen_interface.cpp \
interface/remoteinterface/passwordscreen_interface.cpp \
interface/remoteinterface/protovisionscreen_interface.cpp \
interface/remoteinterface/radiotransmitterscreen_interface.cpp \
interface/remoteinterface/rankingscreen_interface.cpp \
interface/remoteinterface/recordscreen_interface.cpp \
interface/remoteinterface/remoteinterface.cpp \
interface/remoteinterface/remoteinterfacescreen.cpp \
interface/remoteinterface/securityscreen_interface.cpp \
interface/remoteinterface/shareslistscreen_interface.cpp \
interface/remoteinterface/sharesviewscreen_interface.cpp \
interface/remoteinterface/socialsecurityscreen_interface.cpp \
interface/remoteinterface/swsalesscreen_interface.cpp \
interface/remoteinterface/useridscreen_interface.cpp \
interface/remoteinterface/voiceanalysisscreen_interface.cpp \
interface/remoteinterface/voicephonescreen_interface.cpp \
interface/scrollbox.cpp \
interface/taskmanager/decrypter.cpp \
interface/taskmanager/decypher.cpp \
interface/taskmanager/defrag.cpp \
interface/taskmanager/dictionaryhacker.cpp \
interface/taskmanager/faith.cpp \
interface/taskmanager/filecopier.cpp \
interface/taskmanager/filedeleter.cpp \
interface/taskmanager/firewalldisable.cpp \
interface/taskmanager/gatewaynuke.cpp \
interface/taskmanager/iplookup.cpp \
interface/taskmanager/ipprobe.cpp \
interface/taskmanager/lanforce.cpp \
interface/taskmanager/lanprobe.cpp \
interface/taskmanager/lanscan.cpp \
interface/taskmanager/lanspoof.cpp \
interface/taskmanager/logdeleter.cpp \
interface/taskmanager/logmodifier.cpp \
interface/taskmanager/logundeleter.cpp \
interface/taskmanager/motionsensor.cpp \
interface/taskmanager/passwordbreaker.cpp \
interface/taskmanager/proxydisable.cpp \
interface/taskmanager/revelation.cpp \
interface/taskmanager/revelationtracker.cpp \
interface/taskmanager/securitybypass.cpp \
interface/taskmanager/taskmanager.cpp \
interface/taskmanager/tracetracker.cpp \
interface/taskmanager/tutorial.cpp \
interface/taskmanager/uplinkagentlist.cpp \
interface/taskmanager/uplinktask.cpp \
interface/taskmanager/voiceanalyser.cpp \
mainmenu/closing_interface.cpp \
mainmenu/connectionlost_interface.cpp \
mainmenu/demogameover_interface.cpp \
mainmenu/disavowed_interface.cpp \
mainmenu/firsttimeloading_interface.cpp \
mainmenu/genericoptions_interface.cpp \
mainmenu/loading_interface.cpp \
mainmenu/login_interface.cpp \
mainmenu/mainmenu.cpp \
mainmenu/mainmenuscreen.cpp \
mainmenu/graphicoptions_interface.cpp \
mainmenu/networkoptions_interface.cpp \
mainmenu/obituary_interface.cpp \
mainmenu/options_interface.cpp \
mainmenu/revelationlost_interface.cpp \
mainmenu/revelationwon_interface.cpp \
mainmenu/theme_interface.cpp \
mainmenu/theteam_interface.cpp \
mainmenu/warezgameover_interface.cpp \
network/clientconnection.cpp \
network/interfaces/clientcommsinterface.cpp \
network/interfaces/clientstatusinterface.cpp \
network/interfaces/networkscreen.cpp \
network/network.cpp \
network/networkclient.cpp \
network/networkserver.cpp \
options/options.cpp \
uplink.cpp \
view/fps.cpp \
view/view.cpp \
world/agent.cpp \
world/company/company.cpp \
world/company/companyuplink.cpp \
world/company/mission.cpp \
world/company/news.cpp \
world/company/sale.cpp \
world/computer/bankaccount.cpp \
world/computer/bankcomputer.cpp \
world/computer/computer.cpp \
world/computer/computerscreen/bbsscreen.cpp \
world/computer/computerscreen/computerscreen.cpp \
world/computer/computerscreen/cypherscreen.cpp \
world/computer/computerscreen/dialogscreen.cpp \
world/computer/computerscreen/disconnectedscreen.cpp \
world/computer/computerscreen/genericscreen.cpp \
world/computer/computerscreen/highsecurityscreen.cpp \
world/computer/computerscreen/linksscreen.cpp \
world/computer/computerscreen/logscreen.cpp \
world/computer/computerscreen/menuscreen.cpp \
world/computer/computerscreen/messagescreen.cpp \
world/computer/computerscreen/passwordscreen.cpp \
world/computer/computerscreen/shareslistscreen.cpp \
world/computer/computerscreen/useridscreen.cpp \
world/computer/databank.cpp \
world/computer/gateway.cpp \
world/computer/gatewaydef.cpp \
world/computer/lancomputer.cpp \
world/computer/lanmonitor.cpp \
world/computer/logbank.cpp \
world/computer/recordbank.cpp \
world/computer/security.cpp \
world/computer/securitymonitor.cpp \
world/computer/securitysystem.cpp \
world/connection.cpp \
world/date.cpp \
world/generator/consequencegenerator-sc.cpp \
world/generator/demoplotgenerator-sc.cpp \
world/generator/langenerator-sc.cpp \
world/generator/missiongenerator-sc.cpp \
world/generator/namegenerator-sc.cpp \
world/generator/newsgenerator-sc.cpp \
world/generator/numbergenerator-sc.cpp \
world/generator/plotgenerator-sc.cpp \
world/generator/recordgenerator-sc.cpp \
world/generator/worldgenerator-sc.cpp \
world/message.cpp \
world/person.cpp \
world/player.cpp \
world/rating.cpp \
world/scheduler/arrestevent.cpp \
world/scheduler/attemptmissionevent.cpp \
world/scheduler/bankrobberyevent.cpp \
world/scheduler/changegatewayevent.cpp \
world/scheduler/eventscheduler.cpp \
world/scheduler/installhardwareevent.cpp \
world/scheduler/notificationevent.cpp \
world/scheduler/runplotsceneevent.cpp \
world/scheduler/seizegatewayevent.cpp \
world/scheduler/shotbyfedsevent.cpp \
world/scheduler/uplinkevent.cpp \
world/scheduler/warningevent.cpp \
world/vlocation.cpp \
world/world.cpp

SOURCES2 = app/decode.c

SOURCES = $(SOURCES1) $(SOURCES2)

ENCSRC = $(filter %-sc.cpp, $(SOURCES))

OBJECTS = $(SOURCES1:%.cpp=$(OBJDIR)/%.o) $(SOURCES2:%.c=$(OBJDIR)/%.o)

all: uplink.static 

uplink.static: $(OBJECTS) $(INTERNALLIBS)
	@echo -n "Linking... "
	@$(LIBTOOL) --quiet $(LINK) $+ $(UPLINKSTATICLIBS) -o uplink.static
	@$(NM) uplink.static > uplink.static-linux.map
	@$(STRIP) uplink.static
	@echo done.

uplink.dynamic: $(OBJECTS) $(INTERNALLIBS)
	@echo -n "Linking... "
	@$(LIBTOOL) --quiet $(LINK) $+ $(UPLINKDYNAMICLIBS) -o uplink.dynamic
	@$(NM) uplink.dynamic > uplink.dynamic-linux.map
	@$(STRIP) uplink.dynamic
	@echo done.

dist: uplink.static uplink.dynamic
	@echo -n "Compressing..."
	@-$(UPX) uplink.dynamic 2> /dev/null > /dev/null
	@-$(UPX) uplink.static 2> /dev/null > /dev/null
	@echo "done"
	cp uplink.static ../dist/static/uplink
	cp ../changes.txt ../dist/static
	cp ../readme.txt ../dist/static
	cp ../testenv/patch.dat ../dist/static
	cp uplink.dynamic ../dist/dynamic/uplink-bin
	cp ../changes.txt ../dist/dynamic
	cp ../readme.txt ../dist/dynamic
	cp ../testenv/patch.dat ../dist/dynamic
	tar -C ../dist/static -c -z -f ../linuxpatch-static-$(VERSION).tar.gz .
	tar -C ../dist/dynamic -c -z -f ../linuxpatch-dynamic-$(VERSION).tar.gz .

FIXSAVEGAME_OBJECTS = fixsavegame.o $(filter-out obj/linux/uplink.o, $(OBJECTS))

fixsavegame: $(FIXSAVEGAME_OBJECTS)
	$(CXX) -o $@ $+ $(UPLINKSTATICLIBS)

encsrc: $(ENCSRC)
	ls -l $(ENCSRC)

objs: $(OBJECTS)

clean:
	rm -rf $(OBJDIR)/*

$(OBJDIR)/%.o: %.cpp
	@test -d $(dir $(OBJDIR)/$<) || mkdir -p $(dir $(OBJDIR)/$<)
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@test -d $(dir $(OBJDIR)/$<) || mkdir -p $(dir $(OBJDIR)/$<)
	@echo Compiling $<
	@$(CC) $(CXXFLAGS) -c $< -o $@

%-sc.cpp: %.cpp process.pl
	@echo Encryping text strings in $<
	@perl process.pl $<

include auto-depend.mk

