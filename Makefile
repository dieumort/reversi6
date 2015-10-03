#------------------------------------------------------------------------------
#  Makefile for C++
#------------------------------------------------------------------------------
# basic settings
TARGET := prg
OBJDIR := ./obj
TAGNAME := .tags
#------------------------------------------------------------------------------
# C++ compiler settings
COMPILER := g++
LIBS :=
INCS :=
FLAGS := -O3 --std=c++11 -Wall -Wextra -Wno-unused-parameter -Wfloat-equal
OFLAGS := -MMD -MP
#------------------------------------------------------------------------------
# extensions
SRCEXT := cpp
OBJEXT := o
DEPEXT := d
#------------------------------------------------------------------------------
# files & directories
SRCS := $(shell find . -type f -name "*.$(SRCEXT)" | sed -e 's/^\.\///g')
OBJS := $(addprefix $(OBJDIR)/, $(SRCS:%.$(SRCEXT)=%.$(OBJEXT)))
DEPS := $(OBJS:%.$(OBJEXT)=%.$(DEPEXT))
#------------------------------------------------------------------------------
# all
.PHONY: all
all: $(TARGET)
# debug
.PHONY: debug
debug: FLAGS += -g
debug: all
# ctags
.PHONY: ctags
ctags:
	ctags -R -f $(TAGNAME)
# clean
.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJDIR) $(TAGNAME)
#------------------------------------------------------------------------------
# target
$(TARGET): $(OBJS)
	$(COMPILER) $(FLAGS) $^ $(LIBS) -o $(TARGET)
# object files, dependancy files
$(OBJDIR)/%.$(OBJEXT): %.$(SRCEXT)
	@(mkdir -p `dirname $@`)
	$(COMPILER) $(FLAGS) $(OFLAGS) -c $< $(INCS) -o $@
#------------------------------------------------------------------------------
# include dependancy files
-include $(DEPS)
#------------------------------------------------------------------------------
