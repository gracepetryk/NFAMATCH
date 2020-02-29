//
// Created by gracepetryk on 2/23/20.
//


#ifndef NFAMATCH_DFA_H
#define NFAMATCH_DFA_H

#include "Nfa.h"
#include "DfaState.h"

#include <map>
#include <set>
#include <vector>
#include <iostream>

class Dfa {
public:
    explicit Dfa(Nfa& nfa);
    const std::vector<char> &getAlphabet() const;
    void prettyPrintTable(std::ostream& stream);
    void printTable(std::ostream& stream);
    int match(const string& matchString);

private:
    std::map<int, DFAState> transitionTable;
    std::vector<char> alphabet;

    void optimize();
    std::map<int, DFAState> mergeStates();
    void pruneDeadStates();
    void pruneUnreachableStates();
};


#endif //NFAMATCH_DFA_H
