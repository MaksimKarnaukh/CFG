//
// Created by Maksim Karnaukh on 28/10/2021.
//

#ifndef CFG1_PDA_H
#define CFG1_PDA_H

#include "CFG.h"

class PDA {

public:
    vector<string> States;
    vector<string> Alphabet;
    vector<string> StackAlphabet;
    nlohmann::basic_json<> Transitions;
    string StartState;
    string StartStack;

    PDA(const string& filename);

    CFG toCFG();

    string createBracketState(const string& from, const string& stacktop, const string& to); // functie om een state van de vorm [pXq] te maken

    void makeVariabeles(CFG &cfg);
};


#endif //CFG1_PDA_H
