#include "hexGraph.h"
#include <assert.h>

void hexGraph::init(const hexBoard* board) {
    int s_size = sideLength * sideLength;
    
    // Connect the game board graph
    pair<int, int> coords;
    int x, y;
    for (int i = 0; i < s_size; i++) {
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
    
    pseudo_node_distance = sideLength * sideLength;
    for (int i = 0; i < sideLength; i++) {
        addEdge(pseudo_top, board->getIndex(i, sideLength - 1), pseudo_node_distance);
        addEdge(pseudo_bottom, board->getIndex(i, 0), pseudo_node_distance);
        addEdge(pseudo_left, board->getIndex(0, i), pseudo_node_distance);
        addEdge(pseudo_right, board->getIndex(sideLength - 1, i), pseudo_node_distance);
    }
}

void hexGraph::clearAdjacency(int index) {
    nodes.at(index).clear();
}

void hexGraph::zeroCostAdjacency(int index) {
    for (int i = 0; i < nodes.at(index).size(); i++) {
        nodes.at(index).at(i)->distance = 0;
    }
}
