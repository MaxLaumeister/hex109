#include <stack>
#include <queue>
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

typedef pair<int, int> Move;

struct compare  
{  
  bool operator()(const Move& left, const Move& right)  
  {  
      return left.second < right.second;  
  }  
};

void hexGraph::init(const hexBoard* board) {
    // Initialize the pseudonodes
    pseudo_top = size - 4;
    pseudo_bottom = size - 3;
    pseudo_left = size - 2;
    pseudo_right = size - 1;
          
    // Connect the game board graph
    pair<int, int> coords;
    unsigned int x, y;
    for (unsigned int i = 0; i < size - 4; i++) {
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
    for (unsigned int i = 0; i < sideLength; i++) {
        addEdge(pseudo_top, board->getIndex(i, sideLength - 1));
        addEdge(pseudo_bottom, board->getIndex(i, 0));
        addEdge(pseudo_left, board->getIndex(0, i));
        addEdge(pseudo_right, board->getIndex(sideLength - 1, i));
    }
}

// Prettyprint an entire graph

ostream& operator<<(ostream &out, hexGraph &inGraph) {
    out << "Graph:" << endl;

    for (unsigned int i = 0; i < inGraph.size; i++) {
        out << i << ": ";
        for (unsigned int j = 0; j < inGraph.nodes.at(i).size(); j++) {
            out << inGraph.nodes.at(i).at(j) << " ";
        }
        out << endl;
    }

    return out;
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
    assert(false); // Color should always be either black or white.
    return false;
}

Space hexGraph::checkWinner(const hexBoard* board) const {
    if (hasWon(board, P_WHITE)) return P_WHITE;
    if (hasWon(board, P_BLACK)) return P_BLACK;
    return P_EMPTY;
}

inline Space hexGraph::oppositeColor(Space color) {
    if (color == P_BLACK) return P_WHITE;
    else if (color == P_WHITE) return P_BLACK;
    else assert(false);
}

// Depth first search to see if nodes are connected

bool hexGraph::isConnectedDFS(const hexBoard* board, unsigned int node1, unsigned int node2, Space color) const {
    vector<int> visited(size, false);
    stack<int> s;
    s.push(node1);
    unsigned int currnode;
    while (!s.empty()) {
        currnode = s.top();
        s.pop(); // Pop the top node off the stack
        visited[currnode] = true; // Set node 1 as visited
        unsigned int node_list_size = nodes[currnode].size();
        unsigned int candidate_node;
        for (unsigned int i = 0; i < node_list_size; i++) { // For each node adjacent to node 1
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

// Public-facing function that returns the best move for a specific color to make.

int hexGraph::getAIMove(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player) const {
    int threads = 1; //thread::hardware_concurrency(); // Threading only works on some systems
    if (threads < 1) threads = 1;
    pair<int, int> move = getBestAIMoveWeight(board, iterations, plies, threads, this_player);
    if (move.second != 0) return move.first;
    cout << "CPU Resigns. Player wins!" << endl; // If there is no move to keep the player from winning
    exit(0);
}

// Returns (move, weight)

pair<int, int> hexGraph::getBestAIMoveWeight(const hexBoard &board, const int &iterations, const int &plies, int threads, const Space &this_player) const {
    assert(iterations > threads);
    int board_size = board.getSize();
    if (threads > board_size) threads = board_size;
    const int iters_per_thread = iterations / threads;
    const int main_thread_iters = iters_per_thread + (iterations % threads); // Prevent thread round off error
    
    if (DEBUG) cout << "(Depth: " << plies << endl;
    
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
    vector<thread> thread_ids;
    for (int i = 0; i < threads - 1; i++) {
	thread thr = thread(&hexGraph::getAIMoveWeights, this, ref(board), ref(iters_per_thread), ref(plies), ref(this_player), ref(unused_spaces), ref(*(thread_move_weights[i])));
        thread_ids[i] = move(thr);
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
        for (unsigned int j = 0; j < main_thread_weight.size(); j++) {
            cout << main_thread_weight[j] << " ";
        }
        cout << endl;
        cout << "Slave Thread Weights: " << endl;
        for(unsigned int i = 0; i < thread_move_weights.size(); i++) {
            for (unsigned int j = 0; j < thread_move_weights[i]->size(); j++) {
                cout << (*(thread_move_weights[i]))[j] << " ";
            }
            cout << endl << endl;
        }
        cout << "Total Weights: " << endl;
        for (unsigned int j = 0; j < result.size(); j++) {
            cout << result[j] << " ";
        }
        cout << endl;
    }
      
    // Garbage collect
    
    for (int i = 0; i < thread_move_weights_size; i++) {
        delete thread_move_weights[i];
    }
    
    // Convert result vector to max priority queue
    priority_queue<Move, vector<Move>, compare> result_q;
    for (unsigned int i = 0; i < result.size(); i++) {
        //cout << unused_spaces[i] << ", " << result[i] << endl;
        Move newMove(unused_spaces.at(i), result.at(i));
        result_q.push(newMove);
    }
    
    Move bestMove = result_q.top();
    //cout << "Best: " << bestMove.first << ", " << bestMove.second << endl;
    /*while(!result_q.empty()) {
        cout << result_q.top().first << ": " << result_q.top().second << endl;
        result_q.pop();
    }*/
    return bestMove;
}

// Calculate move weights and return them using the input parameter "&result".

void hexGraph::getAIMoveWeights(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player, const vector<int> &moves, vector<int> &result) const {
    const int moves_size = moves.size();
    result.reserve(moves_size);
    for (int i = 0; i < moves_size; i++) {
        result[i] = getAIMoveWeight(board, iterations, plies, this_player, moves[i]);
    }
}

// Calculate the weight for a particular move, either by using Monte Carlo or by going one ply deeper.

inline int hexGraph::getAIMoveWeight(const hexBoard &board, const int &iterations, const int &plies, const Space &this_player, const int &move) const {
    if (plies == 1) return getMonteCarloWeight(board, iterations, this_player, move); // Last ply -> pass off to Monte Carlo method
    assert (plies > 0);
    // Otherwise, recursively calculate the best opponent response
    hexBoard moveMade(board); // Copy board
    moveMade.setSpace(move, this_player); // Make the new move on the board
    // The worse White's next best move is, the better our move is.
    return iterations - getBestAIMoveWeight(moveMade, iterations, plies - 1, 1, oppositeColor(this_player)).second;
}

// Calculate a move weight using Monte Carlo. This represents bottoming-out of the AI.

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
