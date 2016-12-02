include Makefile.cfg

ifndef BINLOC

.DEFAULT_GOAL:=default

$(builds): export BINLOC = $(binarydir)/$($@dir)/$(name)
$(builds): export FLAGS  = $($@flags)
$(builds): export objdir = $(objectdir)/$($@dir)
$(builds): export depdir = $(dependdir)/$($@dir)
$(builds): export bindir = $(binarydir)/$($@dir)
$(builds):
	@$(MAKE)

else

ifeq ($(OS), Windows_NT)
	FLAGS += -DWIN32

	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
#64bit
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
#64bit
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
#32bit
		endif
	endif
else
	UNAME_S:=$(shell uname -s)
	UNAME_P:=$(shell uname -p)

	ifeq ($(UNAME_S),Linux)
#linux
	endif
	ifeq ($(UNAME_S),Darwin)
#OSX
	endif

	ifeq ($(UNAME_S),x86_64)
#64bit
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
#32bit
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
#arm
	endif
endif

.DEFAULT_GOAL:=$(BINLOC)

SOURCES:=$(shell find $(sourcedir) -name '*$(sourceextension)')
OBJECTS:=$(subst $(sourcedir),$(objdir), $(subst $(sourceextension),.o, $(SOURCES)))
DEPENDS:=$(subst $(sourcedir),$(depdir), $(subst $(sourceextension),.d, $(SOURCES)))
DIRS := $(sourcedir) $(headerdir) $(objectdir) $(objdir) $(dependdir) $(depdir) $(binarydir) $(bindir)

.SECONDARY:$(OBJECTS)

# link everything together
%/$(name):$(OBJECTS)
	$(CC) $+ $(FLAGS) -lglfw -o $@

-include $(DEPENDS)

# generate directories
$(DIRS):
	@echo Making directory: $@
	@mkdir $@

# more complicated dependency computation, so all prereqs listed
# will also become command-less, prereq-less targets
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
$(objdir)/%.o:|$(DIRS)
	$(CC) -c $(sourcedir)/$*$(sourceextension) $(FLAGS) -o $@
	$(CC) -MM $(FLAGS) $(sourcedir)/$*$(sourceextension) > $(depdir)/$*.d
	@mv -f $(depdir)/$*.d $(depdir)/$*.d.tmp
	@sed -e 's|.*:|$(objdir)/$*.o:|' < $(depdir)/$*.d.tmp > $(depdir)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(depdir)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(depdir)/$*.d
	@$(RM) -f $(depdir)/$*.d.tmp

endif

clean:
	-@$(RM) -rf $(objectdir) *.exe* $(dependdir) $(binarydir) $(name)

#file partially written based on information from:
#http://scottmcpeak.com/autodepend/autodepend.html

.PHONY:clean $(DEPENDS) Makefile Makefile.cfg $(builds)

