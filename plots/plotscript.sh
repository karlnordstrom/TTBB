#!/bin/bash

## Use: Plots all the .plot files in the directory with the given instructions (arg1+arg2).
## Example: ./plotscript.sh RT pt
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
        plotter $f $1 $2 > ${f}${2}.dat && make-plots ${f}${2}.dat && mv ${f}${2}.dat ../$dats && mv ${f}${2}.pdf ../$plots
done

exit

