Spies
=====

This is a program designed to figure out solutions to a problem similar to n queens.

This problem is harder than n queens in that it has all of the same rules and one extra preventing any three "queen"s/spies from being in line with eachother. Finding solutions without this restriction is much easier and faster. If you are just looking to solve N queens, feel free to comment out the for loop with the comment "mark all integer intersections of spies lined up" above and it will solve the original N queens problem with ease.

Usage
-----

Simply run `make` then run spies with the size of the board you wish to use.

`./spies [N]`

Special Usage
-------------
This program can be optimized with profile guided optimization. To do so, simply run `make profile` followed by `./spies-profile [N]` where N can be any size, it is recommended to keep N around 30 to give the profile a good idea of what to expect in your program while not taking too long. Then run `make profiled` and the `spies` executable will now be optimized even better than before.

Future
------

In the future there may also be an opencl version of spies to see if it brings any speed up to the table.

