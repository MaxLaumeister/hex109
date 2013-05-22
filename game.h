#include <vector>
#include <cstdlib>
#include "hexGraph.h"
#include "player.h"

using namespace std;

enum Agent {NOBODY, PLAYER, COM};

class Game {
    public:
      Game() {
          turn = 0;
          widthHeight = Player::getBoardSize();
          board = new hexGraph(widthHeight);
          // DEBUG: Print the graph
          cout << *board;
          // DEBUG: Run Dijkstra
          cout << board->dijkstra(board->pseudo_top, board->pseudo_bottom);
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
      int widthHeight;
      hexGraph* board;
};

class Move {
    int x;
    int y;
};

