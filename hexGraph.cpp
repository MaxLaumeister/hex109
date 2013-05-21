#include "hexGraph.h"

inline int hexGraph::getIndex(int x, int y) {
    return widthHeight*y + x;
}

inline Space hexGraph::getSpace(int index) {
   return spaces[index]; 
}

inline Space hexGraph::getSpace(int x, int y) {
    return getSpace(getIndex(x, y));
}
