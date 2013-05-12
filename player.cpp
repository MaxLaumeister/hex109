#include <iostream>
#include "player.h"

using namespace std;

// This namespace contains all functions that retrieve input from the end user.

namespace Player{
    
    // Get the board size from the player.

    int getBoardSize() {
	int size;
	while(true) {
            cout << "Enter Game Board Size (11 is standard): ";
	    cin >> size;
	    if (cin.fail()) {
                cout << "Enter a number lol" << endl << endl;
		cin.clear();
		cin.ignore(10000, '\n');
		continue;
	    }
	    if (size < 100 && size > 1) break; // Good input; break here.
	    if (size >= 100) cout << "Way too ambitious. Try a number less than 100." << endl << endl;
	    if (size <= 1) cout << "Too small. Minimum board size is 2." << endl << endl;
	}
	return size;
    }
}


