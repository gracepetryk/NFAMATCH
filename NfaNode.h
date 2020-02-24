//
// Created by gracepetryk on 2/23/20.
//

#include <unordered_set>

#ifndef NFAMATCH_NFANODE_H
#define NFAMATCH_NFANODE_H


#include "NfaTransition.h"

struct NfaNode {
    int num;
    bool isAccepting;

    std::unordered_set<NfaTransition*> transitions;
};

#endif //NFAMATCH_NFANODE_H

