#ifndef HEXGRAPH_H
#define HEXGRAPH_H

#include "graph.h"

enum Space {P_EMPTY, P_BLACK, P_WHITE};

class hexGraph : public Graph {
    public:
      // Constructor adds 4 extra nodes to act as board edge pseudonodes.
      hexGraph(int thisSideLength) : Graph(thisSideLength * thisSideLength + 4), 
      sideLength(thisSideLength){
          pseudo_top = sideLength * sideLength;
          pseudo_bottom = pseudo_top + 1;
          pseudo_left = pseudo_bottom + 1;
          pseudo_right = pseudo_left + 1;
          init();
      };
      ~hexGraph() {
          delete spaces;
      }

      // Board manipulating functions

      void init();
      int getIndex(int x, int y);
      pair<int, int> getCoords(int index);
      Space getSpace(int index);
      Space getSpace(int x, int y);
      void setSpace(int index, Space sp);
      void setSpace(int x, int y, Space sp);
      bool isValidMove(int x, int y);
      const int sideLength;

      // Underlying graph manipulating functions

      void zeroCostAdjacency(int index);
      void clearAdjacency(int index);
      void clearAdjacency(int x, int y);

      // Indexes of the pseudonodes

      int pseudo_top;
      int pseudo_bottom;
      int pseudo_left;
      int pseudo_right;
    private:
      bool isValidSpace(int index);
      bool isValidSpace(int x, int y);
      vector<Space>* spaces;
};

#endif  /* HEXGRAPH_H */
