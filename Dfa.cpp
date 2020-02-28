//
// Created by gracepetryk on 2/23/20.
//

#include "Dfa.h"
#include "Nfa.h"
#include <stack>


using namespace std;

map<char, int> mergeStates(Nfa& nfa, const set<int> states) {
    map<char, int> mergedState;
    for (int state : states) {
        for (char c : nfa.getAlphabet()) {
            auto const& nfaState = nfa.getState(state);
            auto it = nfaState.find(c);
            if (it != nfaState.end()) {
                mergedState.insert(pair<char, int>(c, it->second));
            }
        }
    }
    return mergedState;
}

Dfa::Dfa(Nfa& nfa) {
    map<set<int>, map<char, int>> mergedNfaStates; // maps a state in the nfa to it's merged state in the DFA;
    set<set<int>> dfaAcceptingStates;

    stack<set<int>> L;
    set<int> dfaStartState = nfa.followLambda({0});
    mergedNfaStates.insert(pair<set<int>, map<char, int>>(dfaStartState, mergeStates(nfa, dfaStartState)));
    L.push(dfaStartState);

    while(!L.empty()) {
        set<int> S = L.top();
        L.pop();

        for (char c : nfa.getAlphabet()) {
            set<int> nfaStates = nfa.followLambda(nfa.followChar(S, c));
            if (nfaStates.empty()) {
                continue;
            }
            if (mergedNfaStates.count(nfaStates) == 0) {
                auto row = pair<set<int>, map<char, int>>(nfaStates, mergeStates(nfa, nfaStates));
                mergedNfaStates.insert(row);

                for (int state : nfaStates) {
                    if (nfa.getAcceptingStates().count(state) == 1) {
                        dfaAcceptingStates.insert(nfaStates);
                    }
                }
                L.push(nfaStates);
            }

        }
    }

    transitionTable.insert(pair<int, map<char, int>>(0, mergedNfaStates.at(dfaStartState)));
}

