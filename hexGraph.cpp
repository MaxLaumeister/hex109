#include "hexGraph.h"
#include <assert.h>

void hexGraph::init() {
    // Initialize the game board array
    int s_size = sideLength * sideLength;
    spaces = new vector<Space>(s_size, P_EMPTY);
    
    // Connect the game board graph
    pair<int, int> coords;
    int x, y;
    for (int i = 0; i < s_size; i++) {
        coords = getCoords(i);
        x = coords.first;
	y = coords.second;
	// Add all adjacent nodes
	if (isValidSpace(x - 1, y)) addArc(i, getIndex(x - 1, y));
	if (isValidSpace(x + 1, y)) addArc(i, getIndex(x + 1, y));
        if (isValidSpace(x, y - 1)) addArc(i, getIndex(x, y - 1));
	if (isValidSpace(x - 1, y - 1)) addArc(i, getIndex(x - 1, y - 1));
	if (isValidSpace(x, y + 1)) addArc(i, getIndex(x, y + 1));
	if (isValidSpace(x + 1, y + 1)) addArc(i, getIndex(x + 1, y + 1));
    }

    // Connect board-edge pseudonodes
    
    const int max_length = sideLength * sideLength;
    for (int i = 0; i < sideLength; i++) {
        addEdge(pseudo_top, getIndex(i, sideLength - 1), max_length);
        addEdge(pseudo_bottom, getIndex(i, 0), max_length);
        addEdge(pseudo_left, getIndex(0, i), max_length);
        addEdge(pseudo_right, getIndex(sideLength - 1, i), max_length);
    }
}

bool hexGraph::isValidSpace(int index) {
    return index >= 0 && index < spaces->size();
}

bool hexGraph::isValidSpace(int x, int y) {
    return x >= 0 && y >= 0 && x < sideLength && y < sideLength;
}

bool hexGraph::isValidMove(int x, int y) {
    return isValidSpace(x, y) && getSpace(x, y) == P_EMPTY;
}

int hexGraph::getIndex(int x, int y) {
    return sideLength*y + x;
}

pair<int, int> hexGraph::getCoords(int index) {
    int x = index % sideLength;
    int y = index / sideLength;
    return pair<int, int>(x, y);
}

Space hexGraph::getSpace(int index) {
    assert(isValidSpace(index));
    return spaces->at(index); 
}

Space hexGraph::getSpace(int x, int y) {
    return getSpace(getIndex(x, y));
}

void hexGraph::setSpace(int index, Space sp) {
    assert(isValidSpace(index));
    (*spaces)[index] = sp;
}

void hexGraph::setSpace(int x, int y, Space sp) {
    setSpace(getIndex(x, y), sp);
}

