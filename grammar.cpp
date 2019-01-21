//
// Created by vkutuev on 01.11.18.
//

#include <fstream>
#include <iostream>
#include <ctime>
#include "grammar.h"

using namespace std;

Grammar::Grammar() = default;

void Grammar::read(const string &filename) {
    ifstream input(filename);

    string line;
    while (getline(input, line)) {
        unsigned long divider = line.find(' ');
        string head = line.substr(0, divider);
        string body = line.substr(divider + 1, line.size() - (divider + 1));
        nonterminals[head] = nullptr;
        divider = body.find(' ');
        if (divider == string::npos) {
            terminals[body].push_back(head);
            continue;
        }
        string fst = body.substr(0, divider);
        string snd = body.substr(divider + 1, body.size() - (divider + 1));
        nonterminals[fst] = nullptr;
        nonterminals[snd] = nullptr;
        rules[nonterminal_pair(fst, snd)].push_back(head);
    }
    input.close();
}

void Grammar::intersection_with_graph(int n, graph_t graph, char *filename) {
    clock_t begin = clock();
    for (auto &nonterminal: nonterminals) {
        nonterminal.second = mzd_init(n, n);
    }

    for (auto &edge: graph) {
        for (auto &nonterm: terminals[edge.label]) {
            mzd_write_bit(nonterminals[nonterm], edge.from, edge.to, 1);
        }
    }

    bool finished = false;

    std::map<std::string, bool> changed_previous; // change optimization
    std::map<std::string, bool> changed_next; // change optimization
    for (auto &nonterminal: nonterminals) { // change optimization
        changed_next[nonterminal.first] = true; // change optimization
    } // change optimization
    while (!finished) {
        finished = true;
        for (auto &nonterminal: nonterminals) { // change optimization
            changed_previous[nonterminal.first] = changed_next[nonterminal.first]; // change optimization
            changed_next[nonterminal.first] = false; // change optimization
        } // change optimization
        for (auto &rule: rules) {
            if (changed_previous[rule.first.first] || changed_previous[rule.first.second]) { // change optimization
                mzd_t *mul_result = mzd_sr_mul_m4rm(nullptr, nonterminals[rule.first.first],
                                                    nonterminals[rule.first.second], 0);
                for (auto &head: rule.second) {
                    mzd_t *new_head = mzd_or(nullptr, nonterminals[head], mul_result);
                    if (!mzd_equal(nonterminals[head], new_head)) {
                        mzd_free(nonterminals[head]);
                        nonterminals[head] = new_head;
                        changed_next[head] = true; // change optimization
                        finished = false;
                    } else {
                        mzd_free(new_head);
                    }
                }
                mzd_free(mul_result);
            } // change optimization
        }
    }
    clock_t end = clock();
    cout << (int)(double(end - begin) / CLOCKS_PER_SEC * 1000) << endl;

    ofstream output_file;
    output_file.open(filename);
    for (auto &nonterminal: nonterminals) {
        output_file << nonterminal.first;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (mzd_read_bit(nonterminal.second, i, j) != 0) {
                    output_file << ' ' << i << ' ' << j;
                }
            }
        }
        output_file << endl;
    }
    output_file.close();
}

Grammar::~Grammar() {
    for (auto &nonterminal: nonterminals) {
        mzd_free(nonterminal.second);
    }
}
