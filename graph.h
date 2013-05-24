#ifndef GRAPH_H
#define	GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <list>

using namespace std;

extern const int debug;

enum Color {
    NONE, RED, GREEN, BLUE, BLACK, ALL
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

// Weighted directed graph

class Graph {
public:

    Graph(int s) : size(s), nodes(vector< vector<Arc*> >(size)) {}
    Graph(string inputfile) throw(int) {
        init(inputfile);
    }
    void init(string inputfile);

    virtual ~Graph();

    inline int getSize() {
        return size;
    };

    inline bool isEmpty() {
        return size == 0;
    }
    void addEdge(int node1, int node2, double distance = 1, Color color = NONE);
    void addArc(int node1, int node2, double distance = 1, Color color = NONE);
    void randomFill(double density, int min_distance, int max_distance);
    Graph* prim(vector<Color> colorlist, int startnode = 0) throw (int);
    list<int> dijkstra(int node1, int node2, Color color = ALL, bool verbose = false);
    double totalCost();
    ostream& prettyPrint(ostream &out = cout);
    ostream& uglyPrint(ostream &out = cout);
    friend ostream& operator<<(ostream &out, Graph &inGraph);
protected:
    int size;
    vector< vector<Arc*> > nodes;
};

#endif	/* GRAPH_H */

