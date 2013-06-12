#include <vector>
#include <cstdlib>
#include "player.h"
#include "hexBoard.h"

using namespace std;

enum Agent {NOBODY, PLAYER, COM};

class Game {
    public:
      // Default constructor
      Game() : turn(0), ai_monte_carlo_iterations(5000), ai_plies(1) {
          board = new hexBoard(Player::getBoardSize());
      }
      // You probably want to use this constructor
      Game(int iterations, int plies) : turn(0), ai_monte_carlo_iterations(iterations), ai_plies(plies) {
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
      int ai_monte_carlo_iterations;
      int ai_plies;
      hexBoard* board;
};

