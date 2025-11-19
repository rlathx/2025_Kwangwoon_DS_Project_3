#include "ListGraph.h"

#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size) {
    this->m_List = new map<int, int>[size];
}

ListGraph::~ListGraph() {
    delete[] this->m_List;
}

void ListGraph::getAdjacentEdges(
    int vertex, map<int, int>* m)  // Definition of getAdjacentEdges(No Direction == Undirected)
{
    m->clear();

    // 1. out edges
    for (auto& p : this->m_List[vertex]) {
        int to = p.first;
        int weight = p.second;
        (*m)[to] = weight;
    }

    // 2. in edges
    for (int u = 0; u < getSize(); u++) {
        if (u == vertex) continue;
        auto it = this->m_List[u].find(vertex);
        if (it != this->m_List[u].end()) {
            (*m)[u] = it->second;
        }
    }
}

void ListGraph::getAdjacentEdgesDirect(
    int vertex, map<int, int>* m)  // Definition of getAdjacentEdges(Directed graph)
{
    m->clear();
    if (this->m_List[vertex].empty()) return;

    for (auto& p : this->m_List[vertex]) {
        int to = p.first;
        int weight = p.second;
        (*m)[to] = weight;
    }
}

// m_List[from][to] = weight - Directed graph
// kw_graph[from].push_back(to) - Undirected graph
void ListGraph::insertEdge(int from, int to, int weight)  // Definition of insertEdge
{
    this->m_List[from][to] = weight;
}

bool ListGraph::printGraph(ofstream* fout)  // Definition of print Graph
{
    if (this->m_List == nullptr) {
        return false;
    }

    (*fout) << "========PRINT========\n";
    for (int i = 0; i < this->getSize(); i++) {
        (*fout) << '[' << i << ']';

        if (this->m_List[i].empty()) {
            (*fout) << " -> ";
        } else {
            for (auto& p : this->m_List[i]) {
                int to = p.first;
                int weight = p.second;

                (*fout) << " -> " << '(' << to << ',' << weight << ')';
            }
        }

        (*fout) << '\n';
    }
    (*fout) << "=======================\n\n";

    return true;
}