//
// Created by gracepetryk on 2/23/20.
//

#ifndef NFAMATCH_NFA_H
#define NFAMATCH_NFA_H


#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class Nfa {
public:
    explicit Nfa(std::istream& definitionStream);

    const std::set<char> &getAlphabet() const;


    std::unordered_set<int> followLambdaSet(int start);

private:
    unsigned int numNodes{};
    char lambdaChar{};
public:
    char getLambdaChar() const;

private:
    std::set<char> alphabet;
    std::unordered_map<int, std::unordered_multimap<char, int>> states;
    std::unordered_set<int> acceptingStates;

    void followLambdaSetInner(int start, std::unordered_set<int>& lambdaSet);
};




#endif //NFAMATCH_NFA_H
