#include <stack>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <thread>

#include "hexGraph.h"

using namespace std;

void hexGraph::init(const hexBoard* board) {
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
}

// Prettyprint an entire graph

ostream& operator<<(ostream &out, hexGraph &inGraph) {
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

inline void hexGraph::addEdge(int node1, int node2) {
    addArc(node1, node2);
    addArc(node2, node1);
}

// Add a one-way edge to the graph, as in a digraph

inline void hexGraph::addArc(int node1, int node2) {
    nodes[node1].push_back(node2);
}

inline bool hexGraph::hasWon(const hexBoard* board, const Space color) const {
    if (color == P_BLACK) return isConnectedDFS(board, pseudo_bottom, pseudo_top, P_BLACK);
    if (color == P_WHITE) return isConnectedDFS(board, pseudo_left, pseudo_right, P_WHITE);
    assert(color != P_EMPTY); // Color should always be either black or white.
}

Space hexGraph::checkWinner(const hexBoard* board) const {
    if (hasWon(board, P_WHITE)) return P_WHITE;
    if (hasWon(board, P_BLACK)) return P_BLACK;
    return P_EMPTY;
}

// Depth first search to see if nodes are connected

bool hexGraph::isConnectedDFS(const hexBoard* board, int node1, int node2, Space color) const {
    vector<int> visited(size, false);
    stack<int> s;
    s.push(node1);
    int currnode;
    while (!s.empty()) {
        currnode = s.top();
        s.pop(); // Pop the top node off the stack
        visited[currnode] = true; // Set node 1 as visited
        int node_list_size = nodes[currnode].size();
        int candidate_node;
        for (int i = 0; i < node_list_size; i++) { // For each node adjacent to node 1
            candidate_node = nodes[currnode][i];
            if (candidate_node == node2) return true; // Search is over
            if (visited[candidate_node]) continue;
            if (candidate_node >= size - 4 || board->getSpace(candidate_node) != color) {
                visited[candidate_node] = true;
                continue; // If it's the wrong color, throw it out.
            }
            s.push(nodes[currnode][i]); // Push the new node onto the search stack
        }
    }
    return false;
}

// Move weights are stored in move_weights input

void hexGraph::getMonteCarloWeights(vector<int> &move_weights, const hexBoard &board, const int iterations, const Space currentMove, const Space lastMove) const{
    assert(move_weights.size() == board.getSize());
    // Count the number of unused spaces on the board
    int unused_spaces_count = 0;
    vector<int> unused_spaces;
    for (int i = 0; i < board.getSize(); i++) {
        if (board.getSpace(i) == P_EMPTY) {
            unused_spaces_count++;
            unused_spaces.push_back(i);
        }
    }
    // Fill a vector with alternating moves
    vector<Space> random_chips(unused_spaces_count - 1);
    for (int i = 0; i < unused_spaces_count - 1; i++) { // We will place our chip on the board, so the next move will be the other player's
        if (i % 2 == 0) random_chips[i] = lastMove;
        else random_chips[i] = currentMove;
    }
    // Begin Monte Carlo
    hexBoard carlo_board(board); // Create a working board for iteration
    for (int carlo_move_unusedspaces_index = 0; carlo_move_unusedspaces_index < unused_spaces_count; carlo_move_unusedspaces_index++) { // For each unused space
        carlo_board.setSpace(unused_spaces[carlo_move_unusedspaces_index], currentMove); // Make a move in that space
        // Then run Monte Carlo on the new configuration
        int wins = 0;
        for (int ii = 0; ii < iterations; ii++) { // Iterate over random shuffles
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
        // Record the times won as the "move weight"
        move_weights[unused_spaces[carlo_move_unusedspaces_index]] = wins;
    }
}

int hexGraph::getMonteCarloMove(const hexBoard* board, int iterations, const Space currentMove, const Space lastMove) const{
    const unsigned int s_threads = thread::hardware_concurrency();
    const unsigned int threads = s_threads > 1 ? s_threads : 1;
    const int iters_per_thread = iterations / threads;
    const int board_size = board->getSize();
    
    // Create an vector to hold the results of each thread
    vector< vector<int>* > results(threads - 1);
    
    // Initialize all of the results vectors
    for (int i = 0; i < threads - 1; i++) {
        results[i] = new vector<int>(board_size, 0);
    }
    
    // Set off monte carlo threads
    vector<thread> thread_ids;
    for (int i = 0; i < threads - 1; i++) {
        thread_ids.push_back(move(thread(&hexGraph::getMonteCarloWeights, this, ref(*(results[i])), ref(*board), iters_per_thread, currentMove, lastMove))); // Gather the results
    }
    
    // Use the main thread to do the same work
    vector<int>* main_thread_results = new vector<int>(board_size);
    getMonteCarloWeights(*main_thread_results, *board, iters_per_thread, currentMove, lastMove);
        
    // DEBUG
    cout << "Main thread with vector: " << endl;
    for (int j = 0; j < main_thread_results->size(); j++) {
        cout << (*main_thread_results)[j] << " ";
    }
    cout << endl;
    
    // Gather monte carlo threads together
    for (int i = 0; i < threads - 1; i++) {
        thread_ids[i].join();
        cout << "Slave thread with vector: " << endl;
        vector<int>* results_i = results[i];
        for (int j = 0; j < results_i->size(); j++) {
            cout << (*results_i)[j] << " ";
        }
        cout << endl;
    }
    
    // Add up all the vectors from results and main_thread_results
    vector<int> final_result(board_size, 0);
    for (int i = 0; i < threads - 1; i++) { // For each one of the results arrays
        for (int j = 0; j < board_size; j++) { // For each one of the board spaces
            final_result[j] += (*(results[i]))[j];
        }
    }
    // Add the results of the main thread as well.
    for (int j = 0; j < board_size; j++) { // For each one of the board spaces
        final_result[j] += (*main_thread_results)[j];
    }
    
    // DEBUG
    cout << "Final Thread Vector: " << endl;
    for (int j = 0; j < final_result.size(); j++) {
        cout << final_result[j] << " ";
    }
    cout << endl;
    
    // Extract the best move from the vector of move weights
    int best_move_index;
    int best_move_weight = 0;
    for (int i = 0; i < final_result.size(); i++) {
        if (final_result[i] > best_move_weight) {
            best_move_index = i;
            best_move_weight = final_result[i];
        }
    }
    
    // Print the best move out for the player
    pair<int, int> bmc = board->getCoords(best_move_index);
    cout << "Best Move Found: (" << bmc.first << ", " << bmc.second << ") with weight " << best_move_weight << endl;
    
    return best_move_index;
}
