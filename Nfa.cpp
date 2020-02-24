//
// Created by Grace Petryk on 2/23/20.
//

#include <istream>
#include <sstream>
#include <iostream>
#include "Nfa.h"
#include "NfaNode.h"
#include "NfaTransition.h"

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

        bool isAccepting;
        isAccepting = acceptingChar == '+';

        int fromNodeNum;
        int toNodeNum;
        lineStream >> fromNodeNum >> toNodeNum;


        NfaNode* fromNode = addNode(fromNodeNum);
        NfaNode* toNode = addNode(toNodeNum);

        if (!root) {
            root = fromNode;
        }

        fromNode->isAccepting = isAccepting;

        char transitionChar;
        if (lineStream >> transitionChar) {
            // create or get origin and destination nodes

            // create transition
            auto* transition = new NfaTransition;

            transition->c = transitionChar;
            transition->isLambda = transitionChar == lambdaChar;
            transition->target = toNode;
            transition->origin = fromNode;

            // add transition to fromNode
            fromNode->transitions.insert(transition);
        }
    }
}

NfaNode* Nfa::addNode(int num) {
    /**
     * adds a node if node does not already exist, returns the new node
     *
     * @returns pointer to the newly added node, or the existing one if it already exists
     */
    if (nodes.count(num) == 0) {
        // create new node
        NfaNode node;

        node.num = num;
        node.transitions = {};

        nodes.insert(pair<int, NfaNode>(num, node));
    }

    return &nodes.at(num);
}
