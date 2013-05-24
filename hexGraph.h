#ifndef HEXGRAPH_H
#define HEXGRAPH_H

#include "graph.h"
#include "hexBoard.h"

class hexGraph : public Graph {
    public:
      // Constructor adds 4 extra nodes to act as board edge pseudonodes.
      hexGraph(const hexBoard* board) : Graph(board->sideLength * board->sideLength + 4), 
      sideLength(board->sideLength){
          pseudo_top = sideLength * sideLength;
          pseudo_bottom = pseudo_top + 1;
          pseudo_left = pseudo_bottom + 1;
          pseudo_right = pseudo_left + 1;
          init(board);
      };

      void init(const hexBoard* board);
      const int sideLength;

      // Underlying graph manipulating functions

      void zeroCostAdjacency(int index);
      void clearAdjacency(int index);

      // Indexes of the pseudonodes

      int pseudo_top;
      int pseudo_bottom;
      int pseudo_left;
      int pseudo_right;
      
      int pseudo_node_distance;
};

#endif  /* HEXGRAPH_H */
