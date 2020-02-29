//
// Created by gracepetryk on 2/28/20.
//

#include "DfaState.h"


bool operator<(const DFAState &state1, const DFAState &state2) {
    return state1.keyInDFA < state2.keyInDFA;
}

void DFAState::updateTransitions(int currentState, int newState) {
    for (auto& transition : transitions) {
        if (transition.second == currentState) {
            transition.second = newState;
        }
    }
}
