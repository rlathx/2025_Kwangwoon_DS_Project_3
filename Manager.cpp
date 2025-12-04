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
            if (!this->PRINT()) {
                this->printErrorCode(200);
            }

            continue;
        }
        if (cmd == "BFS") {
            char option = '\0';
            int vertex = -1;

            commandSS >> option >> vertex;

            if ((option != 'O' && option != 'X') || (vertex == -1)) {
                this->printErrorCode(300);
                continue;
            }

            if (!this->mBFS(option, vertex)) {
                this->printErrorCode(300);
            }
            continue;
        }
        if (cmd == "DFS") {
            char option = '\0';
            int vertex = -1;

            commandSS >> option >> vertex;

            if ((option != 'O' && option != 'X') || (vertex == -1)) {
                this->printErrorCode(400);
                continue;
            }

            if (!this->mDFS(option, vertex)) {
                this->printErrorCode(400);
            }
            continue;
        }
        if (cmd == "KRUSKAL") {
            if (!this->mKRUSKAL()) {
                this->printErrorCode(500);
            }
            continue;
        }
        if (cmd == "DIJKSTRA") {
            char option = '\0';
            int vertex = -1;

            commandSS >> option >> vertex;

            if ((option != 'O' && option != 'X') || (vertex == -1)) {
                this->printErrorCode(600);
                continue;
            }

            if (!this->mDIJKSTRA(option, vertex)) {
                this->printErrorCode(600);
            }
            continue;
        }
        if (cmd == "BELLMANFORD") {
            char option = '\0';
            int start = -1;
            int end = -1;

            commandSS >> option >> start >> end;

            if ((option != 'O' && option != 'X') || (start == -1) || (end == -1)) {
                this->printErrorCode(700);
                continue;
            }

            if (!this->mBELLMANFORD(option, start, end)) {
                this->printErrorCode(700);
            }
            continue;
        }
        if (cmd == "FLOYD") {
            char option = '\0';

            commandSS >> option;

            if (option != 'O' && option != 'X') {
                this->printErrorCode(800);
                continue;
            }

            if (!this->mFLOYD(option)) {
                this->printErrorCode(800);
            }
            continue;
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
    if (this->graph->printGraph(&this->fout)) {
        return true;
    }

    return false;
}

bool Manager::mBFS(char option, int vertex) {
    vector<int> bfs = BFS(this->graph, option, vertex);

    if (bfs.size() == 0) {
        return false;
    }

    this->fout << "========BFS========\n";

    if (option == 'O') {
        this->fout << "Directed Graph BFS\n";
    } else if (option == 'X') {
        this->fout << "Undirected Graph BFS\n";
    }

    this->fout << "Start: " << vertex << '\n';

    for (int v = 0; v < bfs.size(); v++) {
        if (v == 0) {
            this->fout << bfs[v];
            continue;
        }

        this->fout << " -> " << bfs[v];
    }

    this->fout << '\n' << "=======================\n\n";

    return true;
}

bool Manager::mDFS(char option, int vertex) {
    vector<int> dfs = DFS(this->graph, option, vertex);

    if (dfs.size() == 0) {
        return false;
    }

    this->fout << "========DFS========\n";

    if (option == 'O') {
        this->fout << "Directed Graph DFS\n";
    } else if (option == 'X') {
        this->fout << "Undirected Graph DFS\n";
    }

    this->fout << "Start: " << vertex << '\n';

    for (int v = 0; v < dfs.size(); v++) {
        if (v == 0) {
            this->fout << dfs[v];
            continue;
        }

        this->fout << " -> " << dfs[v];
    }

    this->fout << '\n' << "=======================\n\n";

    return true;
}

