#include <vector>
#include <cstdlib>
#include "hexGraph.h"
#include "player.h"

using namespace std;

enum Agent {NOBODY, PLAYER, COM};

class Game {
    public:
      Game() : turn(0) {
          board = new hexGraph(Player::getBoardSize());
      }
      ~Game(){
          delete board;
      }
      void beginPlay();
      void drawBoard();
    private:
      void mainLoop();
      void movePlayer();
      void moveCom();
      Agent checkWinner();
      int turn;
      hexGraph* board;
};

class Move {
    int x;
    int y;
};

