#include <iostream>
#include <fstream>
#include "Nfa.h"
#include "Dfa.h"

using namespace std;

int main(int argc, char *argv[]) {

    ifstream iFileStream (argv[1]);
    ofstream oFileStream (argv[2]);


    Nfa nfa(iFileStream);
    Dfa dfa(nfa);
    dfa.printTable(oFileStream);
    cout << "\n";

    for (size_t i = 3; i < argc; i++) {
        cout << "OUTPUT ";
        string matchString = argv[i];
        if (matchString.size() > 2 && matchString[0] == '\'') {
            matchString = matchString.substr(1, matchString.size() - 2); // trim surrounding quotes if present
        }
        int result = dfa.match(matchString);
        if (result == -1) {
            cout << ":M:";
        } else {
            cout << result;
        }
        cout << "\n\n";
    }

    return 0;
}