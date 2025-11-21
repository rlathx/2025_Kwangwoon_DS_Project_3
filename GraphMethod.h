#ifndef _GRAPHMETHOD_H_
#define _GRAPHMETHOD_H_

#include "ListGraph.h"
#include "MatrixGraph.h"

bool BFS(Graph* graph, char option, int vertex);
bool DFS(Graph* graph, char option, int vertex);
bool Centrality(Graph* graph);
vector<vector<pair<int, int>>> Kruskal(Graph* graph);
vector<pair<vector<int>, int>> Dijkstra(Graph* graph, char option, int vertex);  // Dijkstra
bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex);         // Bellman - Ford
int** FLOYD(Graph* graph, char option);                                          // FLoyd

#endif