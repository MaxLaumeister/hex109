#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "game.h"

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

void Game::beginPlay() {
    mainLoop();
}

void Game::mainLoop() {
    Agent winner;
    while(true) {
        drawBoard();
	movePlayer();
	if (winner = checkWinner()) break;
        moveCom();
	if (winner = checkWinner()) break;
    }
    // use winner value
}

void Game::movePlayer(){

    pair<int, int> move;
    
    int badmove = false;
    do {
        if (badmove) {
            drawBoard();
            cout << "Invalid Move." << endl;
        }
        move = Player::getPlayerMove();
        badmove = true;
    } while (!board->isValidMove(move.first, move.second));

    // Record move in game board

    board->setSpace(move.first, move.second, P_WHITE);

    // Update underlying graph to be read by the AI.
    // This removes all outgoing links from the node,
    // forcing it out of the Dijkstra calculation.

    board->clearAdjacency(move.first, move.second);
}

void Game::moveCom(){
    //TODO

    // DEBUG: Run Dijkstra
    list<int> dij = board->dijkstra(board->pseudo_top, board->pseudo_bottom);
    cout << "Dijkstra size " << dij.size() << " of board: " << endl;
    for (list<int>::const_iterator itor = dij.begin(), end = dij.end(); itor != end; ++itor) {
        pair<int, int> thispair = board->getCoords(*itor);
        cout << "(" << thispair.first << ", " << thispair.second << "), ";
    }
    cout << endl;
}

Agent Game::checkWinner(){
    //TODO
    return NOBODY;
}

void Game::drawBoard() {
    
    int boardHeight = board->sideLength;
    int boardWidth = boardHeight;

    // Drawing a nice board ended up being 10x more difficult than I ever
    // thought it could be. Anyway, here's the spaghetti that produces
    // a really pretty command-line game board.

    cout << endl;

    const int totalpadding = log10(boardHeight);

    // Print first row of column numbers

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
    
    for (int i = 0; i < boardWidth + totalpadding + 4; i++) {
        cout << " ";
    }

    for (int j = 0; j < boardWidth; j++) {
            if (j >= 10) cout << j/10 << " ";
	    else cout << j << " ";
    }
    cout << endl;

    // Print second row of column numbers

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
    
}


