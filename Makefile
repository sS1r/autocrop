CC=g++
LFLAGS=-lboost_program_options -lpng -ljpeg -ltiff

ODIR=obj
SDIR=src
TDIR=test
EXECUTABLE=autocrop
MAKEDIR=mkdir -p $(ODIR)

# Modify some variables if on windows
ifeq ($(OS), Windows_NT)
	ODIR=obj-win
	EXECUTABLE=autocrop.exe
	MAKEDIR=mkdir $(ODIR) 2> nul
	LFLAGS += -Llib
endif


.PHONY: all clean

# ##############################################
# Main build with pre build step, very useful! #
# ##############################################

all: main-build

main-build: pre-build
	@$(MAKE) --no-print-directory -C $(SDIR) src
	@$(MAKE) --no-print-directory $(EXECUTABLE)
# @$(MAKE) --no-print-directory -C $(TDIR) test

pre-build:
	-$(MAKEDIR)
	
# #############################################

$(EXECUTABLE): $(wildcard $(ODIR)/*.o)
	$(CC) $^ -o $@ -Wall $(LFLAGS)

# #############################################

clean:
ifeq ($(OS), Windows_NT)
	-del /Q $(ODIR)\*
	-rmdir $(ODIR)
	-del $(EXECUTABLE)
else
	rm -f $(ODIR)/*
	rm -d $(ODIR)
	rm $(EXECUTABLE)
endif