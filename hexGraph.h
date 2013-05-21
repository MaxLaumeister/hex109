#ifndef HEXGRAPH_H
#define HEXGRAPH_H

#include "graph.h"

enum Space {P_EMPTY, P_BLACK, P_WHITE};

class hexGraph : public Graph {
    public:
      hexGraph(int thisWidthHeight) : Graph(thisWidthHeight){
          widthHeight = thisWidthHeight;
          spaces = new vector<Space>(widthHeight*widthHeight, P_EMPTY);
      };
      ~hexGraph() {
          delete spaces;
      }
      int getIndex(int x, int y);
      bool isValidSpace(int index);
      bool isValidSpace(int x, int y);
      Space getSpace(int index);
      Space getSpace(int x, int y);
      void setSpace(int index, Space sp);
      void setSpace(int x, int y, Space sp);
    private:
      int widthHeight;
      vector<Space>* spaces;
};

#endif  /* HEXGRAPH_H */
