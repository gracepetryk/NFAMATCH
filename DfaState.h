//
// Created by gracepetryk on 2/28/20.
//

#ifndef NFAMATCH_DFASTATE_H
#define NFAMATCH_DFASTATE_H

#include <map>

using namespace std;

struct DFAState {
    map<char, int> transitions; // empty transitions marked by -1
    int keyInDFA;
    bool isAccepting;
    bool isStarting;

    friend bool operator<(const DFAState& state1, const DFAState& state2);
    void updateTransitions(int currentState, int newState);
};




#endif //NFAMATCH_DFASTATE_H
