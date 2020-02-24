//
// Created by gracepetryk on 2/23/20.
//


#ifndef NFAMATCH_DFA_H
#define NFAMATCH_DFA_H

#include "Nfa.h"

#include <map>

class Dfa {
public:
    explicit Dfa(Nfa& nfa);
private:
    std::map<int, std::map<char, int>> transitionTable;
    std::set<int> acceptingStates;
};


#endif //NFAMATCH_DFA_H
