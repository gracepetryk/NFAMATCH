//
// Created by gracepetryk on 2/23/20.
//

#include "Dfa.h"
#include "Nfa.h"
#include <stack>
#include <iostream>
#include <iomanip>


using namespace std;

Dfa::Dfa(Nfa& nfa) {

    this->alphabet = nfa.getAlphabet();

    map<set<int>, map<char, set<int>>> mergedNfaTransitionTable;
    set<set<int>> mergedNfaAcceptingStates;
    stack<set<int>> L;
    set<int> dfaStartState = nfa.followLambda({0});
    mergedNfaTransitionTable.insert({dfaStartState, {}});
    L.push(dfaStartState);

    if (nfa.getAcceptingStates().count(0) == 1) {
        // start state is an accepting state
        mergedNfaAcceptingStates.insert(dfaStartState);
    }

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

    int dfaIndex = 1;
    for (auto const& row : mergedNfaTransitionTable) {
        if (mergedNfaToDfaMap.count(row.first) == 0) {
            mergedNfaToDfaMap.insert({row.first, dfaIndex});
            ++dfaIndex;
        }
    }

    // populate transition table with empty DFAStates
    for (size_t i = 0; i < mergedNfaTransitionTable.size(); i++) {
        DFAState state;
        transitionTable.insert({i, state});
    }


    // populate DFAStates with proper values
    for (auto const& row : mergedNfaTransitionTable) {
        DFAState& state = transitionTable.at(mergedNfaToDfaMap.at(row.first));
        state.keyInDFA = mergedNfaToDfaMap.at(row.first);
        for (char c : alphabet) {
            if (row.second.count(c) == 1) {
                // character exists in state
                state.transitions.insert({c, mergedNfaToDfaMap.at(row.second.at(c))});
            } else {
                state.transitions.insert({c, -1}); // insert -1 to represent no transition.
            }
        }
        if (mergedNfaAcceptingStates.count(row.first) == 1){
            state.isAccepting = true;
        }
    }

    transitionTable[0].isStarting = true;
    optimize();
    prettyPrintTable(cout);
}

void Dfa::optimize() {
    map<int, DFAState> testTable;
    do {
        testTable = mergeStates();
        if (testTable.size() < transitionTable.size()) {
            transitionTable = testTable;
        }
    } while (testTable.size() != transitionTable.size());
    
    // do pruneUnreachableStates first because we'll have a graph with less nodes when we invert for pruneDeadStates.
    pruneUnreachableStates();
    pruneDeadStates();
    
    // make all state numbers sequential
    map<int, int> newStateKeys; // maps a state's old key to it's new one

    int last = -1;
    for (auto const& row : transitionTable) {
        if (row.first != last + 1) {
            newStateKeys.insert({row.first, last + 1});
        }
        last++;
    }

    for(pair<int, int> newStateKey : newStateKeys) {
        DFAState tempState = transitionTable.at(newStateKey.first);
        tempState.keyInDFA = newStateKey.second;
        transitionTable.erase(newStateKey.first);
        transitionTable.insert({newStateKey.second, tempState});


        for (auto& state : transitionTable) {
            state.second.updateTransitions(newStateKey.first, newStateKey.second);
        }
    }

    // trim any transitions that point to nodes that don't exist
    for(auto& state : transitionTable) {
        for (auto& transition : state.second.transitions) {
            if (transition.second == -1) {
                continue;
            }
            if (transitionTable.count(transition.second) == 0) {
                transition.second = -1;
            }
        }
    }
}

