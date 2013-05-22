#ifndef HEXGRAPH_H
#define HEXGRAPH_H

#include "graph.h"

enum Space {P_EMPTY, P_BLACK, P_WHITE};

class hexGraph : public Graph {
    public:
      // This constructor adds 4 extra nodes - these will become board edge pseudonodes.
      hexGraph(int thisWidthHeight) : Graph(thisWidthHeight * thisWidthHeight + 4){
          widthHeight = thisWidthHeight;
          pseudo_top = widthHeight * widthHeight;
          pseudo_bottom = widthHeight * widthHeight + 1;
          pseudo_left = widthHeight * widthHeight + 2;
          pseudo_right = widthHeight * widthHeight + 3;
          init();
      };
      ~hexGraph() {
          delete spaces;
      }
      void init();
      int getIndex(int x, int y);
      pair<int, int> getCoords(int index);
      bool isValidSpace(int index);
      bool isValidSpace(int x, int y);
      Space getSpace(int index);
      Space getSpace(int x, int y);
      void setSpace(int index, Space sp);
      void setSpace(int x, int y, Space sp);
    private:
      int widthHeight;
      vector<Space>* spaces;
      // Indexes of the pseudonodes
      int pseudo_top;
      int pseudo_bottom;
      int pseudo_left;
      int pseudo_right;
};

#endif  /* HEXGRAPH_H */
