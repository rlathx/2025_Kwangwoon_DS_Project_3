#include "Manager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GraphMethod.h"

Manager::Manager() {
    graph = nullptr;
    fout.open("log.txt", ios::app);
    load = 0;  // Anything is not loaded
}

Manager::~Manager() {
    if (load)  // if graph is loaded, delete graph
        delete graph;
    if (fout.is_open())  // if fout is opened, close file
        fout.close();    // close log.txt File
}

void Manager::run(const char* command_txt) {
    ifstream fin;                         // Command File Input File Stream
    fin.open(command_txt, ios_base::in);  // File open with read mode

    if (!fin) {  // If command File cannot be read, Print error
        fout << "command file open error" << endl;
        return;  // Return
    }

    string line;

    // Read each command line from the input file
    while (getline(fin, line)) {
        stringstream commandSS(line);
        string cmd;
        commandSS >> cmd;

        if (cmd == "LOAD") {
            string filename;
            commandSS >> filename;

            if (this->LOAD(filename.c_str()) == true) {
                this->fout << "========LOAD========\n"
                           << "Success\n"
                           << "=======================\n\n";
            } else {
                this->printErrorCode(100);
            }
        }
        if (cmd == "PRINT") {
        }
        if (cmd == "BFS") {
        }
        if (cmd == "DFS") {
        }
        if (cmd == "KRUSKAL") {
            if (this->mKRUSKAL()) {
                this->fout << "========KRUSKAL========";
                this->fout << "=======================";
            }
            continue;
        }
        if (cmd == "DIJKSTRA") {
        }
        if (cmd == "BELLMANFORD") {
        }
        if (cmd == "FLOYD") {
        }
        if (cmd == "CENTRALITY") {
        }
        if (cmd == "EXIT") {
        }
    }

    fin.close();
    return;
}

bool Manager::LOAD(const char* filename) {
    // Try opening employee.txt
    ifstream dataFile(filename);
    if (!dataFile.is_open()) {
        return false;
    }

    // Read and insert each record into graph
    string line;

    // 1. graph type (L or M)
    if (!getline(dataFile, line)) {
        return false;
    }
    string graph_type = line;
    // 2. graph size (node count)
    if (!getline(dataFile, line)) {
        return false;
    }
    int graph_size;
    try {
        graph_size = stoi(line);
    } catch (...) {
        return false;
    }

    // If you have an existing graph, organize it
    if (this->graph != nullptr) {
        delete this->graph;
        this->graph = nullptr;
    }

    // 3. Create a graph & connect to this->graph
    if (graph_type == "L") {
        this->graph = new ListGraph(true, graph_size);

        // 4-L. Parsing the remaining lines
        int current_from = -1;
        int to, weight;

        while (getline(dataFile, line)) {
            stringstream ss(line);
            int x, y;

            if (!(ss >> x)) {
                return false;
            }

            if (!(ss >> y)) {
                current_from = x;
            } else {
                if (current_from == -1) {
                    return false;
                }
                to = x;
                weight = y;
            }
            // Insert an edge into the graph
            this->graph->insertEdge(current_from, to, weight);
        }

        return true;
    } else if (graph_type == "M") {
        this->graph = new MatrixGraph(true, graph_size);

        // 4-M. Parsing the remaining lines
        int row = 0;

        while (row < graph_size && getline(dataFile, line)) {
            stringstream ss(line);

            for (int col = 0; col < graph_size; col++) {
                int weight;

                if (!(ss >> weight)) {
                    return false;
                }
                // Insert an edge into the graph
                this->graph->insertEdge(row, col, weight);
            }

            row++;
        }

        if (row != graph_size) {
            return false;
        }

        return true;
    } else {
        return false;
    }

    dataFile.close();
    return true;
}

bool Manager::PRINT() {
    this->printErrorCode(200);
}

bool Manager::mBFS(char option, int vertex) {
    this->printErrorCode(300);
}

bool Manager::mDFS(char option, int vertex) {
    this->printErrorCode(400);
}

bool Manager::mKRUSKAL() {
    this->printErrorCode(500);
    return false;
}

bool Manager::mDIJKSTRA(char option, int vertex) {
    this->printErrorCode(600);
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex) {
    this->printErrorCode(700);
}

bool Manager::mFLOYD(char option) {
    this->printErrorCode(800);
}

bool Manager::mCentrality() {
    this->printErrorCode(900);
}

void Manager::printErrorCode(int n) {
    fout << "========ERROR=======" << endl;
    fout << n << endl;
    fout << "====================" << endl << endl;
}
