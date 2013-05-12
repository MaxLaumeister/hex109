#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "game.h"
#include "player.h"

using namespace std;

int main() {
    Game newGame(Player::getBoardSize());
    newGame.drawBoard();

    return 0;
}


