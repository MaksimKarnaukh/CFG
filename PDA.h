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
};


#endif //CFG1_PDA_H
