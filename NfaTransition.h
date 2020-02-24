//
// Created by gracepetryk on 2/23/20.
//


#ifndef NFAMATCH_NFATRANSITION_H
#define NFAMATCH_NFATRANSITION_H

#include "NfaNode.h"

struct NfaNode;

struct NfaTransition {
    bool isLambda;
    char c;

    NfaNode* origin;
    NfaNode* target;
};

#endif //NFAMATCH_NFATRANSITION_H