bool Manager::mKRUSKAL() {
    vector<vector<pair<int, int>>> kruskal = Kruskal(this->graph);

    // error1: No graph
    if (kruskal.size() == 0) {
        return false;
    }

    // error2: If there are inaccessible vertices
    for (int i = 0; i < this->graph->getSize(); i++) {
        if (kruskal[i].size() == 0) {
            return false;
        } else {
            continue;
        }
    }

    int cost = 0;
    this->fout << "========KRUSKAL========\n";
    for (int i = 0; i < this->graph->getSize(); i++) {
        this->fout << ' [' << i << ']';
        for (int j = 0; j < kruskal[i].size(); j++) {
            int end = kruskal[i][j].first;
            int weight = kruskal[i][j].second;
            this->fout << ' ' << end << '(' << weight << ')';

            // Add only once among undirected edges
            if (i < end) {
                cost += weight;
            }
        }
        this->fout << '\n';
    }
    this->fout << "Cost: " << cost << '\n';
    this->fout << "=======================\n\n";

    return true;
}

bool Manager::mDIJKSTRA(char option, int vertex) {
    vector<pair<vector<int>, int>> dijkstra = Dijkstra(this->graph, option, vertex);

    if (dijkstra.size() == 0) {
        return false;
    }

    this->fout << "========DIJKSTRA========\n";

    if (option == 'O') {
        this->fout << "Directed Graph Dijkstra\n";
    } else if (option == 'X') {
        this->fout << "Undirected Graph Dijkstra\n";
    }

    this->fout << "Start: " << vertex << '\n';

    for (int v = 0; v < this->graph->getSize(); v++) {
        this->fout << ' [' << v << '] ';

        if (dijkstra[v].second == -1) {
            this->fout << " x\n";
            continue;
        }

        for (int i = 0; i < dijkstra[v].first.size(); i++) {
            if (i > 0) {
                this->fout << " -> ";
            }
            this->fout << dijkstra[v].first[i];
        }

        this->fout << " (" << dijkstra[v].second << ")\n";
    }

    this->fout << "=======================\n\n";

    return true;
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex) {
    vector<int> bellmanford = Bellmanford(this->graph, option, s_vertex, e_vertex);

    if (bellmanford.size() == 0) {
        return false;
    }

    this->fout << "========BELLMANFORD========\n";

    if (option == 'O') {
        this->fout << "Directed Graph Bellman-Ford\n";
    } else if (option == 'X') {
        this->fout << "Undirected Graph Bellman-Ford\n";
    }

    if ((bellmanford.size() == 1) && (bellmanford.back() == -1)) {
        this->fout << "x\n";
    } else {
        this->fout << bellmanford[0];
        for (int i = 1; i < bellmanford.size() - 1; i++) {
            this->fout << " -> " << bellmanford[i];
        }

        this->fout << "\nCost: " << bellmanford.back() << "\n";
    }
    this->fout << "=======================\n\n";

    return true;
}

bool Manager::mFLOYD(char option) {
    int** floyd = FLOYD(this->graph, option);
    int inf = 2147483647;

    if (floyd == nullptr) {
        return false;
    }

    this->fout << "========FLOYD========\n";

    if (option == 'O') {
        this->fout << "Directed Graph Floyd\n";
    } else if (option == 'X') {
        this->fout << "Undirected Graph Floyd\n";
    }

    this->fout << "    ";

    for (int i = 0; i < this->graph->getSize(); i++) {
        this->fout << ' [' << i << '] ';
    }
    this->fout << '\n';

    for (int row = 0; row < this->graph->getSize(); row++) {
        this->fout << ' [' << row << '] ';

        for (int col = 0; col < this->graph->getSize(); col++) {
            if (floyd[row][col] == inf) {
                this->fout << " " << 'x' << "  ";
            }
            this->fout << " " << floyd[row][col] << "  ";
        }
        this->fout << '\n';
    }
    this->fout << "=======================\n\n";

    for (int i = 0; i < this->graph->getSize(); i++) {
        delete[] floyd[i];
    }
    delete[] floyd;

    return true;
}

bool Manager::mCentrality() {
    this->printErrorCode(900);
}

void Manager::printErrorCode(int n) {
    fout << "========ERROR=======" << endl;
    fout << n << endl;
    fout << "====================" << endl << endl;
}
