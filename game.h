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
          // DEBUG: Print the graph
          cout << *board;
          // DEBUG: Run Dijkstra
          list<int> dij = board->dijkstra(board->pseudo_top, board->pseudo_bottom);
          cout << "Dijkstra size " << dij.size() << " of board: " << endl;
          for (list<int>::const_iterator itor = dij.begin(), end = dij.end(); itor != end; ++itor) {
              cout << *itor << ", ";
          }
          cout << endl;
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