std::map<int, DFAState> Dfa::mergeStates() {
    stack<pair<set<DFAState>, vector<char>>> lStack; // pair because we need to store alphabet with state
    set<set<DFAState>> mergeSet;

    // push accepting states onto L
    set<DFAState> acceptingStates;
    for (auto const& state : transitionTable) {
        if (state.second.isAccepting) {
            acceptingStates.insert(state.second);
        }
    }
    lStack.push({acceptingStates, alphabet});

    // push accepting states onto L
    set<DFAState> nonAcceptingStates;
    for (auto const& state : transitionTable) {
        if (!state.second.isAccepting) {
            nonAcceptingStates.insert(state.second);
        }
    }
    lStack.push({nonAcceptingStates, alphabet});

    while (!lStack.empty()) {
        set<DFAState> S = lStack.top().first;
        vector<char> C = lStack.top().second;
        lStack.pop();

        char c = C.back();
        C.pop_back();

        if (C.empty()) {
            mergeSet.insert(S);
            continue;
        }

        map<int, set<DFAState>> segregatedStates;
        for (DFAState const& state : S) {
            if (state.transitions.at(c) == -1) {
                // skip empty transitions
               continue;
            }
            if (segregatedStates.count(state.transitions.at(c)) == 1) {
                // if Xn exists, add current state
                segregatedStates.at(state.transitions.at(c)).insert(state); // if Xn exists,
            } else {
                // if not, initialize it and add it to segregatedStates
                segregatedStates.insert({state.transitions.at(c), {state}});
            }
        }
        for (auto const& Xi: segregatedStates) {
            if (Xi.second.size() <= 1) {
                continue;
            }
            if (!C.empty()) {
                lStack.push({Xi.second, C});
            }
        }
    }

    map<int, DFAState> newTable = transitionTable;
    for (set<DFAState> const& states : mergeSet) {
        // merge all states in set to the earliest one;
        int smallestState = newTable.size();
        for (DFAState const& state : states) {
            if (state.keyInDFA < smallestState) {
                smallestState = state.keyInDFA;
            }
        }

        DFAState& mergeToState = newTable.at(smallestState);

        for (DFAState const& state : states) {
            if (state.keyInDFA == smallestState) {
                continue;
            }
            if (state.isAccepting) {
                mergeToState.isAccepting = true;
            }
            if (state.isStarting) {
                mergeToState.isStarting = true;
            }

            for (auto& tableRow : newTable) {
                tableRow.second.updateTransitions(state.keyInDFA, smallestState);
            }

            newTable.erase(state.keyInDFA); // erase old state
        }
    }
    return newTable;
}

void Dfa::pruneDeadStates() {
    map<int, DFAState> reversedTable;
    set<int> validStates; // int because we don't want to add the reversed states, just their indexes. 
    stack<DFAState> statesToVisit;

    // reverse all transitions and add (reversed) accepting states to statesToVisit
    for (auto const& state : transitionTable) {
        // ensure start state gets added
        if (state.second.isStarting) {
            DFAState newState;
            newState.keyInDFA = 0;
            newState.transitions = {};
            reversedTable.insert({0, newState});
        }

        for (pair<char, int> transition : state.second.transitions) {
            if (transition.second < 0 ) {
                // ignore empty transitions
                continue;
            }
            if (reversedTable.count(transition.second) == 0) {
                DFAState newState;
                newState.keyInDFA = transition.second;
                reversedTable.insert({transition.second, newState});
            }
            DFAState& reversedState = reversedTable.at(transition.second);
            reversedState.transitions.insert({transition.first, state.second.keyInDFA});
        }
        if (state.second.isAccepting) {
            if (reversedTable.count(state.second.keyInDFA) == 0) {
                DFAState newState;
                newState.keyInDFA = state.second.keyInDFA;
                reversedTable.insert({state.second.keyInDFA, newState});
            }
            statesToVisit.push(reversedTable.at(state.second.keyInDFA));
            validStates.insert(state.second.keyInDFA);
        }
    }
    
    // follow the reversed graph and add any visited states to validStates
    while (!statesToVisit.empty()) {
        DFAState nextState = statesToVisit.top();
        statesToVisit.pop();

        for (pair<char, int> transition : nextState.transitions) {
            if (transition.second < 0) {
                continue;
            }
            if (validStates.count(transition.second) == 0) {
                // not visited before, add to stack and valid states
                statesToVisit.push(reversedTable.at(transition.second));
                validStates.insert(transition.second);
            }
        }
    }
    
    // delete all states not found in validStates
    set<DFAState> deleteStates;
    for (auto const& state : transitionTable) {
        if (validStates.count(state.first) == 0) {
            deleteStates.insert(state.second);
        }
    }
    
    for (DFAState const& state : deleteStates) {
        transitionTable.erase(state.keyInDFA);
    }
}

