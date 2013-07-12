Comparing truth and reconstructed data
===========================

All code is written for the particular setup I had when doing the project:

* Your truth data is in a pool format
* Your reconstructed data is in a mini ntuple
* You have access to Athena / know how to do an asetup
* I (usually) used 17.2.2

===========================

Boost libraries
---------------

The boost libraries have a bunch of very useful stuff, in particular I've used
BOOST_FOREACH which allows you to use python-esque loops in C++, and boost::any
which allows some void pointer-like operations with less danger. Since the library
is (almost completely) implemented as headers, you only need to include the path
to your boost headers (-I/path/to/my/headers) and don't have to deal with linking.

===========================

The makefile
------------

As explained above you need to specify the path to your boost headers in the makefile.
You also need to set MY_LD_LIBRARY_PATH to a location that is in your LD_LIBRARY_PATH
(or just change LD_LIBRARY_PATH to include $(pwd)/lib).

===========================

Data files
----------

All the data file names are hardcoded into the .cc files in the src directory -- you
will have to change the names if you plan to use your own files. Same goes for output
file name.

To convert the truth pool files into a root ntuple use convert.sh in ./data/, and
fixname.sh to remove the .1 and .2s. You have to do an asetup for this to work --
17.2.2 is again what I used.

===========================

Creating HTML web page
----------------------

For this step, you need to install my event `plotter` in a location in PATH. The source is
in ./plotter, and can be built using the makefile (as long as you have asetup'd correctly).
Then just copy the executable into a location that is in your $PATH environment variable.
You also need access to a Rivet install that you can access from the shell -- if you don't have
that, you can still use the plotter by putting the ./plotter/make-plots script in your $PATH
together with the `plotter` executable.

To create the webpage, you first have to run matchjets and createjetplot to generate
the raw data. First make sure that ./plots/raw/ and ./plots/stats/ are both empty (just
delete whatever is in there at the moment, I know this is a bit ugly but I don't have the time
to make it nice and user-friendly at the moment).

Then you should do the following:

MY_RUN=myrun // Set to whatever you want to call this run

`./matchjets ./plots/stats/$MY_RUN`

`./createjetplot ./plots/raw/$MY_RUN`

You can now create the webpage by:

`cd plots`
`make root`

The webpage will be placed in the .plots/html/ folder, and can be viewed by using

`firefox index.html`

when in the folder.

==========================

Installing the `plotter` tool
-----------------------------

The `plotter` is a simple event visualiser written in C++ that converts a basic text format into \latex,
which is then compiled using Rivet's make-plots python script. The reason is that I was too lazy to
write the \latex stuff from scratch and kind of piggybacked on make-plots to provide the basic canvas and
coordinate system. If you use the scripts in ./plots/ it should just *work*, but in case (when) it doesn't:

You need to have your raw data file in the correct format. Currently the best way to check the format is to
open the source file in a text editor... There are instructions at the beginning of it. Sorry.

Once you have the data file, run the plotter on it, specifying what you want to plot. Usually you want to give
RT as the first argument (this means you are comparing truth to reconstructed). The alternatives are T3 (only
plot truth) and R (only plot reconstructed). The second argument is which quantity you want to weight the marker
sizes by: a lot of stuff is supported, but the two most common ones to use are probably pt and constant. So an example
call would be:

`plotter raw_data RT constant`

Now this will print the \latex file to your standard output, so you have to redirect it into a file of your choosing:

`plotter raw_data RT constant > plot.dat`

Now you can compile plot.dat by using the make-plots script:

`make-plots plot.dat`

which compiles the \latex document and saves it as plot.pdf.
