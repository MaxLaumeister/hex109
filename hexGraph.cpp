#include <stack>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <thread>
#include <vector>
#include <iostream>
#include <assert.h>
#include <functional>
#include "hexGraph.h"

const bool DEBUG = false;

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

// Add two vectors element by element
template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(result), std::plus<T>());
    return result;
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

inline Space hexGraph::oppositeColor(Space color) {
    if (color == P_BLACK) return P_WHITE;
    else if (color == P_WHITE) return P_BLACK;
    else exit(1);
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
                continue; // If it's the wrong color, or a pseudonode, don't pathfind through it.
            }
            s.push(nodes[currnode][i]); // Push the new node onto the search stack
        }
    }
    return false;
}

int hexGraph::getAIMove(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player) const {
    int threads = thread::hardware_concurrency();
    if (threads < 1) threads = 1;
    pair<int, int> move = getBestAIMoveWeight(board, iterations, plies, threads, this_player);
    if (move.second != 0) return move.first;
    cout << "CPU Resigns. Player wins!" << endl; // If there is no move to keep the player from winning
    exit(0);
}

// Returns (move, weight)

pair<int, int> hexGraph::getBestAIMoveWeight(const hexBoard &board, const int &iterations, const int &plies, int threads, const Space &this_player) const {
    assert(iterations > threads);
    if (threads > board_size) threads = board_size;
    const int iters_per_thread = iterations / threads;
    const int main_thread_iters = iters_per_thread + (iterations % threads); // Prevent thread round off error
    
    if (DEBUG) cout << "(Depth: " << plies << endl;
    int board_size = board.getSize();
    
    // Get indexes of empty spaces on the board
    
    vector<int> unused_spaces;
    unused_spaces.reserve(board_size);
    for (int i = 0; i < board_size; i++) {
        if (board.getSpace(i) == P_EMPTY) {
            unused_spaces.push_back(i);
        }
    }
    const int unused_spaces_size = unused_spaces.size();
    
    // Get AI weights for empty spaces
    
    vector< vector<int>* > thread_move_weights(threads - 1);
    int thread_move_weights_size = thread_move_weights.size();
    for (int i = 0; i < thread_move_weights_size; i++) {
        thread_move_weights[i] = new vector<int>(unused_spaces_size);
    }
    vector<thread> thread_ids(threads - 1);
    for (int i = 0; i < threads - 1; i++) {
        thread_ids[i] = move(thread(&hexGraph::getAIMoveWeights, this, ref(board), ref(iters_per_thread), ref(plies), ref(this_player), ref(unused_spaces), ref(*(thread_move_weights[i]))));
    }
    
    vector<int> main_thread_weight(unused_spaces_size);
    getAIMoveWeights(board, main_thread_iters, plies, this_player, unused_spaces, main_thread_weight);
    
    // Add all the threads
    
    vector<int> result(unused_spaces_size, 0);
    for (int i = 0; i < threads - 1; i++) {
        thread_ids[i].join();
        result = result + (*(thread_move_weights[i]));
    }
    result = result + main_thread_weight;
        
    // DEBUG
    if (DEBUG) {
        cout << "Main Thread Weights: " << endl;
        for (int j = 0; j < main_thread_weight.size(); j++) {
            cout << main_thread_weight[j] << " ";
        }
        cout << endl;
        cout << "Slave Thread Weights: " << endl;
        for(int i = 0; i < thread_move_weights.size(); i++) {
            for (int j = 0; j < thread_move_weights[i]->size(); j++) {
                cout << (*(thread_move_weights[i]))[j] << " ";
            }
            cout << endl << endl;
        }
        cout << "Total Weights: " << endl;
        for (int j = 0; j < result.size(); j++) {
            cout << result[j] << " ";
        }
        cout << endl;
    }
      
    // Garbage collect
    
    for (int i = 0; i < thread_move_weights_size; i++) {
        delete thread_move_weights[i];
    }
    
    // Extract best move weight out of all weights
    int best_move;
    int best_move_weight = 0;
    int move_weights_size = result.size();
    for (int i = 0; i < move_weights_size; i++) {
        if (result[i] > best_move_weight) {
            best_move = unused_spaces[i];
            best_move_weight = result[i];
        }
    }
    if (DEBUG) cout << "Best: " << best_move_weight << ")" << endl;
    return pair<int, int>(best_move, best_move_weight);
}

void hexGraph::getAIMoveWeights(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player, const vector<int> &moves, vector<int> &result) const {
    const int moves_size = moves.size();
    result.reserve(moves_size);
    for (int i = 0; i < moves_size; i++) {
        result[i] = getAIMoveWeight(board, iterations, plies, this_player, moves[i]);
    }
}

inline int hexGraph::getAIMoveWeight(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player, const int &move) const {
    if (plies == 1) return getMonteCarloWeight(board, iterations, this_player, move); // Last ply -> pass off to Monte Carlo method
    assert (plies > 0);
    // Otherwise, recursively calculate the best opponent response
    hexBoard moveMade(board); // Copy board
    moveMade.setSpace(move, this_player); // Make the new move on the board
    // The worse White's next best move is, the better our move is.
    return iterations - getBestAIMoveWeight(moveMade, iterations, plies - 1, 1, oppositeColor(this_player)).second;
}

int hexGraph::getMonteCarloWeight(const hexBoard &board, const int &iterations, const Space &this_player, const int &move) const {
    Space other_player = oppositeColor(this_player);
    
    // Create a working board for iteration
    hexBoard carlo_board(board);
    carlo_board.setSpace(move, this_player); // Make our move on the board
    
    // Count the number of unused spaces on the board
    int unused_spaces_count = 0;
    vector<int> unused_spaces;
    int carlo_board_size = carlo_board.getSize();
    for (int i = 0; i < carlo_board_size; i++) {
        if (carlo_board.getSpace(i) == P_EMPTY) {
            unused_spaces_count++;
            unused_spaces.push_back(i);
        }
    }
    // Fill a vector with alternating moves
    vector<Space> random_chips(unused_spaces_count - 1);
    for (int i = 0; i < unused_spaces_count - 1; i++) { // We will place our chip on the board, so the next move will be the other player's
        if (i % 2 == 0) random_chips[i] = other_player;
        else random_chips[i] = this_player;
    }
    // Then run Monte Carlo
    int wins = 0;
    for (int ii = 0; ii < iterations; ii++) { // Iterate over random shuffles
        random_shuffle(random_chips.begin(), random_chips.end()); //  Shuffle the chips
        for (int i = 0; i < unused_spaces_count; i++) {
            carlo_board.setSpace(unused_spaces[i], random_chips[i]); // Copy them into the board
        }
        if (hasWon(&carlo_board, this_player)) wins++; // Check to see if it is a win
    }
    return wins;
}
