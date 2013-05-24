#include <vector>
#include <iostream>
#include <assert.h>

#include "simpleHexGraph.h"

using namespace std;

void simpleHexGraph::init(const hexBoard* board) {
    // Initialize the pseudonodes
    pseudo_top = size;
    pseudo_bottom = pseudo_top + 1;
    pseudo_left = pseudo_bottom + 1;
    pseudo_right = pseudo_left + 1;
          
    // Connect the game board graph
    pair<int, int> coords;
    int x, y;
    for (int i = 0; i < size; i++) {
        coords = board->getCoords(i);
        x = coords.first;
	y = coords.second;
	// Add all adjacent nodes
	if (board->isValidSpace(x - 1, y)) addArc(i, board->getIndex(x - 1, y));
	if (board->isValidSpace(x + 1, y)) addArc(i, board->getIndex(x + 1, y));
        if (board->isValidSpace(x, y - 1)) addArc(i, board->getIndex(x, y - 1));
	if (board->isValidSpace(x - 1, y - 1)) addArc(i, board->getIndex(x - 1, y - 1));
	if (board->isValidSpace(x, y + 1)) addArc(i, board->getIndex(x, y + 1));
	if (board->isValidSpace(x + 1, y + 1)) addArc(i, board->getIndex(x + 1, y + 1));
    }

    // Connect board-edge pseudonodes
    for (int i = 0; i < sideLength; i++) {
        addEdge(pseudo_top, board->getIndex(i, sideLength - 1));
        addEdge(pseudo_bottom, board->getIndex(i, 0));
        addEdge(pseudo_left, board->getIndex(0, i));
        addEdge(pseudo_right, board->getIndex(sideLength - 1, i));
    }
}

// Prettyprint an entire graph

ostream& operator<<(ostream &out, simpleHexGraph &inGraph) {
    cout << "Graph:" << endl;

    for (int i = 0; i < inGraph.size; i++) {
        cout << i << ": ";
        for (int j = 0; j < inGraph.nodes.at(i).size(); j++) {
            cout << inGraph.nodes.at(i).at(j) << " ";
        }
        cout << endl;
    }

    return cout;
}

// Add a two-way edge to the graph

inline void simpleHexGraph::addEdge(int node1, int node2) {
    addArc(node1, node2);
    addArc(node2, node1);
}

// Add a one-way edge to the graph, as in a digraph

inline void simpleHexGraph::addArc(int node1, int node2) {
    nodes[node1].push_back(node2);
}

inline bool simpleHexGraph::hasWon(Space color, const hexBoard* board) {
    if (color == P_WHITE) return isConnectedDFS(pseudo_left, pseudo_right, P_WHITE);
    if (color == P_BLACK) return isConnectedDFS(pseudo_top, pseudo_bottom, P_BLACK);
    assert(false); // Color should always be either black or white.
}

inline Space simpleHexGraph::checkWinner(const hexBoard* board) {
    if (hasWon(P_WHITE, board)) return P_WHITE;
    if (hasWon(P_BLACK, board)) return P_BLACK;
    return P_EMPTY;
}

// Depth first search to see if nodes are connected

bool simpleHexGraph::isConnectedDFS(int node1, int node2, Space color){
    
}

// Put it all together!

void simpleHexGraph::getMonteCarloMove(const hexBoard* board, int iterations, Space thisMove, Space lastMove){
    
}