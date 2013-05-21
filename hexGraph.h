#ifndef HEXGRAPH_H
#define HEXGRAPH_H

#include "graph.h"

enum Space {P_EMPTY, P_BLACK, P_WHITE};

class hexGraph : public Graph {
    public:
      hexGraph(int thisWidthHeight) : Graph(thisWidthHeight){
          widthHeight = thisWidthHeight;
	  spaces = vector<Space>(thisWidthHeight*thisWidthHeight, P_EMPTY);
	  cout << endl << "vector length: " + spaces.size();
      };
      int getIndex(int x, int y);
      Space getSpace(int index);
      Space getSpace(int x, int y);
    private:
      int widthHeight;
      vector<Space> spaces;
};

#endif  /* HEXGRAPH_H */
