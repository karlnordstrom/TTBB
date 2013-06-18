#!/bin/bash

## Use: Plots all the .plot files in the directory with the given instructions (arg1).
## Example: ./plotscript.sh RT
## This will plot all quantities for R+T3 for all .plot files, put them in the specified directory, and move all the generated .dat files into another specified one.

##Directory for plots:
plots=pdf
##Directory for .dat files:
dats=dat
##Directory for raw files:
raws=raw

##Check directories exist, exit if they don't.
if [ ! -d "$plots" ]; then
	echo "Plot directory does not exist or is not a directory!"
	exit
fi
if [ ! -d "$dats" ]; then
	echo ".dat directory does not exist or is not a directory!"
	exit
fi
if [ ! -d "$raws" ]; then
	echo "raw directory does not exist or is not a directory!"
	exit
fi

##Make sure plotting instructions make sense.
if [ "$1" == "RT"  ]||[ "$1" == "R"  ]||[ "$1" == "T1"  ]||[ "$1" == "T2"  ]||[ "$1" == "T3"  ]; then
	echo "Plotting."
else
	echo "Plotting instructions don't make sense. Look at instructions in plotter source."
	exit
fi

cd $raws
for f in $(ls)
do
        plotter $f $1 pt > ${f}pt.dat && make-plots ${f}pt.dat && mv ${f}pt.dat ../$dats && mv ${f}pt.pdf ../$plots
        #plotter $f $1 p > ${f}p.dat && make-plots ${f}p.dat && mv ${f}p.dat ../$dats && mv ${f}p.pdf ../$plots
        #plotter $f $1 e > ${f}e.dat && make-plots ${f}e.dat && mv ${f}e.dat ../$dats && mv ${f}e.pdf ../$plots
        #plotter $f $1 gamma > ${f}gamma.dat && make-plots ${f}gamma.dat && mv ${f}gamma.dat ../$dats && mv ${f}gamma.pdf ../$plots
        #plotter $f $1 lngamma > ${f}lngamma.dat && make-plots ${f}lngamma.dat && mv ${f}lngamma.dat ../$dats && mv ${f}lngamma.pdf ../$plots
        #plotter $f $1 b > ${f}b.dat && make-plots ${f}b.dat && mv ${f}b.dat ../$dats && mv ${f}b.pdf ../$plots
done

exit

