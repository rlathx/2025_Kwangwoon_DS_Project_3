#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <math.h>
#include <string.h>

#include <algorithm>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

using namespace std;

class Graph {
   protected:
    bool m_Type;
    int m_Size;

   public:
    Graph(bool type, int size);
    virtual ~Graph();

    // True if the graph exists. If true, free the graph memory.
    bool getType() {
        return this->m_Type;
    }
    int getSize() {
        return this->m_Size;
    }

    // A function that transmits information about the neighboring nodes of the current node
    // during graph operations.
    virtual void getAdjacentEdges(int vertex, map<int, int>* m) = 0;
    virtual void getAdjacentEdgesDirect(int vertex, map<int, int>* m) = 0;

    virtual void insertEdge(int from, int to, int weight) = 0;
    virtual bool printGraph(ofstream* fout) = 0;
    virtual void clear() = 0;
};

#endif