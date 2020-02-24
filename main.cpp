#include <iostream>
#include <fstream>
#include "Nfa.h"

using namespace std;

int main(int argc, char *argv[]) {
    cout << argv[1];
    ifstream fileStream (argv[1]);

    Nfa nfa(fileStream);

    return 0;
}