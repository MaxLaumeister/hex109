#include <vector>
#include <cstdlib>
#include "player.h"
#include "hexBoard.h"

using namespace std;

enum Agent {NOBODY, PLAYER, COM};

class Game {
    public:
      Game() : turn(0) {
          board = new hexBoard(Player::getBoardSize());
      }
      ~Game(){
          delete board;
      }
      void drawBoard();
      void gameLoop();
    private:
      void movePlayer(bool piRule = false);
      int turn;
      hexBoard* board;
};

