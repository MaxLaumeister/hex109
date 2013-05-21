#include <vector>
#include <cstdlib>

using namespace std;

enum Space {P_EMPTY, P_BLACK, P_WHITE};

class Game {
    public:
      Game() {
          init(11, 11);
      }
      Game(int size) {
          init(size, size);
      }
      Game(int width, int height){
          init(width, height);
      }
      void drawBoard();
    private:
      inline void init(int width, int height);
      int boardHeight;
      int boardWidth;
      int turn;
      vector< vector<Space> > board;
};

inline void Game::init(int width, int height) {
    turn = 0;
    if (width > 99 || height > 99) {
        cerr << "Board Width and Height must be less than 100." << endl;
        exit(EXIT_FAILURE);
    }
    boardWidth = width;
    boardHeight = height;
    board.resize(boardHeight, vector<Space>(boardWidth, P_EMPTY));
}

class Move {
    int x;
    int y;
};

