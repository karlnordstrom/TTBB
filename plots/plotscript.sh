#!/bin/bash

## Use: Plots all the .plot files in the directory with the given instructions (arg1+arg2).
## Example: ./plotscript.sh RT pt
## This will plot all quantities for R+T3 for all .plot files, put them in the specified directory, and move all the generated .dat files into another specified one.

rivet=rivet
root=root

if [ "$3" = "$rivet" ]; then
##Directory for plots:
plots=rivet_pdf
##Directory for .dat files:
dats=rivet_dat
##Directory for raw files:
raws=rivet_raw
else
##Directory for plots:
plots=pdf
##Directory for .dat files:
dats=dat
##Directory for raw files:
raws=raw
fi

if [ ! -d "$plots" ]; then
    mkdir $plots
fi

if [ ! -d "$dats" ]; then
    mkdir $dats
fi

##Check raw directory exists, exit if not
if [ ! -d "$raws" ]; then
	echo "Raw directory does not exist or is not a directory!"
	exit
fi

##Make sure plotting instructions make sense.
if [ "$1" == "RT"  ]||[ "$1" == "R"  ]||[ "$1" == "T1"  ]||[ "$1" == "T2"  ]||[ "$1" == "T3"  ]; then
	echo "Plotting instructions accepted."
else
	echo "Plotting instructions don't make sense. Look at instructions in plotter source."
	exit
fi

echo "Creating and compiling latex..."

for f in $(ls $raws)
do
        plotter $raws/$f $1 $2 > $dats/${f}${2}.dat && make-plots $dats/${f}${2}.dat && mv $dats/${f}${2}.pdf $plots
done

exit

