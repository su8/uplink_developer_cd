# auto-dependency information
# define SOURCES

DEPENDDIR = .depend

$(DEPENDDIR)/%.d: %.cpp
	@echo make-depend $<
	@mkdir -p $(dir $@) 2> /dev/null
	@$(CC) -MM $(CXXFLAGS) $< | \
	( IFS=':' ; read target deps ; \
          echo $(OBJDIR)/$(<:.cpp=.o) $@\:$$deps ) > $@

$(DEPENDDIR)/%.d: %.c
	@echo make-depend $<
	@mkdir -p $(dir $@) 2> /dev/null
	@$(CC) -MM $(CXXFLAGS) $< | \
	( IFS=':' ; read target deps ; \
          echo $(OBJDIR)/$(<:.c=.o) $@\:$$deps ) > $@

DEP1 = $(SOURCES:%.cpp=$(DEPENDDIR)/%.d)
DEP2 = $(DEP1:%.c=$(DEPENDDIR)/%.d)
ALLDEPS = $(DEP2)

include $(ALLDEPS)
