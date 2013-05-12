#include <cstdlib>
#include <iostream>
#include <limits>
#include <fstream>

#include "graph.h"

using namespace std;

const int INTEGER_MAX = std::numeric_limits<int>::max();

// Returns a random double between 0 and 1

inline double prob() {
    return static_cast<double> (rand()) / RAND_MAX;
}

Graph::Graph(int s) : size(s), nodes(vector< vector<Arc*> >(size)) {}

Graph::Graph(string inputfile) throw(int) {
    
    size = 0;

    ifstream ifs(inputfile.c_str());
    if (!ifs.is_open()) {
        throw(1); // File not found
    }

    // File is open, get first int as graph size.

    if (!ifs.good()) {
        return;
    }
    int line;
    ifs >> line;
    size = line;
    nodes = vector< vector<Arc*> >(size);

    // Read all the graph info in.

    while (true) {

        // Source node

        if (!ifs.good()) break;
        ifs >> line;
        int source = line;

        // Destination node

        if (!ifs.good()) break;
        ifs >> line;
        int destination = line;

        // Cost

        if (!ifs.good()) break;
        ifs >> line;
        int cost = line;

        // Color

        if (!ifs.good()) break;
        ifs >> line;
        Color color = (Color) line;

        // Add the edge

        addArc(source, destination, cost, color);
    }
    ifs.close();
    return;
}

Graph::~Graph() {
        // Garbage collect arcs and edges
        for (int i = 0; i < size; i++) {
            while (!nodes.at(i).empty()) {
                delete nodes.at(i).back();
                nodes.at(i).pop_back();
            }
        }

    }

bool operator<(const Arc &a1, const Arc &a2) {
    return a1.distance < a2.distance;
}

bool operator>(const Arc &a1, const Arc &a2) {
    return a1.distance > a2.distance;
}

bool operator<=(const Arc &a1, const Arc &a2) {
    return a1.distance <= a2.distance;
}

bool operator>=(const Arc &a1, const Arc &a2) {
    return a1.distance >= a2.distance;
}

// Prettyprint an arc

ostream& operator<<(ostream &out, Arc &inArc) {
    out << inArc.source_node << "->" << inArc.target_node << " (" << inArc.distance << ":" << colorToAlpha(inArc.color) << "), ";
    return out;
}

// Prettyprint an entire graph

ostream& operator<<(ostream &out, Graph &inGraph) {
    out << "Graph:" << endl;

    for (int i = 0; i < inGraph.getSize(); i++) {
        out << i << ": ";
        for (int j = 0; j < inGraph.nodes.at(i).size(); j++) {
            out << *(inGraph.nodes.at(i).at(j)) << " ";
        }
        out << endl;
    }

    return out;
}

string colorToAlpha(Color c) {
    switch (c) {
        case RED: return "RED";
        case GREEN: return "GRN";
        case BLUE: return "BLU";
        case BLACK: return "BLK";
        case ALL: return "ALL";
            return "";
    }
}

// Add a two-way edge to the graph

inline void Graph::addEdge(int node1, int node2, double distance = 1, Color color = BLACK) {
    addArc(node1, node2, distance, color);
    addArc(node2, node1, distance, color);
}

// Add a one-way edge to the graph, as in a digraph

inline void Graph::addArc(int node1, int node2, double distance = 1, Color color = BLACK) {
    Arc* newarc = new Arc(node1, node2, distance, color);
    nodes.at(node1).push_back(newarc);
}

// Fill the graph randomly with a given edge density between 0 and 1

void Graph::randomFill(double density = 0.2, int min_distance = 1, int max_distance = 10) {
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j) continue;
            if (prob() < density) {
                int distance = (rand() % (max_distance - min_distance + 1)) + min_distance;
                Color color = (prob() < 0.5) ? BLACK : RED;
                addEdge(i, j, distance, color);
            }
        }
    }

}

// Computes the path length average of 0 to 1, 0 to 2, 0 to 3, ..., 0 to (size-1)

double Graph::avgPath(Color color) {
    double total = 0;
    int count = 0;
    for (int i = 0; i < getSize(); i++) {
        double path_length = dijkstra(0, i, color);
        if (path_length != -1) {
            total += path_length;
            count++;
        }
        if (debug) cout << total << ", ";
    }
    return total / static_cast<double> (count);
}

double Graph::totalCost() {
    double total = 0;
    for (int source = 0; source < getSize(); source++) {
        int arclen = nodes[source].size();
        for (int arci = 0; arci < arclen; arci++) {
            total += nodes[source][arci]->distance;
        }
    }
    return total;
}

// Prints a graph all ugly, like how it's given to us in the input file.

void Graph::uglyPrint() {
    cout << getSize() << " " << endl;
    Arc* currarc;
    for (int source = 0; source < getSize(); source++) {
        int arclen = nodes[source].size();
        for (int arci = 0; arci < arclen; arci++) {
            currarc = nodes[source][arci];
            cout << currarc->source_node << " "
                 << currarc->target_node << " "
                 << currarc->distance << " "
                 << currarc->color << " " << endl;
        }
    }
}
