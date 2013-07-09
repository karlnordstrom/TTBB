#! /bin/bash

for name in $(ls *.1); do
	mv $name `echo $name | sed -e 's/\(.1\)*$//g'`
done

for name in $(ls *.2); do
	mv $name `echo $name | sed -e 's/\(.2\)*$//g'`
done
exit
