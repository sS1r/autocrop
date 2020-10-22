CC=g++
CFLAGS=-c -g -std=c++11 -Wall -m32
ODIR=../obj

ifeq ($(OS),Windows_NT)
	CC=mingw32-g++
	ODIR=../obj-win
	CFLAGS += -I..\.github\include
endif

all:

src: $(ODIR)/autocrop.o $(ODIR)/parse_options.o $(ODIR)/main.o $(ODIR)/run.o

$(ODIR)/autocrop.o: autocrop.cpp autocrop.h
	$(CC) $(CFLAGS) $< -o $@

$(ODIR)/parse_options.o: parse_options.cpp parse_options.h
	$(CC) $(CFLAGS) $< -o $@

$(ODIR)/run.o: run.cpp run.h
	$(CC) $(CFLAGS) $< -o $@

$(ODIR)/main.o: main.cpp
	$(CC) $(CFLAGS) $< -o $@

