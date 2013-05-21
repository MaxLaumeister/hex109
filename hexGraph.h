#ifndef HEXGRAPH_H
#define HEXGRAPH_H

#include "graph.h"
#include "game.h"

using namespace std;

class hexGraph : public Graph {
    public:
      hexGraph(int thisWidthHeight) : Graph(thisWidthHeight){
          widthHeight = thisWidthHeight;
	  spaces = vector<Space>(thisWidthHeight, P_EMPTY);
      };
      Space getSpace(int x, int y);
      Space getSpace(int index);
      int getIndex(int x, int y);
    private:
      int widthHeight;
      vector<Space> spaces;
};

#endif  /* HEXGRAPH_H */
