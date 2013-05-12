#ifndef GRAPH_H
#define	GRAPH_H

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int debug = false;

enum Color {
    RED, GREEN, BLUE, BLACK, ALL
};

// Each node has a list of Arcs that connect it to other nodes

class Arc {
public:

    Arc() {
    } // Default constructor

    Arc(int s, int t, double d, Color c) : source_node(s), target_node(t), distance(d), color(c) {
    }
    int source_node;
    int target_node;
    double distance;
    Color color;
    friend ostream& operator<<(ostream &out, Arc &inArc);
};

bool operator<(const Arc &a1, const Arc &a2);
bool operator>(const Arc &a1, const Arc &a2);
bool operator<=(const Arc &a1, const Arc &a2);
bool operator>=(const Arc &a1, const Arc &a2);

string colorToAlpha(Color c);

// Weighted undirected graph

class Graph {
public:

    Graph(int s);
    Graph(string inputfile) throw(int);

    ~Graph();

    int getSize() {
        return size;
    };

    bool isEmpty() {
        return size == 0;
    }
    void addEdge(int node1, int node2, double distance, Color color);
    void addArc(int node1, int node2, double distance, Color color);
    void randomFill(double density, int min_distance, int max_distance);
    Graph* prim(vector<Color> colorlist, int startnode = 0) throw (int);
    double dijkstra(int node1, int node2, Color color = ALL, bool verbose = false);
    double avgPath(Color color);
    double totalCost();
    void uglyPrint();
    friend ostream& operator<<(ostream &out, Graph &inGraph);
private:
    int size;
    vector< vector<Arc*> > nodes;
};

#endif	/* GRAPH_H */

