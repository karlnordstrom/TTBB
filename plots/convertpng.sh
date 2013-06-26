#!/bin/bash

rivet=rivet
root=root

if [ "$1" = "$rivet" ]; then
##Directory for plots:
plots=rivet_pdf
##Directory for .dat files:
pngs=rivet_png
else
##Directory for plots:
plots=pdf
##Directory for .dat files:
pngs=png
fi

if [ ! -d "$pngs" ]; then
    mkdir $pngs
fi

if [ ! -d "$plots" ]; then
    mkdir $plots
fi

echo "Converting pdfs to pngs..."

cd $plots
for f in $(ls *.pdf)
do
    convert -density 200x200 pdf:$f ../$pngs/${f%.pdf}.png
done

exit
