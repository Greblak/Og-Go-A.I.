Og-Go-A.I.
==========

Dependencies and recommended software
---------------

C++ Boost libraries is required to build Og-Go-A.I.
GoGui is not required, but highly recommended to visualise the game better.

Og-Go A.I. has only been built and tested on Linux systems but should soon be operational on Windows systems as well

Execution flags
---------------------

To execute the software, either run it in console or as a command in GoGui.

-d displays all debug messages
-v displays all verbose and debug messages
-rseed [seed] sets the random seed for debugging and reproducing games
-t plays through the GTP commands found in the file test.gtp in the working directory(not included)
-ai [ran|mc|ucb] [params] selects the AI to use for this session. 
    ran is completely random and takes no parameters. 
    mc is Monte Carlo with parameters:
      search width(initial moves to search through) 
      search depth(how many games that will be played out for each initial move)
    ucb is not yet implemented
