#ifndef SIMPLEHEXGRAPH_H
#define	SIMPLEHEXGRAPH_H

#include <iostream>
#include <vector>
#include <list>

#include "hexBoard.h"

using namespace std;

// Unweighted undirected graph

class hexGraph {
public:
    hexGraph(const hexBoard* board) : sideLength(board->sideLength), size(board->sideLength * board->sideLength + 4), nodes(board->sideLength * board->sideLength + 4) {init(board);}
    void init(const hexBoard* board);
    friend ostream& operator<<(ostream &out, hexGraph &inGraph);
    Space checkWinner(const hexBoard* board) const;
    bool hasWon(const hexBoard* board, const Space color) const;
    static Space oppositeColor(Space color);
    int getAIMove(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player) const;
    const unsigned int sideLength; // Side length that this graph models
    const unsigned int size; // Size after factoring in pseudonodes
private:
    // hexGraph algorithm
    int getMonteCarloMove(const hexBoard* board, int iterations, const Space currentMove) const;
    int getMonteCarloWeight(const hexBoard &board, const int &iterations, const Space &this_player, const int &move) const;
    pair<int, int> getBestAIMoveWeight(const hexBoard &board, const int &iterations, const int &plies, int threads, const Space &this_player) const;
    int getAIMoveWeight(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player, const int &move) const;
    void getAIMoveWeights(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player, const vector<int> &moves, vector<int> &result) const;
    void getMonteCarloWeights(vector<int> &move_weights, const hexBoard &board, int iterations, const Space this_player) const;
    bool isConnectedDFS(const hexBoard* board, unsigned int node1, unsigned int node2, Space color) const;
    
    // hexGraph
    
    void addEdge(int node1, int node2);
    void addArc(int node1, int node2);
    vector< vector<int> > nodes;
    
    // Pseudonode indices
    
    int pseudo_top;
    int pseudo_bottom;
    int pseudo_left;
    int pseudo_right;
};

#endif	/* SIMPLEHEXGRAPH_H */

