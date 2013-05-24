#include <vector>
#include <cstdlib>
#include "hexGraph.h"
#include "player.h"

using namespace std;

enum Agent {NOBODY, PLAYER, COM};

class Game {
    public:
      Game() : turn(0) {
          board = new hexBoard(Player::getBoardSize());
          cpu_graph = new hexGraph(board);
      }
      ~Game(){
          delete board;
      }
      void beginPlay();
      void drawBoard();
    private:
      void mainLoop();
      void movePlayer(bool piRule = false);
      void moveCom();
      Agent checkWinner();
      int turn;
      hexBoard* board;
      hexGraph* cpu_graph;
};

