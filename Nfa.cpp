//
// Created by Grace Petryk on 2/23/20.
//

#include <istream>
#include <sstream>
#include <iostream>
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
        alphabet.insert(token);
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
        }
    }
}

void Nfa::followLambdaSetInner(int start, unordered_set<int>& lambdaSet) {

    if (states[start].count(lambdaChar) > 0) {
        auto range = states[start].equal_range(lambdaChar);
        for (auto it = range.first; it != range.second; it++) {
            if (lambdaSet.count(it->second) == 0) {
                lambdaSet.insert(it->second);
                followLambdaSetInner(it->second, lambdaSet);
            }

        }
    }

}

unordered_set<int> Nfa::followLambdaSet(int start) {
    unordered_set<int> lambdaSet;
    followLambdaSetInner(start, lambdaSet);
    return lambdaSet;
}

const set<char> &Nfa::getAlphabet() const {
    return alphabet;
}

char Nfa::getLambdaChar() const {
    return lambdaChar;
}


