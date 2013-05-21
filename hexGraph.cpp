#include "hexGraph.h"
#include <assert.h>

bool hexGraph::isValidSpace(int index) {
    return index >= 0 && index < spaces.size();
}

bool hexGraph::isValidSpace(int x, int y) {
    return isValidSpace(getIndex(x, y));
}

int hexGraph::getIndex(int x, int y) {
    return widthHeight*y + x;
}

Space hexGraph::getSpace(int index) {
    assert(isValidSpace(index));
    return spaces.at(index); 
}

Space hexGraph::getSpace(int x, int y) {
    return getSpace(getIndex(x, y));
}

void hexGraph::setSpace(int index, Space sp) {
    assert(isValidSpace(index));
    spaces[index] = sp;
}

void hexGraph::setSpace(int x, int y, Space sp) {
    setSpace(getIndex(x, y), sp);
}

