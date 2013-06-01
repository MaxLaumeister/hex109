#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "game.h"
#include "hexGraph.h"
#include "hexBoard.h"

using namespace std;

ostream& operator<<(ostream &out, Space sp) {
   switch (sp) {
       case P_EMPTY:
           out << "-";
	   break;
       case P_BLACK:
           out << "B";
	   break;
       case P_WHITE:
           out << "W";
	   break;
   }
   return out;
}

void Game::gameLoop() {
    int monte_carlo_iterations = 200;
    Space winner;
    bool playerWentFirst = Player::goesFirst(); // Get player input
    if (!playerWentFirst) { // Make a predetermined first move
        int rank = min(2, board->sideLength - 1); // Account for teeny tiny boards
        board->setSpace(rank, rank, P_BLACK); // I read somewhere that 2,2 is a balanced opening move (remember pie rule).
    }
    hexGraph gameGraph(board);
    int comMoveIndex = -1;
    while(true) {
        turn++;
        drawBoard();
        if (comMoveIndex != -1) { // Remind the player of where the CPU went
            pair<int, int> comMove = board->getCoords(comMoveIndex);
            cout << "Black just moved (" << comMove.first << ", " << comMove.second << ")." << endl;
        }
	movePlayer(turn == 1 && !playerWentFirst); // Pass whether the pi rule is in effect
        drawBoard();
	if (winner = gameGraph.checkWinner(board)) break;
        cout << "Calculating optimal CPU move..." << endl;
        if (turn == 1 && playerWentFirst) {
            // Calculate move on a fresh board, because we can pie away the opponent's move if needed
            hexBoard temp(board->sideLength); // Fresh board with same dimensions
            comMoveIndex = gameGraph.getAIMove(temp, monte_carlo_iterations, 2, P_BLACK);
            if (board->getSpace(comMoveIndex) == P_WHITE) cout << "CPU Player took your Pie!" << endl;
        } else {
            comMoveIndex = gameGraph.getAIMove(*board, monte_carlo_iterations, 2, P_BLACK); // Calculate move
        }
        board->setSpace(comMoveIndex, P_BLACK);
	if (winner = gameGraph.checkWinner(board)) break;
    }
    drawBoard();
    if (winner == P_WHITE) cout << "Player Wins!" << endl;
    if (winner == P_BLACK) cout << "CPU Player Wins!" << endl;
}

void Game::movePlayer(bool piRule){

    pair<int, int> move;

    if (piRule) cout << "(To use the pie rule, move where the CPU player just did.)" << endl;
    
    int badmove = false;
    do {
        if (badmove) {
            drawBoard();
            cout << "Invalid Move." << endl;
        }
        move = Player::getPlayerMove();
        badmove = true;
    } while (!board->isValidMove(move.first, move.second, piRule));

    // Record move in game board

    board->setSpace(move.first, move.second, P_WHITE);
}

void Game::drawBoard() {
    
    string hz = "-";
 
    int boardHeight = board->sideLength;
    int boardWidth = boardHeight;
    
    int indent = 3;
    int bannersize = (2 * indent) + 5 + boardWidth * 3 + log10(boardWidth);
    
    string vt = "";
    for (int i = 0; i < indent - 1; i++) vt = vt + " "; // This is sloppy, I know.

    // Drawing a nice board ended up being 10x more difficult than I ever
    // thought it could be. Anyway, here's the spaghetti that produces
    // a really pretty command-line game board.

    // First print out a nice turn banner

    cout << endl;

    int turndigits = log10(turn) + 1;
    int bannerpadding = (bannersize - (11 + turndigits)) / 2;

    for (int i = 0; i < bannerpadding; i++) cout << hz;
    
    cout << " HEX Turn " << turn << " ";

    for (int i = 0; i < bannerpadding; i++) cout << hz;
    if (turndigits % 2 == 0) cout << hz;
    
    cout << endl << vt << endl;

    // Now the heavy work of printing the board itself

    const int totalpadding = log10(boardHeight);

    // Print first row of column numbers
    
    cout << vt;
    for (int i = 0; i < totalpadding + 2; i++) {
        cout << " ";
    }

    for (int j = 0; j < boardWidth; j++) {
        if (j < 10) {
            cout << "  ";
	}
	else {
	    cout << j/10 << " ";
	}
    }
    cout << endl;
    
    // Print second row of column numbers
    
    cout << vt;
    for (int i = 0; i < totalpadding + 3; i++) {
        cout << " ";
    }

    for (int j = 0; j < boardWidth; j++) {
            cout << j%10 << " ";
    }
    cout << endl;

    // Print the rows, one by one

    int intlength;
    for (int i = boardHeight - 1; i >= 0; i--) {
	cout << vt;

        // Pad the board on the left to make it a parallelogram
	
	for (int j = 0; j < boardHeight - i - 1; j++) {
            cout << " ";
	}

        // Pad the number on the left, so that the board doesn't shift around.

	intlength = (i == 0) ? 0 : log10(i);
	for (int j = 0; j < totalpadding - intlength; j++) {
            cout << " ";
	}

        // Print the row nunber

	cout << " " << i << "  ";
 
        // Print all of the spaces in the row
        
	for(int j = 0; j < boardWidth; j++) {
            cout << board->getSpace(j, i) << " ";
        }

        // Print the row number one more time on the right

	cout << " " << i << endl;
    }

    // Print first row of column numbers

    cout << vt;
    for (int i = 0; i < boardWidth + totalpadding + 4; i++) {
        cout << " ";
    }

    for (int j = 0; j < boardWidth; j++) {
            if (j >= 10) cout << j/10 << " ";
	    else cout << j << " ";
    }
    cout << endl;

    // Print second row of column numbers
    
    cout << vt;
    for (int i = 0; i < boardWidth + totalpadding + 5; i++) {
        cout << " ";
    }

    for (int j = 0; j < boardWidth; j++) {
        if (j < 10) {
            cout << "  ";
	}
	else {
	    cout << j%10 << " ";
	}
    }
    cout << endl << endl;
    
    for (int i = 0; i < bannersize; i++) cout << hz;

    cout << endl << endl;
    
}


