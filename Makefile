## Makefile for ROOT stuff
## Suppressing output since the library links are massive

FLAGS=-g -Wall -O3

MY_INCS=-I/afs/phas.gla.ac.uk/user/k/knordstrom/rivet/local/include -I/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/include
MY_LIBS=

THIS_LIB=-L/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/lib -lTTBB

ROOT_INCS=$(shell root-config --cflags)
ROOT_LIBS=$(shell root-config --libs)

matchjets: ./src/matchjets.cxx library
	@echo "Building matchjets..."
	@g++ $(FLAGS) -o matchjets ./src/matchjets.cxx $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

matchleptons: ./src/matchleptons.cxx library
	@echo "Building matchleptons..."
	@g++ $(FLAGS) -o matchleptons ./src/matchleptons.cxx $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

dumpjets: ./src/dumpjets.cxx library
	@echo "Building dumpjets..."
	@g++ $(FLAGS) -o dumpjets ./src/dumpjets.cxx $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

library: ./src/library.cc ./include/library.h
	@echo "Building library..."
	@g++ $(FLAGS) -c -fPIC ./src/library.cc -o library.o $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)
	@echo "Linking library..."
	@g++ -shared -Wl -o ./lib/libTTBB.so library.o -lc $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)
	@rm library.o
	@export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/lib
	@echo "All done!"

clean:
	@rm matchjets || :
	@rm matchleptons || :
	@rm dumpjets || :
	@rm ./lib/libTTBB.so || :
	@echo "Removed binaries and library."
