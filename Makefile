# Makefile for jkcompiler

# Tools
CC := gcc
LEX := flex
YACC := bison
DIFF := diff

# Directories
SRCDIR := src
BINDIR := bin
BUILDDIR := build
TESTSDIR := tests
TARGET := opc

# GCC Debug Flags
DEBUGFLAGS := -g3 -O0 -Wall -pg
RTCHECKS := -fmudflap -fstack-check -gnato
# GCC Production Flags
PRODFLAGS := -Wall -O2
# Active Flags
CFLAGS := -std=gnu99 $(DEBUGFLAGS)
LINK := $(DEBUGFLAGS)

# Bison Flags (-d = defines, -y = yacc behavior, -v = verbose)
YACCFLAGS := -d -y -v

# File paths
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Targets

all: yacc lex $(OBJECTS) tests_prepare
	@echo " Linking..."; $(CC) $(LINK) $(OBJECTS) -o $(BINDIR)/$(TARGET)
 
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -c -o $@ $<

lex:
	@cd $(SRCDIR); $(LEX) pascal.l

yacc:
	(cd $(SRCDIR); $(YACC) $(YACCFLAGS) pascal.y)
	echo "#include \"shared.h\"" >> $(SRCDIR)/y.tab.h.tmp
	cat $(SRCDIR)/y.tab.h >> $(SRCDIR)/y.tab.h.tmp
	rm $(SRCDIR)/y.tab.h
	mv $(SRCDIR)/y.tab.h.tmp $(SRCDIR)/y.tab.h
	
clean:
	echo " Cleaning...";
	rm -r $(BUILDDIR) $(BINDIR)/$(TARGET)*
	cd $(SRCDIR); rm -f y.tab.h.tmp y.output
	rm -f $(TESTSDIR)/tests_lib.inc
	rm -f $(TESTSDIR)/tests_semantic/results/$(TARGET).{log,sum}
	rm -f $(TESTSDIR)/tests_semantic/*.output
	rm -f $(TESTSDIR)/tests_semantic/out.c
	rm -f $(TESTSDIR)/tests_semantic/a.out

tests: tests_semantic tests_opt tests_gen

tests_semantic: tests_prepare
	# "runtest" comes from DejaGNU
	mkdir -p $(TESTSDIR)/tests_semantic/results
	(cd $(TESTSDIR)/tests_semantic; runtest --all --output results --tool $(TARGET) tests_semantic.exp)

tests_prepare:
	@echo "BINARY=../../$(BINDIR)/$(TARGET)" > $(TESTSDIR)/tests_lib.inc

.PHONY: all lex yacc clean tests


