#ifndef HEXGRAPH_H
#define HEXGRAPH_H

#include "graph.h"

enum Space {P_EMPTY, P_BLACK, P_WHITE};

class hexGraph : public Graph {
    public:
      hexGraph(int thisWidthHeight) : Graph(thisWidthHeight){
          widthHeight = thisWidthHeight;
	  for (int i = 0; i < widthHeight*widthHeight; i++) spaces.push_back(P_EMPTY);
	  cout << endl << "vector length: " << spaces.size();
      };
      int getIndex(int x, int y);
      bool isValidSpace(int index);
      bool isValidSpace(int x, int y);
      Space getSpace(int index);
      Space getSpace(int x, int y);
      void setSpace(int index, Space sp);
      void setSpace(int x, int y, Space sp);
    private:
      int widthHeight;
      vector<Space> spaces;
};

#endif  /* HEXGRAPH_H */
