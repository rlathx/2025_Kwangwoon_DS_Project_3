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

const int inf = 2147483647;  // int maximum value == infinity

vector<int> BFS(Graph* graph, char option, int vertex) {
    // Graph does not exist
    if (graph == nullptr) {
        return {};
    }

    // Non-existent vertex
    if (vertex < 0 || vertex >= graph->getSize()) {
        return {};
    }

    queue<int> q;
    vector<bool> visit(graph->getSize(), false);
    vector<int> bfs;  // result

    q.push(vertex);

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        if (visit[cur] == true) {
            continue;
        }
        visit[cur] = true;
        bfs.push_back(cur);

        // If you declare it outside the while loop, you have to clear it every time.
        // Otherwise, the value will accumulate.
        map<int, int> m;  // to, weight

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
            q.push(end);
        }
    }

    return bfs;
}

vector<int> DFS(Graph* graph, char option, int vertex) {
    // Graph does not exist
    if (graph == nullptr) {
        return {};
    }

    // Non-existent vertex
    if (vertex < 0 || vertex >= graph->getSize()) {
        return {};
    }

    vector<int> stack;
    vector<bool> visit(graph->getSize(), false);
    vector<int> dfs;  // result

    map<int, int> m;  // to, weight

    stack.push_back(vertex);

    while (!stack.empty()) {
        int cur = stack.back();
        stack.pop_back();

        if (visit[cur] == true) {
            continue;
        }
        visit[cur] = true;
        dfs.push_back(cur);

        // Option branch
        if (option == 'O') {
            graph->getAdjacentEdgesDirect(cur, &m);
        } else if (option == 'X') {
            graph->getAdjacentEdges(cur, &m);
        } else {
            // Non-existent option
            return {};
        }

        for (auto it = m.rbegin(); it != m.rend(); it++) {
            int end = it->first;
            stack.push_back(end);
        }
    }

    return dfs;
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

    for (int i = 0; i < graphSize; i++) {
        sort(kruskal[i].begin(), kruskal[i].end());
    }

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

vector<int> Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex) {
    // Graph does not exist
    if (graph == nullptr) {
        return {};
    }

    // Non-existent vertex
    int graphSize = graph->getSize();

    if (s_vertex < 0 || s_vertex >= graphSize) {
        return {};
    }
    if (e_vertex < 0 || e_vertex >= graphSize) {
        return {};
    }

    // 1. initialization
    vector<int> bellmanford(graphSize, inf);
    vector<int> parent(graphSize, -1);  // For parent storage like union-find

    bellmanford[s_vertex] = 0;

    // 2. bellmanford algorithm
    map<int, int> m;  // to, weight

    for (int i = 0; i < graphSize - 1; i++) {
        // Check all edges every time
        for (int j = 0; j < graphSize; j++) {
            if (bellmanford[j] == inf) {
                continue;
            }

            // Option branch
            if (option == 'O') {
                graph->getAdjacentEdgesDirect(j, &m);
            } else if (option == 'X') {
                graph->getAdjacentEdges(j, &m);
            } else {
                // Non-existent option
                return {};
            }

            for (auto it = m.begin(); it != m.end(); it++) {
                int end = it->first;
                int weight = it->second;

                if (bellmanford[end] > bellmanford[j] + weight) {
                    bellmanford[end] = bellmanford[j] + weight;
                    parent[end] = j;
                }
            }
        }
    }

    // 3. Check for negative cycles
    for (int j = 0; j < graphSize; j++) {
        if (bellmanford[j] == inf) {
            continue;
        }

        // Option branch
        if (option == 'O') {
            graph->getAdjacentEdgesDirect(j, &m);
        } else if (option == 'X') {
            graph->getAdjacentEdges(j, &m);
        } else {
            // Non-existent option
            return {};
        }

        for (auto it = m.begin(); it != m.end(); it++) {
            int end = it->first;
            int weight = it->second;

            if (bellmanford[end] > bellmanford[j] + weight) {
                return {};
            }
        }
    }

    // 4. Restoring the path using the parent array (like union-find)
    vector<int> result;

    // If s_vertex is unreachable from e_vertex, return a vector containing only -1.
    // If the vector's size is 1 and the top is -1, output x.
    if (bellmanford[e_vertex] == inf) {
        result.push_back(-1);
        return result;
    }

    for (int i = e_vertex; i != -1; i = parent[i]) {
        result.push_back(i);
    }
    reverse(result.begin(), result.end());
    result.push_back(bellmanford[e_vertex]);

    return result;
}

int** FLOYD(Graph* graph, char option) {
    if (graph == nullptr) {
        return nullptr;
    }

    int graphSize = graph->getSize();
    int** floyd = new int*[graphSize];

    // 1. Initialize the shortest distance table
    for (int i = 0; i < graphSize; i++) {
        floyd[i] = new int[graphSize];

        for (int j = 0; j < graphSize; j++) {
            floyd[i][j] = inf;
        }
    }

    for (int i = 0; i < graphSize; i++) {
        floyd[i][i] = 0;

        // If you declare it outside the while loop, you have to clear it every time.
        // Otherwise, the value will accumulate.
        map<int, int> m;  // to, weight

        // Option branch
        if (option == 'O') {
            graph->getAdjacentEdgesDirect(i, &m);
        } else if (option == 'X') {
            graph->getAdjacentEdges(i, &m);
        } else {
            // Non-existent option
            for (int i = 0; i < graphSize; i++) {
                delete[] floyd[i];
            }
            delete[] floyd;

            return nullptr;
        }

        for (auto it = m.begin(); it != m.end(); it++) {
            int end = it->first;
            int weight = it->second;

            floyd[i][end] = weight;
        }
    }

    // 2. Update the table considering cases where a specific node is passed through
    for (int v = 0; v < graphSize; v++) {
        for (int i = 0; i < graphSize; i++) {
            for (int j = 0; j < graphSize; j++) {
                if (floyd[i][v] == inf || floyd[v][j] == inf) {
                    continue;
                }

                // Overflow prevention
                long long temp = floyd[i][v] + floyd[v][j];
                if (temp < floyd[i][j]) {
                    floyd[i][j] = (int)temp;
                }
            }
        }
    }

    // 3. Negative cycle check
    for (int v = 0; v < graphSize; v++) {
        if (floyd[v][v] < 0) {
            return nullptr;
        }
    }

    return floyd;
}

vector<int> Centrality(Graph* graph) {
    int** floyd = FLOYD(graph, 'X');

    // If there is no graph or a negative cycle occurs during the shortest path calculation.
    if (floyd == nullptr) {
        return {};
    }

    // 1. initialization
    int graphSize = graph->getSize();
    vector<int> centrality(graphSize, 0);

    // 2. The sum of the shortest path costs from all other vertices to that vertex.
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            if (floyd[j][i] == inf) {
                continue;
            }
            centrality[i] += floyd[j][i];
        }
    }

    for (int i = 0; i < graphSize; i++) {
        delete[] floyd[i];
    }
    delete[] floyd;

    return centrality;
}