## Makefile for ROOT stuff
## Since I'm only using boost I'm okay with only an
## include, if you want something more fancy you'll
## need to define MY_LIBS

MY_INCS=-I/afs/phas.gla.ac.uk/user/k/knordstrom/rivet/local/include
MY_LIBS=

ROOT_INCS=$(shell root-config --cflags)
ROOT_LIBS=$(shell root-config --libs)

matchjets: matchjets.cxx
	g++ -o matchjets matchjets.cxx $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)

matchleptons: matchleptons.cxx
	g++ -o matchleptons matchleptons.cxx $(MY_INCS) $(MY_LIBS) $(ROOT_INCS) $(ROOT_LIBS)
