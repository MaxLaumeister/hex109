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
        turn++;
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
    // Run Dijkstra
    list<int> dij = board->dijkstra(board->pseudo_top, board->pseudo_bottom);

    // Pop off the pseudonodes at the ends of the Djikstra list
    dij.pop_front();
    dij.pop_back();
   
    // DEBUG: Print Dijkstra results
    cout << endl << "Current CPU Strategy (no peeking): " << endl;
    cout << "TOP * ";
    for (list<int>::const_iterator itor = dij.begin(), end = dij.end(); itor != end; ++itor) {
        pair<int, int> thispair = board->getCoords(*itor);
        cout << "(" << thispair.first << ", " << thispair.second << ") * ";
    }
    cout << "BOTTOM" << endl;

    // Find a suitable move:
    // Start building a bridge from the center of the shortest path.
    
    // Find the center
    list<int>::const_iterator center = dij.begin();
    for (int i = 0; i < dij.size() / 2; i++) {
        center++;
    }

    // Walk out from the center to find a valid move
    list<int>::const_iterator walk_left = center;
    list<int>::const_iterator walk_right = center;
    list<int>::const_iterator final_move;
    while(!(walk_left == dij.begin() && walk_right == dij.end())) {
        if (board->isValidMove(*walk_left)) {
            final_move = walk_left;
	    break;
	}
        if (board->isValidMove(*walk_right)) {
            final_move = walk_right;
	    break;
	}
	if (walk_left != dij.begin()) walk_left--;
	if (walk_right != dij.begin()) walk_right++;
    }

    board->setSpace(*final_move, P_BLACK);

    // Update underlying graph to be read by the AI.
    // This makes all outgoing arcs from the move space
    // have cost zero, giving preference to partially-built
    // paths in the Dijkstra algorithm.

    board->zeroCostAdjacency(*final_move);
}

Agent Game::checkWinner(){
    //TODO
    return NOBODY;
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


