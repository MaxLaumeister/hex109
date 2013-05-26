#ifndef SIMPLEHEXGRAPH_H
#define	SIMPLEHEXGRAPH_H

#include <iostream>
#include <vector>
#include <list>

#include "hexBoard.h"

using namespace std;

// Unweighted undirected graph

class simpleHexGraph {
public:
    simpleHexGraph(const hexBoard* board) : sideLength(board->sideLength), size(board->sideLength * board->sideLength + 4), nodes(board->sideLength * board->sideLength + 4) {init(board);}
    void init(const hexBoard* board);
    friend ostream& operator<<(ostream &out, simpleHexGraph &inGraph);
    const int size; // Size after factoring in pseudonodes
    const int sideLength; // Side length that this graph models
    Space checkWinner(const hexBoard* board);
    bool hasWon(const hexBoard* board, const Space color);
    int getMonteCarloMove(const hexBoard* board, int iterations, const Space currentMove = P_BLACK, const Space lastMove = P_WHITE);
private:
    bool isConnectedDFS(const hexBoard* board, int node1, int node2, Space color);
    bool DFSLoop(const hexBoard* board, int node1, int node2, Space color, vector<int>* visited);
    void addEdge(int node1, int node2);
    void addArc(int node1, int node2);
    vector< vector<int> > nodes;
    
    // Indexes of the pseudonodes
    int pseudo_top;
    int pseudo_bottom;
    int pseudo_left;
    int pseudo_right;
};

#endif	/* SIMPLEHEXGRAPH_H */

