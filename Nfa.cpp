//
// Created by Grace Petryk on 2/23/20.
//

#include <istream>
#include <sstream>
#include <iostream>
#include <stack>
#include "Nfa.h"

using namespace std;

Nfa::Nfa(istream& definitionStream){

    // read header Stream
    string headerLine;
    getline(definitionStream, headerLine);
    istringstream headerStream (headerLine);

    headerStream >> numNodes;
    headerStream >> lambdaChar;

    char token;
    while (headerStream >> token) {
        alphabet.push_back(token);
    }
    cout << "done!";

    string line;
    istringstream lineStream;
    while (getline(definitionStream, line)) {
        lineStream = istringstream(line);

        // read inputs for transition
        char acceptingChar;
        lineStream >> acceptingChar;


        int fromNodeNum;
        int toNodeNum;
        lineStream >> fromNodeNum >> toNodeNum;


        if (acceptingChar == '+') {
            acceptingStates.insert(fromNodeNum);
        }

        char transitionChar;
        if (lineStream >> transitionChar) {
            states[fromNodeNum].insert({transitionChar, toNodeNum});
        } else {
            states[fromNodeNum].insert({});
        }
    }
}

/**
 * @returns the set of states reachable by following all the lambda transitions in followStates
 * @param followStates
 */
set<int> Nfa::followLambda(set<int> followStates) {
    stack<int> L;

    for (auto const& followState : followStates) {
        L.push(followState);
    }

    while (!L.empty()) {
        int state = L.top();
        L.pop();

        // loop over lambda transitions in the current state;
        if (states.at(state).empty()) {
            continue;
        }
        auto range = states.at(state).equal_range(this->getLambdaChar());
        for (auto it = range.first; it != range.second; it++) {
            if (followStates.count(it->second) == 0) {
                L.push(it->second);
                followStates.insert(it->second);
            }
        }
    }
    return followStates;
}
/**
 * @returns the set of all Nfa States reachable by following c-transitions from each state in followStates
 * @param followStates the set states to follow from
 * @param c the character determining what states to follow
 */
set<int> Nfa::followChar(set<int> followStates, const char c) {
    std::set<int> F;
    for (int followSate : followStates) {
        auto range = states.at(followSate).equal_range(c);
        for (auto it = range.first; it != range.second; it++) {
            F.insert(it->second);
        }
    }
    return F;
}

const vector<char> &Nfa::getAlphabet() const {
    return alphabet;
}

char Nfa::getLambdaChar() const {
    return lambdaChar;
}

const multimap<char, int> & Nfa::getState(int state) const {
    return states.at(state);
}

const set<int> &Nfa::getAcceptingStates() const {
    return acceptingStates;
}




