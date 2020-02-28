//
// Created by gracepetryk on 2/23/20.
//


#ifndef NFAMATCH_DFA_H
#define NFAMATCH_DFA_H

#include "Nfa.h"

#include <map>
#include <set>
#include <vector>

class Dfa {
public:
    explicit Dfa(Nfa& nfa);
    const std::vector<char> &getAlphabet() const;
private:
    std::map<int, std::map<char, int>> transitionTable;
    std::vector<char> alphabet;

private:
    std::set<int> acceptingStates;

    void printTable();
};


#endif //NFAMATCH_DFA_H
