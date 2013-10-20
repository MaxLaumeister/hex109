#include <ctime>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <cstdio>
#include "game.h"
#include "player.h"

using namespace std;

void benchmark() {
    // The threading in this program only works
    // on some systems, so it's disabled by default.

    /*
    unsigned int n = thread::hardware_concurrency();
    if (n <= 1) {
        cout << "No multithreading support detected." << endl;
    } else {
        cout << "Multithreading across " << n << " logical cores." << endl;
    }
    */
}

int main(int argc, const char *argv[]) {
    srand(time(0));
    
    // Parse command line arguments
    
    int iterations;
    if (argc > 1) {
        if ((sscanf(argv[1], "%i", &iterations) != 1) || argc > 2) {
            cerr << "USAGE: " << argv[0] << " [iterations]" << endl;
            cerr << "(Default Monte Carlo iterations is 5000 per tile)" << endl;
            return EXIT_FAILURE;
        }
    } else {
        iterations = 5000;
    }
    
    // Output some decor
    
    cout << "-- Hex109 v1.0 --" << endl;
    benchmark();
    cout << "AI iterations: " << iterations << endl;
    
    // Start game loop
    
    int plies = 1;
    Game newGame(iterations, plies);
    newGame.gameLoop();
    
    return EXIT_SUCCESS;
}


