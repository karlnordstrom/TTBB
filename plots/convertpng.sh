#!/bin/bash

##Directory for plots:
plots=pdf
##Directory for .dat files:
pngs=png

cd $plots

for f in $(ls *.pdf)
do
    convert -density 200x200 pdf:$f ../png/${f%.pdf}.png
done

exit
