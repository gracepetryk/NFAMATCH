#include <iostream>
#include <fstream>
#include "Nfa.h"
#include "Dfa.h"

using namespace std;

int main(int argc, char *argv[]) {

    cout << argv[1];
    ifstream fileStream (argv[1]);

    Nfa nfa(fileStream);
    Dfa dfa(nfa);

    return 0;
}