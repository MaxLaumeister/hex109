#include "hexGraph.h"

int hexGraph::getIndex(int x, int y) {
    return widthHeight*y + x;
}

Space hexGraph::getSpace(int index) {
   return spaces[index]; 
}

Space hexGraph::getSpace(int x, int y) {
    return getSpace(getIndex(x, y));
}
