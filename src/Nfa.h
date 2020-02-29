//
// Created by gracepetryk on 2/23/20.
//

#ifndef NFAMATCH_NFA_H
#define NFAMATCH_NFA_H


#include <string>
#include <set>
#include <map>
#include <set>
#include <vector>


class Nfa {
public:
    explicit Nfa(std::istream& definitionStream);

    std::set<int> followLambda(std::set<int> followStates);
    std::set<int> followChar(std::set<int> followStates, char c);


    const std::vector<char> &getAlphabet() const;
    char getLambdaChar() const;

    const std::multimap<char, int> &getState(int state) const;
    const std::set<int> &getAcceptingStates() const;

private:
    unsigned int numNodes{};
    char lambdaChar{};

    std::vector<char> alphabet;
    std::map<int, std::multimap<char, int>> states;
    std::set<int> acceptingStates;

};




#endif //NFAMATCH_NFA_H
