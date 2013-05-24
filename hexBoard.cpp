#include "hexBoard.h"
#include <assert.h>

bool hexBoard::isValidSpace(int index) const {
    return index >= 0 && index < spaces->size();
}

bool hexBoard::isValidSpace(int x, int y) const {
    return x >= 0 && y >= 0 && x < sideLength && y < sideLength;
}

bool hexBoard::isValidMove(int x, int y, bool piRule) const {
    return isValidSpace(x, y) && (getSpace(x, y) == P_EMPTY || piRule);
}

bool hexBoard::isValidMove(int index, bool piRule) const {
    return isValidSpace(index) && (getSpace(index) == P_EMPTY || piRule);
}

int hexBoard::getIndex(int x, int y) const {
    return sideLength*y + x;
}

pair<int, int> hexBoard::getCoords(int index) const {
    int x = index % sideLength;
    int y = index / sideLength;
    return pair<int, int>(x, y);
}

Space hexBoard::getSpace(int index) const {
    assert(isValidSpace(index));
    return spaces->at(index); 
}

Space hexBoard::getSpace(int x, int y) const {
    return getSpace(getIndex(x, y));
}

void hexBoard::setSpace(int index, Space sp) {
    assert(isValidSpace(index));
    (*spaces)[index] = sp;
}

void hexBoard::setSpace(int x, int y, Space sp) {
    setSpace(getIndex(x, y), sp);
}

