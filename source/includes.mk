LIBDIR = $(TOP)../lib
LIBS = tosser gucci soundgarden vanbakel eclipse tcp4u
OTHER = .

INCLUDES += $(LIBS:%=-I$(LIBDIR)/%) $(OTHER:%=-I%)
CXXFLAGS += $(INCLUDES)
