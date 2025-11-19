#include "MatrixGraph.h"

#include <iostream>
#include <string>
#include <vector>

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size) {
    this->m_Mat = new int*[size];
    for (int i = 0; i < size; i++) {
        this->m_Mat[i] = new int[size]();
    }
}

MatrixGraph::~MatrixGraph() {
    for (int i = 0; i < this->getSize(); i++) {
        delete[] this->m_Mat[i];
    }

    delete[] this->m_Mat;
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m) {
    m->clear();

    for (int u = 0; u < this->getSize(); u++) {
        if (u == vertex) {
            continue;
        }
        int weight1 = this->m_Mat[vertex][u];  // out edges
        int weight2 = this->m_Mat[u][vertex];  // in edges

        if (weight1 != 0) {
            (*m)[u] = weight1;
        }
        if (weight2 != 0) {
            (*m)[u] = weight2;
        }
    }
}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m) {
    m->clear();

    for (int i = 0; i < this->getSize(); i++) {
        if (this->m_Mat[vertex][i] == 0) {
            continue;
        }
        int to = i;
        int weight = this->m_Mat[vertex][i];

        (*m)[to] = weight;
    }
}

void MatrixGraph::insertEdge(int from, int to, int weight) {
    this->m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph(ofstream* fout) {
    if (this->m_Mat == nullptr) {
        return false;
    }

    (*fout) << "========PRINT========\n"
            << "    ";

    for (int i = 0; i < getSize(); i++) {
        (*fout) << ' [' << i << '] ';
    }
    (*fout) << '\n';

    for (int row = 0; row < getSize(); row++) {
        (*fout) << ' [' << row << '] ';
        for (int col = 0; col < getSize(); col++) {
            (*fout) << " " << this->m_Mat[row][col] << "  ";
        }
        (*fout) << '\n';
    }
    (*fout) << "=======================\n\n";

    return true;
}