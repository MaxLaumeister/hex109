#include <vector>
#include <cstdlib>
#include "hexGraph.h"
#include "player.h"

using namespace std;

class Game {
    public:
      Game() {
          turn = 0;
          widthHeight = Player::getBoardSize();
          board = new hexGraph(widthHeight);
      }
      ~Game(){
          delete board;
      }
      void drawBoard();
    private:
      int turn;
      int widthHeight;
      hexGraph* board;
};

class Move {
    int x;
    int y;
};

