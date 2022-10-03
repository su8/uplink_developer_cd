LIBS=tosser gucci eclipse bungle redshirt slasher
CONTRIBDIR = ../../../contrib
INSTALLDIR = $(CONTRIBDIR)/install
INCLUDES += $(LIBS:%=-I../%) 
INCLUDES += -I$(INSTALLDIR)/include 
INCLUDES += -I$(INSTALLDIR)/include/gltt

#INCLUDES += -I$(CONTRIBDIR)/install/include/freetype2
#INCLUDES += -I$(CONTRIBDIR)/install/include/freetype2/freetype
INCLUDES += -I$(CONTRIBDIR)/ftgl_dist/src
CXXFLAGS += $(INCLUDES)
#CXXFLAGS += -g
