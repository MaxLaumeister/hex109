#include <ctime>
#include <thread>
#include <iostream>
#include "game.h"
#include "player.h"

using namespace std;

void benchmark() {
    unsigned int n = thread::hardware_concurrency();
    if (n <= 1) {
        cout << "No multithreading support detected." << endl;
    } else {
        cout << "Multithreading across " << n << " logical cores." << endl;
    }
}

int main() {
    srand(time(0));
    cout << "-- Hex109 v1.0 --" << endl;
    benchmark();
    Game newGame(5000, 1); // (Iterations, plies) for the ai.
    newGame.gameLoop();
    return 0;
}


