## Makefile for ROOT stuff

FLAGS=-g -Wall -O3

MY_INCS=-I/afs/phas.gla.ac.uk/user/k/knordstrom/rivet/local/include -I/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/include
MY_LIBS=

THIS_LIB=-L/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/lib -lTTBB

ROOT_INCS=$(shell root-config --cflags)
ROOT_LIBS=$(shell root-config --libs)

matchjets: ./src/matchjets.cxx
	g++ $(FLAGS) -o matchjets ./src/matchjets.cxx $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

matchleptons: ./src/matchleptons.cxx
	g++ $(FLAGS) -o matchleptons ./src/matchleptons.cxx $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

dumpjets: ./src/dumpjets.cxx
	g++ $(FLAGS) -o dumpjets ./src/dumpjets.cxx $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

library: ./src/library.cc ./include/library.h
	g++ $(FLAGS) -c -fPIC ./src/library.cc -o library.o $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)
	g++ -shared -Wl -o ./lib/libTTBB.so library.o -lc $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)
	rm library.o
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/lib
