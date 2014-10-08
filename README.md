## Hex109 - Command Line Hex Game ##

A command line C++ implementation of [Game of Hex](http://en.wikipedia.org/wiki/Hex_(board_game)), with a text interface and simple Monte Carlo AI. It was created as a requirement of the *CMPS 109: Advanced Programming* course at University of California, Santa Cruz.

This program requires C++11 support, which is not provided by older versions of g++. It has been tested successfully using g++ v4.4.6 and v4.8.0.

### Usage ###

    ./hex109 [iterations]

Where *iterations* optionally specifies the number of Monte Carlo iterations to perform (per tile) in the AI calculation. Default is 5000, useful values range from 1000 to 20000.

### Sample Game ###

    ---------------- HEX Turn 16 -----------------
    
                             1
          0 1 2 3 4 5 6 7 8 9 0
       10  - - - - - - - - - - B  10
         9  - - - - - - - - - W B  9
          8  - - - - W - - W W B -  8
           7  - - - - - W - - W B -  7
            6  - - - - - - - - W B -  6
             5  - - - - - B W - - - -  5
              4  - - - - B W - - - - -  4
               3  - - - - B B B B - - -  3
                2  - - - W W W W B B - -  2
                 1  - - - - - - W W B - -  1
                  0  - - - - - - - - B - -  0
                      0 1 2 3 4 5 6 7 8 9 1
                                           0
    
    ---------------------------------------------
    
    Black just moved (9, 6).
    Player (White) connects horizontally.
    CPU (Black) connects vertically.
    Enter move coordinate X:

### License ###

If you distribute this project in part or in full, please attribute with a link to [its GitHub page](https://github.com/MaxLaumeister/Hex109). This software is available under The MIT License, reproduced below.

***

Copyright (c) 2013 Maximillian Laumeister

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
