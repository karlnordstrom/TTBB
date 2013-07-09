Comparing truth and reconstructed data
===========================

All code is written for the particular setup I had when doing the project:

* Your truth data is in a pool format
* Your reconstructed data is in a mini ntuple
* You have access to Athena / know how to do an asetup
* I (usually) used 17.2.2
* You can use the boost libraries (1)

===========================

Boost libraries
---------------

(1)

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
(or just change LD_LIBRARY_PATH to include `pwd`/lib).

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

For this step, you need to install my event plotter in a location in PATH. The source is
in ./plotter, and can be built using the makefile (as long as you have asetup'd correctly).
Then just copy the executable into a location that is in your $PATH environment variable.

To create the webpage, you first have to run matchjets and createjetplot to generate
the raw data. First make sure that ./plots/raw/ and ./plots/stats/ are both empty (just
delete whatever is in there at the moment, I know this is a bit ugly but I don't have the time
to make it nice and user-friendly at the moment).

Then you should do the following:

MY_RUN=myrun // Set to whatever you want to call this run

./matchjets ./plots/stats/$MY_RUN

./createjetplot ./plots/raw/$MY_RUN

You can now create the webpage by:

cd plots
make root

The webpage will be placed in the .plots/html/ folder, and can be viewed by using

firefox index.html

when in the folder.
