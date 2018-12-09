#******************************************************************************
#
# File:   makefile
#
# Purpose:
# 	Makefile
#
# Revisions
#
#****************************************************************************/
 
include project.make
# expected are following variables: PROJECT, DEFINES, SOURCES, CFLAGS, CC

OBJDIR := obj
OBJECTS := $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

CFLAGS += $(addprefix -I,$(INCLUDES)) $(addprefix -D,$(DEFINES))
CFLAGS += -g

LDLIBPATH += $(addprefix -L,$(LIBPATH))
LDLIBS += $(addprefix -l,$(LIBS))

ifeq ($(DEBUGMAKE),y)
export silent :=
else
export silent :=@
endif

.PHONY: all

all: check_project_name $(OBJDIR) link

ifeq ($(PROJECT),)
check_project_name:
	$(silent)echo 'Please, define a project name in the makefile (variable PROJECT)!'
	$(silent)exit 1
else
check_project_name:
endif

link: $(OBJECTS)
	$(silent)echo   linking $(CC) $(OBJECTS) $(LDLIBPATH) $(LDLIBS) -o $(OBJDIR)/$(PROJECT)
	$(silent)$(CC) $(OBJECTS) $(LDLIBPATH) $(LDLIBS) -o $(OBJDIR)/$(PROJECT)

# 	$(silent)$(CC) $(LDLIBPATH) $(OBJECTS) -o $(OBJDIR)/$(PROJECT).exe $(LDLIBS)
$(OBJDIR):
	$(silent)mkdir $(OBJDIR)

clean:
	@echo removing $(OBJDIR)
	$(silent)rm -Rf $(OBJDIR)
#	@echo removing $(OBJDIR)\*.*
#	$(silent)@del /Q $(OBJDIR)\*.*

################################################################################
# create rules for compilations and dependencies creation for each source file
# in the following obj is an example of the $(OBJDIR)'s value
# function create_rules:
#   first argument $1 is the object file (e.g. obj/main.o)
#   sedond argument $2 is the source file
# creates a rule for compilation of each source file <some_location>/src.c to
# obj/src.o requires a dependency file: obj/src.o.d
# creates a rule for generating a dependency file obj/src.o.d out of the source
# file
define create_rules
# compile to object file and create dependency file: requires source file
$1: $2
	$(silent)echo compiling $2
	$(silent)$(CC) $(CFLAGS) $2 -o $$@
	$(silent)$(CC) -MM -MT $$@ $(CFLAGS) $2 > $$@.d
endef

# call rules for each source file
# $(SOURCES) contains a list of source files in various locations; the next
# statement calls create_rules for each source file and passes as parameters the
# required object file (which is $(OBJDIR)/src.o) and the original source file
# (which is <some_location>/src.c)
$(foreach src,$(SOURCES),$(eval $(call create_rules,$(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(src))),$(src))))

# pull in dependency info for *existing* .o files
-include $(OBJECTS:%.o=%.o.d)
################################################################################