void Dfa::pruneUnreachableStates() {
    stack<DFAState> statesToVisit;
    set<DFAState> validStates;
    
    statesToVisit.push(transitionTable.at(0));
    validStates.insert(transitionTable.at(0));
    while(!statesToVisit.empty()) {
        DFAState nextState = statesToVisit.top();
        statesToVisit.pop();
        
        for (pair<char, int> transition : nextState.transitions) {
            if (transition.second < 0) {
                // skip empty transitions
                continue;
            }
            DFAState transitionState = transitionTable.at(transition.second);
            if (validStates.count(transitionState) == 0) {
                // not visted before add to stack and valid states
                validStates.insert(transitionState);
                statesToVisit.push(transitionState);
            }
        }
    }

    // delete all states not found in validStates
    set<DFAState> deleteStates;
    for (auto const& state : transitionTable) {
        if (validStates.count(state.second) == 0) {
            deleteStates.insert(state.second);
        }
    }

    for (DFAState const& state : deleteStates) {
        transitionTable.erase(state.keyInDFA);
    }
}

/**
 *
 * @param matchString - the string to match against
 * @returns - -1 if successful match, n if unsuccessful where n = character failed at (1-indexed)
 */
int Dfa::match(const string& matchString) {
    DFAState currentNode = transitionTable.at(0);
    int currentChar = 0;
    for (char c : matchString) {
        currentChar++;
        bool foundMatch = false;
        for (pair<char, int> transition : currentNode.transitions) {
            if (transition.first == c) {
                if (transition.second < 0) {
                    return currentChar;
                }
                currentNode = transitionTable.at(transition.second);
                foundMatch = true;
                break;
            }
        }
        if (foundMatch) {
            continue;
        }
        // if we get here we failed to match some node, return currentChar
        return  currentChar;
    }

    if (currentNode.isAccepting) {
        return -1;
    } else {
        if (matchString.empty()) {
            return 0;
        }
        return (int) matchString.size() + 1;
    }
}


void Dfa::prettyPrintTable(ostream& stream) {
    stream << "      ";
    for (char c : alphabet) {
        cout << c << "   ";
    }
    stream << "\n";
    for (auto const& state : transitionTable) {
        if (state.second.isAccepting) {
            stream << '+' << " ";
        } else {
            stream << '-' << " ";
        }

        stream << state.second.keyInDFA << "   ";
        for (char c : alphabet) {
            if (state.second.transitions.at(c) >= 1) {
                stream << state.second.transitions.at(c) << "   ";
            } else {
                stream << "E   ";
            }
        }

        stream << "\n";
    }
}


void Dfa::printTable(std::ostream &stream) {
    int width = 2;
    if (transitionTable.size() > 10) {
        width = 3;
    }
    for (auto const& state : transitionTable) {
        if (state.second.isAccepting) {
            stream << '+';
        } else {
            stream << '-';
        }

        stream << setw(width) << state.second.keyInDFA << right;
        for (char c : alphabet) {
            if (state.second.transitions.at(c) >= 1) {
                stream << setw(width) << state.second.transitions.at(c) << right;
            } else {
                stream << setw(width) << "E" << right;
            }
        }

        stream << "\n";
    }
}

const vector<char> &Dfa::getAlphabet() const {
    return alphabet;
}

