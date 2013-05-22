#include "hexGraph.h"
#include <assert.h>

void hexGraph::init() {
    // Initialize the game board array
    int s_size = widthHeight * widthHeight;
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
    
    for (int i = 0; i < widthHeight; i++) {
        addArc(pseudo_top, getIndex(i, widthHeight - 1));
        addArc(pseudo_bottom, getIndex(i, 0));
        addArc(pseudo_left, getIndex(0, i));
        addArc(pseudo_right, getIndex(widthHeight - 1, i));
    }
 
    // DEBUG: Print the graph
    uglyPrint();
}

bool hexGraph::isValidSpace(int index) {
    return index >= 0 && index < spaces->size();
}

bool hexGraph::isValidSpace(int x, int y) {
    return x >= 0 && y >= 0 && x < widthHeight && y < widthHeight;
}

int hexGraph::getIndex(int x, int y) {
    return widthHeight*y + x;
}

pair<int, int> hexGraph::getCoords(int index) {
    int x = index % widthHeight;
    int y = index / widthHeight;
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

