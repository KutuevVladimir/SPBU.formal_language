//
// Created by vkutuev on 01.11.18.
//

#include "graph.h"
#include <fstream>

using namespace std;

int read_graph(char* filename, graph_t& graph) {
    int n = 0;
    edge_t edge;
    ifstream input(filename);

    string line ;
    while (getline(input, line)) {
        unsigned long space = line.find(' ');
        string from = line.substr(0, space);
        line = line.substr(space+ 1, line.size() - (space + 1));
        space = line.find(' ');
        string term = line.substr(0, space);
        line = line.substr(space+ 1, line.size() - (space + 1));
        edge.from = stoi(from);
        edge.label = term;
        edge.to = stoi(line);
        n = max(n, edge.from);
        n = max(n, edge.to);
        graph.push_back(edge);
    }
    input.close();
    return n + 1;
}


//void print_graph(graph_t graph) {
//    for (auto &edge: graph)
//        cout << edge.from << ' ' << edge.to << " [" << edge.label << ']' << endl;
//}

