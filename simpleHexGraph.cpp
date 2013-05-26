#include <vector>
#include <iostream>
#include <assert.h>

#include "simpleHexGraph.h"

using namespace std;

void simpleHexGraph::init(const hexBoard* board) {
    // Initialize the pseudonodes
    pseudo_top = size - 4;
    pseudo_bottom = size - 3;
    pseudo_left = size - 2;
    pseudo_right = size - 1;
          
    // Connect the game board graph
    pair<int, int> coords;
    int x, y;
    for (int i = 0; i < size - 4; i++) {
        coords = board->getCoords(i);
        x = coords.first;
	y = coords.second;
	// Add all adjacent nodes
	if (board->isValidSpace(x - 1, y)) addArc(i, board->getIndex(x - 1, y));
	if (board->isValidSpace(x + 1, y)) addArc(i, board->getIndex(x + 1, y));
        if (board->isValidSpace(x, y - 1)) addArc(i, board->getIndex(x, y - 1));
	if (board->isValidSpace(x - 1, y - 1)) addArc(i, board->getIndex(x - 1, y - 1));
	if (board->isValidSpace(x, y + 1)) addArc(i, board->getIndex(x, y + 1));
	if (board->isValidSpace(x + 1, y + 1)) addArc(i, board->getIndex(x + 1, y + 1));
    }

    // Connect board-edge pseudonodes
    for (int i = 0; i < sideLength; i++) {
        addEdge(pseudo_top, board->getIndex(i, sideLength - 1));
        addEdge(pseudo_bottom, board->getIndex(i, 0));
        addEdge(pseudo_left, board->getIndex(0, i));
        addEdge(pseudo_right, board->getIndex(sideLength - 1, i));
    }
    
    cout << *this;
}

// Prettyprint an entire graph

ostream& operator<<(ostream &out, simpleHexGraph &inGraph) {
    cout << "Graph:" << endl;

    for (int i = 0; i < inGraph.size; i++) {
        cout << i << ": ";
        for (int j = 0; j < inGraph.nodes.at(i).size(); j++) {
            cout << inGraph.nodes.at(i).at(j) << " ";
        }
        cout << endl;
    }

    return cout;
}

// Add a two-way edge to the graph

inline void simpleHexGraph::addEdge(int node1, int node2) {
    addArc(node1, node2);
    addArc(node2, node1);
}

// Add a one-way edge to the graph, as in a digraph

inline void simpleHexGraph::addArc(int node1, int node2) {
    nodes[node1].push_back(node2);
}

inline bool simpleHexGraph::hasWon(const hexBoard* board, const Space color) {
    if (color == P_BLACK) return isConnectedDFS(board, pseudo_bottom, pseudo_top, P_BLACK);
    if (color == P_WHITE) return isConnectedDFS(board, pseudo_left, pseudo_right, P_WHITE);
    assert(color != P_EMPTY); // Color should always be either black or white.
}

Space simpleHexGraph::checkWinner(const hexBoard* board) {
    cout << "Checking if White Wins" << endl;
    if (hasWon(board, P_WHITE)) return P_WHITE;
    cout << "Checking if Black Wins" << endl;
    if (hasWon(board, P_BLACK)) return P_BLACK;
    return P_EMPTY;
}

// Depth first search to see if nodes are connected

bool simpleHexGraph::isConnectedDFS(const hexBoard* board, int node1, int node2, Space color){
    vector<int> visited(size, false);
    return DFSLoop(board, node1, node2, color, &visited);
}

bool simpleHexGraph::DFSLoop(const hexBoard* board, int node1, int node2, Space color, vector<int>* visited) {
    cout << "DFS on " << node1 << endl;
    (*visited)[node1] = true; // Set node 1 as visited
    int node_list_size = nodes[node1].size();
    int candidate_node;
    for (int i = 0; i < node_list_size; i++) { // For each node adjacent to node 1
        candidate_node = nodes[node1][i];
        if (candidate_node == node2) return true; // Search is over
        if ((*visited)[candidate_node]) continue;
        if (candidate_node >= size - 4 || board->getSpace(candidate_node) != color) {
            (*visited)[candidate_node] = true;
            cout << "Node " << candidate_node << " marked as visited." << endl;
            continue; // If it's the wrong color, throw it out.
        }
        if (DFSLoop(board, nodes[node1][i], node2, color, visited)) return true; // If we find  the node in a subsequent search, return true
    }
    return false;
}

// Put it all together!

void simpleHexGraph::getMonteCarloMove(const hexBoard* board, int iterations, Space thisMove, Space lastMove){
    cout << "Monte Carlo Start" << endl;
    cout << "Is Connected: ";
    cout << isConnectedDFS(board, pseudo_bottom, pseudo_top ,thisMove) << endl;
}