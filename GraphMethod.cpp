#include "GraphMethod.h"

#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

bool BFS(Graph* graph, char option, int vertex) {
}

bool DFS(Graph* graph, char option, int vertex) {
}

vector<vector<pair<int, int>>> Kruskal(Graph* graph) {
    if (graph == nullptr) {
        return {};
    }
    int graphSize = graph->getSize();
    // MST
    vector<vector<pair<int, int>>> kruskal(graphSize);  // start <end, weight>

    // 1. Sorting edges in ascending order by weight
    // No duplicate edges
    map<int, int> m;                    // (end, weight)
    map<int, pair<int, int>> edgeList;  // key: weight, value: (start, end)

    for (int i = 0; i < graphSize; i++) {
        graph->getAdjacentEdges(i, &m);
        if (m.empty()) {
            continue;
        }

        // i: start vertex
        // it->first: end vertex
        // it->second: weight
        for (auto it = m.begin(); it != m.end(); ++it) {
            int end = it->first;
            int weight = it->second;

            if (i < end) {
                edgeList.insert({weight, make_pair(i, end)});
            }
        }
    }

    // 2. union-find start
    int* disjointSet = new int[graphSize];  // parent

    for (int i = 0; i < graphSize; i++) {
        disjointSet[i] = i;
    }

    // 3. MST construction by looking at the smallest weights first
    int edgeCount = 0;

    for (auto it = edgeList.begin(); it != edgeList.end(); it++) {
        int weight = it->first;
        int start = it->second.first;
        int end = it->second.second;

        int setS = start;
        int setE = end;

        while (disjointSet[setS] != setS) {
            setS = disjointSet[setS];
        }
        while (disjointSet[setE] != setE) {
            setE = disjointSet[setE];
        }

        if (setS == setE) {
            continue;
        }
        kruskal[start].push_back({end, weight});
        kruskal[end].push_back({start, weight});

        // union
        if (setS < setE) {
            disjointSet[setE] = setS;
        } else {
            disjointSet[setS] = setE;
        }

        edgeCount++;
        if (edgeCount == graphSize - 1) {
            break;
        }
    }

    delete[] disjointSet;
    return kruskal;
}

bool Dijkstra(Graph* graph, char option, int vertex) {
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex) {
}

bool FLOYD(Graph* graph, char option) {
}

bool Centrality(Graph* graph) {
}