ROOT_INCS=$(shell root-config --cflags)
ROOT_LIBS=$(shell root-config --libs)
FLAGS=-g -Wall -O3


all: plotter.c
	@echo "Building plotter..."
	@g++ $(FLAGS) -o plotter plotter.c  $(ROOT_INCS) $(ROOT_LIBS)
