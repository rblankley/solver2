# Solver

An edge matching puzzle solver.

Solver is a C++ project that implements a highly optimized depth-first search (DFS) algorithm. This implementation will support up to 31 edge types and 256 pieces (512 pieces on x64 systems) total. For puzzles that are smaller the implementation will scale down accordingly.

The goal of this project is to be the fastest DFS implementation available.

## Building

This project aims to support both 32 and 64 bit variants of Windows and Linux. The code makes use of C++11 so you will need a compiler that supports it (i.e. Studio 2015 or GCC 4.7).

The binary has built in test methods and will validate itself. To perform this, simply run the appropriate option after building the code.
~~~~
solver --validate
~~~~

### Windows

Just open the project in Studio and build. I use the community 2015 version and it works fine.

### Linux

#### Native

Configure and make the project. I use whatever GCC that comes with Ubuntu 14.04.
~~~~
autoreconf -vfi
./configure
make
~~~~

To generate documentation run:
~~~~
make doxygen-doc
~~~~

I've had a lot of luck playing around with default compiler flags. By simply adjusting -O3 optimization flag you can gain 2-3% increases.
~~~~
./configure CXXFLAGS='-O3'
make
~~~~

Of course your mileage may vary...

#### Cross Compiling

Set your toolchain up when configuring the project. Example:
~~~~
autoreconf -vfi
./configure --host=arm-linux-gnueabihf
make
~~~~

## Running

To run the solver simply pass in the width, height, and pieces file. For example, a 16x16 piece puzzle may look like:
~~~~
solver 16 16 pieces.txt
~~~~

The solver logic can build up larger tiles from smaller ones in order to speed up solving. For big puzzles (i.e. 256 pieces) and large tile sets (i.e. 4x8 tile) this could eat up every last byte of memory you have. For this reason I *strongly suggest* you have a look at the usage options. As an example, to have the solver use 2x2 tiles you would run:
~~~~
solver 16 16 pieces.txt --u4
~~~~

The pieces file is formatted with one piece per line, each edge separated by a space. Zero (0) is reserved for border edges. Example:
~~~~
0 0 1 2
1 0 0 4
0 2 3 0
3 4 0 0
~~~~

Have a look in the examples folder for more.

### Advanced Options

By default, the solver will run a single thread and use 1x1 tiles to compute all possible solutions to the puzzle. If you desire different behavior there are several other options available. Use the help argument or no arguments at all to see.
~~~~
solver --help
~~~~

One popular usage option is to just find a single random solution as quick as possible. This could look like:
~~~~
solver -rtpq 16 16 pieces.txt --u4
~~~~

This will run the solver in threaded mode with 2x2 tiles and quit when one solution is found.
