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
        for (auto it = m.begin(); it != m.end(); it++) {
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

vector<pair<vector<int>, int>> Dijkstra(Graph* graph, char option,
                                        int vertex) {  // vertex: start node
    // Graph does not exist
    if (graph == nullptr) {
        return {};
    }

    // Non-existent vertex
    if (vertex < 0 || vertex >= graph->getSize()) {
        return {};
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        pq;                                       // first: weight, second: vertex
    vector<bool> visit(graph->getSize(), false);  // Visit check
    int inf = 2147483647;                         // int maximum value == infinity
    vector<int> cost(graph->getSize(), inf);      // cost
    vector<int> parent(graph->getSize(), -1);     // For recording the previous peak

    vector<pair<vector<int>, int>> visitNodeKList(graph->getSize());  // Shortest path, cost

    map<int, int> m;  // to, weight

    cost[vertex] = 0;
    pq.push({0, vertex});

    while (!pq.empty()) {
        // 1. cur settings
        // top vertex of priority_queue == cur
        auto curCost = pq.top().first;
        auto cur = pq.top().second;
        pq.pop();

        if (visit[cur]) continue;
        visit[cur] = true;

        // Option branch
        if (option == 'O') {
            graph->getAdjacentEdgesDirect(cur, &m);
        } else if (option == 'X') {
            graph->getAdjacentEdges(cur, &m);
        } else {
            // Non-existent option
            return {};
        }

        for (auto it = m.begin(); it != m.end(); it++) {
            int end = it->first;
            int weight = it->second;

            // Major premise: There are no zero weights.
            // Dijkstra: Negative weights are not allowed.
            if (weight <= 0) {
                return {};
            }

            // 2. Cost update according to newly selected cur
            int nextCost = curCost + weight;
            if (nextCost < cost[end]) {
                cost[end] = nextCost;
                parent[end] = cur;  // The previous vertex on the shortest path to end is cur
                pq.push({nextCost, end});
            }
        }
    }

    // 3. Save final results
    for (int v = 0; v < graph->getSize(); v++) {
        vector<int> temp;

        if (cost[v] == inf) {
            // unreachable peak
            visitNodeKList[v] = {temp, -1};  // If the cost in Manager is -1, output x
            continue;
        }

        // Collect the path backwards along the parent to v
        int cur = v;
        while (cur != -1) {
            temp.push_back(cur);
            cur = parent[cur];
        }
        reverse(temp.begin(), temp.end());

        visitNodeKList[v] = {temp, cost[v]};
    }

    return visitNodeKList;
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex) {
}

bool FLOYD(Graph* graph, char option) {
}

bool Centrality(Graph* graph) {
}