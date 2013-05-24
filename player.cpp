#include <iostream>
#include "player.h"

using namespace std;

namespace Player{

    int getBoardSize() {
	int size;
	while(true) {
            cout << "Enter Game Board Size [default 11]: ";
            if (cin.peek() == '\n') return 11; // Default value
	    cin >> size;
	    if (cin.fail()) {
                cout << "Enter a number please!" << endl << endl;
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
    
    pair<int, int> getPlayerMove() {
        int arg = 0;
        int x;
        int y;
        cout << "Player (White) connects horizontally." << endl;
        cout << "CPU (Black) connects vertically." << endl;
        while(true) {
            if (arg == 0) {
                cout << "Enter move coordinate X: ";
                cin >> x;
            } else if (arg == 1) {
                cout << "Enter move coordinate Y: ";
                cin >> y;
            } else break;

            if (cin.fail()) {
                cout << "Enter a number please!" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            } else arg++;
        }
        return pair<int, int>(x, y);
    }
    
    bool goesFirst() {
        string answer;
        while(true) {
            cout << "Who should go first? [P]layer, [C]PU, or [R]andom: ";
            cin >> answer;
            answer = answer.substr(0,1); // Extract first letter
            if (answer == "P" || answer == "p") return true;
            if (answer == "C" || answer == "c") return false;
            if (answer == "R" || answer == "r") return (rand() % 2); // Random
            cout << "Type \"p\", \"c\", or \"r\"." << endl;
        }
    }
}


