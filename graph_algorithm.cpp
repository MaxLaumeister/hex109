#include <cstdlib>
#include <iostream>
#include <limits>
#include <algorithm>

#include "graph.h"

const double DOUBLE_MAX = std::numeric_limits<double>::max();

// Return the length of the shortest path between node1 and node2

list<int> Graph::dijkstra(const int node1, const int node2, const Color color, const bool verbose) {
    vector<double> d_distance = vector<double>(size); // Tentative distance to each node
    list<int> d_unvisited = list<int>(); // List of unvisited nodes

    if (node1 >= size || node2 >= size || node1 < 0 || node2 < 0) {
        cerr << "Dijkstra: Argument out of bounds." << endl;
        return list<int>();
    }

    if (verbose || debug) cout << "Dijkstra: Finding path from " << node1 << " to " << node2 << "." << endl;

    // Initialize distances
    for (int i = 0; i < size; i++) {
        d_distance.at(i) = DOUBLE_MAX;
    }
    d_distance.at(node1) = 0;

    // Initialize unvisited
    for (int i = 0; i < size; i++) {
        d_unvisited.push_back(i);
    }

    // Begin algorithm
    while (true) {
        // Pop the lowest distance point in unvisited as 'current'
        // For maximum efficiency, this could be reimplemented using a priority queue.
        double mindist = DOUBLE_MAX;
        int current = -1;
        if (debug) cout << "Finding current node: ";
        for (list<int>::const_iterator iterator = d_unvisited.begin(); iterator != d_unvisited.end(); ++iterator) {
            if (d_distance.at(*iterator) < mindist) {
                mindist = d_distance.at(*iterator);
                current = *iterator;
            }
            if (debug) cout << "(" << *iterator << ", " << d_distance.at(*iterator) << ") ";
        }
        if (debug) cout << endl << "Current node found: " << current << endl;
        d_unvisited.remove(current);

        // Break if the connected component has been traversed
        if (current == -1) {
            if (verbose || debug) cout << "Dijkstra: No path found." << endl;
            return list<int>();
        }

        vector<Arc*> neighbors = nodes.at(current);

        // Debug: list all neighbors
        if (debug) {
            cout << "Neighbors of " << current << ": ";
            for (int i = 0; i < neighbors.size(); i++) {
                cout << neighbors.at(i)->target_node << ":" << neighbors.at(i)->distance << " ";
            }
            cout << endl;
        }

        // Update all neighbor node distances
        double currdist = d_distance.at(current);
        if (debug) cout << "Current Node Distance: " << currdist << endl;
        for (int i = 0; i < neighbors.size(); i++) {
            Arc* neighbor = neighbors.at(i);
            if (color != ALL && color != neighbor->color) continue; // Color control
            double altdist = currdist + neighbor->distance;
            if (altdist < d_distance.at(neighbor->target_node)) {
                d_distance.at(neighbor->target_node) = altdist; // Write the updated distance
            }
        }


        // Break if the target node has been reached.
        if (current == node2) {
            if (verbose || debug) cout << "Dijkstra: Path length = " << d_distance.at(node2) << endl;
            return list<int>(d_distance.at(node2));
        }
    }
}

// WARNING: This implementation of Prim's algorithm will only work properly on undirected graphs.

Graph* Graph::prim(const vector<Color> colorlist, const int startnode) throw (int) {
    if (getSize() == 0) return new Graph(0); // Base case

    // If the color list contains ALL, set a flag for that.

    bool use_all_colors = find(colorlist.begin(), colorlist.end(), ALL) != colorlist.end();

    // Create a new graph to hold the minimum spanning tree

    Graph* MSTree = new Graph(getSize());

    // Priority queue keeps track of the lowest-distance reachable arcs.

    priority_queue<Arc, vector<Arc>, greater<vector<Arc>::value_type> > arcs;

    // Vector keeps track of which nodes are part of the MST so far.

    vector<bool> selected(getSize());

    // Start by selecting a starting node

    selected[startnode] = true;
    int selectedcount = 1;

    // and adding all its arcs to the queue (but only the ones with color we are interested in)

    for (int i = 0; i < nodes[startnode].size(); i++) {
        const Arc* currnode = nodes[startnode][i];
        if (use_all_colors || find(colorlist.begin(), colorlist.end(), currnode->color) != colorlist.end()) {
            arcs.push(*currnode);
        }
    }

    // Main screen turn on

    while (selectedcount < getSize()) {

        // Get the lowest distance arc that points to a new node,
        // immediately discarding arcs that point to existing nodes

        int i = 0;
        Arc min_arc;
        do {
            if (arcs.empty()) throw (1); // No more traversable arcs - throw a tantrum about having an incomplete MST
            min_arc = arcs.top();
            arcs.pop();
            //cout << "iter " << i++ << endl;
        } while (selected[min_arc.target_node]);

        // Add the min arc as an edge in the MST

        MSTree->addEdge(min_arc.source_node, min_arc.target_node, min_arc.distance, min_arc.color);

        // Select the new node

        selected[min_arc.target_node] = true;
        selectedcount++;

        // Add all of its arcs to the pool

        for (int i = 0; i < nodes[min_arc.target_node].size(); i++) {
            const Arc* currnode = nodes[min_arc.target_node][i];
            if (use_all_colors || find(colorlist.begin(), colorlist.end(), currnode->color) != colorlist.end()) {
                arcs.push(*currnode);
            }
        }
    }

    return MSTree; // Change this
}

