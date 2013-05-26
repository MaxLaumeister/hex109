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
    if (hasWon(board, P_WHITE)) return P_WHITE;
    if (hasWon(board, P_BLACK)) return P_BLACK;
    return P_EMPTY;
}

// Depth first search to see if nodes are connected

bool simpleHexGraph::isConnectedDFS(const hexBoard* board, int node1, int node2, Space color){
    vector<int> visited(size, false);
    return DFSLoop(board, node1, node2, color, &visited);
}

bool simpleHexGraph::DFSLoop(const hexBoard* board, int node1, int node2, Space color, vector<int>* visited) {
    (*visited)[node1] = true; // Set node 1 as visited
    int node_list_size = nodes[node1].size();
    int candidate_node;
    for (int i = 0; i < node_list_size; i++) { // For each node adjacent to node 1
        candidate_node = nodes[node1][i];
        if (candidate_node == node2) return true; // Search is over
        if ((*visited)[candidate_node]) continue;
        if (candidate_node >= size - 4 || board->getSpace(candidate_node) != color) {
            (*visited)[candidate_node] = true;
            continue; // If it's the wrong color, throw it out.
        }
        if (DFSLoop(board, nodes[node1][i], node2, color, visited)) return true; // If we find  the node in a subsequent search, return true
    }
    return false;
}

// Put it all together!

int simpleHexGraph::getMonteCarloMove(const hexBoard* board, const int iterations, const Space currentMove, const Space lastMove){
    // Count the number of unused spaces on the board
    int unused_spaces_count = 0;
    vector<int> unused_spaces;
    for (int i = 0; i < board->getSize(); i++) {
        if (board->getSpace(i) == P_EMPTY) {
            unused_spaces_count++;
            unused_spaces.push_back(i);
        }
    }
    // Fill a vector with alternating moves
    vector<Space> random_chips(unused_spaces_count - 1);
    for (int i = 0; i < unused_spaces_count - 1; i++) {
        if (i % 2 == 0) random_chips[i] = currentMove;
        else random_chips[i] = lastMove;
    }
    // Begin Monte Carlo
    int best_move = -1;
    int best_move_wins = 0;
    hexBoard carlo_board(*board); // Create a working board for iteration
    for (int carlo_move_unusedspaces_index = 0; carlo_move_unusedspaces_index < unused_spaces_count; carlo_move_unusedspaces_index++) { // For each unused space
        carlo_board.setSpace(unused_spaces[carlo_move_unusedspaces_index], currentMove); // Make a move in that space
        // Then run Monte Carlo on the new configuration
        int wins = 0;
        for (int i = 0; i < iterations; i++) { // Iterate over random shuffles
            random_shuffle(random_chips.begin(), random_chips.end()); //  Shuffle the chips
            int i = 0;
            int j = 0;
            while (i < unused_spaces_count - 1) {
                if (j == carlo_move_unusedspaces_index) { // Skip over the space containing the move we made
                    j++;
                    continue;
                }
                carlo_board.setSpace(unused_spaces[j], random_chips[i]); // Copy them into the board
                j++;
                i++;
            }
            if (hasWon(&carlo_board, currentMove)) wins++; // Check to see if it is a win
        }
        if (wins > best_move_wins) {
            best_move_wins = wins;
            best_move = unused_spaces[carlo_move_unusedspaces_index];
        }
        cout << "Move Weight: " << wins << " / " << iterations << endl;
    }
    cout << "Best Move: " << best_move << endl;
    return best_move;
}
