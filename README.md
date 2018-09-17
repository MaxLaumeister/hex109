Hex109 - Command Line Hex Game
==============================

[![Build Status](https://travis-ci.org/MaxLaumeister/hex109.svg?branch=master)](https://travis-ci.org/MaxLaumeister/hex109)

Description
-----------

A command line C++ implementation of [Game of Hex](http://en.wikipedia.org/wiki/Hex_(board_game)), with a text interface and simple Monte Carlo AI. It was created as a requirement of the *CMPS 109: Advanced Programming* course at University of California, Santa Cruz.

This program requires C++11 support, which is not provided by older versions of g++. It has been tested successfully using g++ v4.4.6 and v4.8.0.

Usage
-----

    ./hex109 [iterations]

Where *iterations* optionally specifies the number of Monte Carlo iterations to perform (per tile) in the AI calculation. Default is 5000, useful values range from 1000 to 20000.

Sample Game
-----------

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
