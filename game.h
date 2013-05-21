#include <vector>
#include <cstdlib>
#include "hexGraph.h"

using namespace std;

class Game {
    public:
      Game(int size):board(size), turn(0), widthHeight(size){};
      void drawBoard();
    private:
      int turn;
      int widthHeight;
      hexGraph board;
};

class Move {
    int x;
    int y;
};

