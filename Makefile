CC=g++
LFLAGS=-lboost_program_options -lpng -ljpeg -ltiff

ODIR=obj
SDIR=src
TDIR=test
EXECUTABLE=autocrop

.PHONY: all clean

# ##############################################
# Main build with pre build step, very useful! #
# ##############################################

all: main-build

main-build: pre-build
	@$(MAKE) --no-print-directory -C $(SDIR) src
	# @$(MAKE) --no-print-directory -C $(TDIR) test
	@$(MAKE) --no-print-directory $(EXECUTABLE)
	
pre-build:
	mkdir -p $(ODIR)

# #############################################

$(EXECUTABLE): $(wildcard $(ODIR)/*.o)
	$(CC) $^ -o $@ -Wall $(LFLAGS)

# #############################################

clean:
	rm -f $(ODIR)/*
	rm -d $(ODIR)
	rm $(EXECUTABLE)
