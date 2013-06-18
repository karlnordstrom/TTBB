## Makefile for ROOT stuff
## Suppressing output since the library links are massive

FLAGS=-g -Wall -O3

MY_INCS=-I/afs/phas.gla.ac.uk/user/k/knordstrom/rivet/local/include -I/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/include
MY_LIBS=

THIS_LIB=-L/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/lib -lTTBB

ROOT_INCS=$(shell root-config --cflags)
ROOT_LIBS=$(shell root-config --libs)

all: matchjets matchleptons dumpjets

matchjets: ./src/matchjets.cc library
	@echo "Building matchjets..."
	@g++ $(FLAGS) -o matchjets ./src/matchjets.cc $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

matchleptons: ./src/matchleptons.cc library
	@echo "Building matchleptons..."
	@g++ $(FLAGS) -o matchleptons ./src/matchleptons.cc $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

dumpjets: ./src/dumpjets.cc library
	@echo "Building dumpjets..."
	@g++ $(FLAGS) -o dumpjets ./src/dumpjets.cc $(MY_INCS) $(MY_LIBS) $(THIS_LIB) $(ROOT_INCS) $(ROOT_LIBS)

library: Utils.o FourMomentum.o
	@echo "Linking library..."
	@g++ -shared -Wl -o ./lib/libTTBB.so library.o FourMomentum.o -lc $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)
	@rm library.o
	@rm FourMomentum.o
	@export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):/afs/phas.gla.ac.uk/user/k/knordstrom/code/TTBB/lib

FourMomentum.o: ./src/FourMomentum.cc ./include/FourMomentum.hh
	@echo "Building FourMomentum..."
	@g++ $(FLAGS) -c -fPIC ./src/FourMomentum.cc -o FourMomentum.o $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)

Utils.o: ./src/Utils.cc ./include/Utils.hh
	@echo "Building Utils..."
	@g++ $(FLAGS) -c -fPIC ./src/Utils.cc -o library.o $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)

clean:
	@rm matchjets || :
	@rm matchleptons || :
	@rm dumpjets || :
	@rm ./lib/libTTBB.so || :
	@echo "Removed binaries and library."
