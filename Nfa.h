//
// Created by gracepetryk on 2/23/20.
//

#ifndef NFAMATCH_NFA_H
#define NFAMATCH_NFA_H


#include <string>
#include <set>
#include <unordered_map>
#include <vector>

#include "NfaNode.h"

class Nfa {
public:
    explicit Nfa(std::istream& definitionStream);

private:
    unsigned int numNodes{};
    char lambdaChar{};

    std::set<char> alphabet;

    NfaNode* root;

    std::unordered_map<int, NfaNode> nodes;

    NfaNode* addNode(int num);
};




#endif //NFAMATCH_NFA_H
