Og-Go-A.I.
==========

Dependencies and recommended software
---------------

C++ Boost libraries is required to build Og-Go-A.I.
GoGui is not required, but highly recommended to visualise the game better.
Available at http://gogui.sourceforge.net/


Building
---------------
Go to console and write: (Linux specific)
cd src && make && cd ../bin
then start the master by running:
./oggoai
type quit to exit

Og-Go A.I. has only been built and tested on Linux systems but should soon be operational on Windows systems as well

Execution flags
---------------------

To execute the software, either run it in console or as a command in GoGui.

-d displays all debug messages
-v displays all verbose and debug messages
-t runs all tests
-ai [ran|mc|ucb] [params] selects the AI to use for this session. 
    ran is completely random and takes no parameters. 
    mc is Monte Carlo with parameters:
    usb is Upper Confidence Bounds



