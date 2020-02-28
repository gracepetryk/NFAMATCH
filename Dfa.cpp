//
// Created by gracepetryk on 2/23/20.
//

#include "Dfa.h"
#include "Nfa.h"
#include <stack>
#include <iostream>


using namespace std;

Dfa::Dfa(Nfa& nfa) {

    this->alphabet = nfa.getAlphabet();

    map<set<int>, map<char, set<int>>> mergedNfaTransitionTable;
    set<set<int>> mergedNfaAcceptingStates;
    stack<set<int>> L;
    set<int> dfaStartState = nfa.followLambda({0});
    mergedNfaTransitionTable.insert({dfaStartState, {}});
    L.push(dfaStartState);

    while (!L.empty()) {
        set<int> S = L.top();
        L.pop();

        for (char c : getAlphabet()) {
            set<int> nfaStates = nfa.followLambda(nfa.followChar(S, c));
            if (nfaStates.empty()) {
                continue;
            }
            if (mergedNfaTransitionTable.count(nfaStates) == 0) {
                auto row = pair<set<int>, map<char, set<int>>>(nfaStates, {});
                mergedNfaTransitionTable.insert(row);

                for (int state : nfaStates) {
                    if (nfa.getAcceptingStates().count(state) == 1) {
                        mergedNfaAcceptingStates.insert(nfaStates);
                    }
                }
                L.push(nfaStates);
            }
            mergedNfaTransitionTable.at(S).insert({c, nfaStates});
        }
    }

    // now that we have a transition table, map each set of NFA states to an integer to simplify greatly

    map<set<int>, int> mergedNfaToDfaMap = {{dfaStartState, 0}}; // ensure the start state is always indexed to 0

    int i = 1;
    for (auto const& row : mergedNfaTransitionTable) {
        if (mergedNfaToDfaMap.count(row.first) == 0) {
            mergedNfaToDfaMap.insert({row.first, i});
            ++i;
        }
    }

    for (auto const& row : mergedNfaTransitionTable) {
        map<char, int> transitionTableRow;
        for (auto const& transition : row.second) {
            transitionTableRow.insert({transition.first, mergedNfaToDfaMap.at(transition.second)});
        }
        transitionTable.insert({mergedNfaToDfaMap.at(row.first), transitionTableRow});
    }

    for (auto const& acceptingState: mergedNfaAcceptingStates) {
        acceptingStates.insert(mergedNfaToDfaMap.at(acceptingState));
    }

    printTable();
}

void Dfa::printTable() {
    cout << "    ";
    for (char c : alphabet) {
        cout << c << "   ";
    }
    cout << "\n";
    for (auto const& row : transitionTable) {
        cout << row.first << "   ";
        for (char c : alphabet) {
            if (row.second.count(c) == 1) {
                cout << row.second.at(c) << "   ";
            } else {
                cout << "x   ";
            }
        }
        if (acceptingStates.count(row.first) == 1) {
            cout << '+';
        } else {
            cout << '-';
        }
        cout << "\n";
    }
}

const vector<char> &Dfa::getAlphabet() const {
    return alphabet;
}


