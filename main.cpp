#include <ctime>
#include "game.h"
#include "player.h"

using namespace std;

int main() {
    srand(time(0));
    Game newGame;
    newGame.gameLoop();
    return 0;
}


