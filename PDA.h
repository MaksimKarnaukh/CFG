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

    // Functie createBracketState:
    // Maakt een bracket state [pXq], met (als parameters)
    // p = from
    // X = stacktop
    // q = to
    string createBracketState(const string& from, const string& stacktop, const string& to); // functie om een state van de vorm [pXq] te maken

    // Functie makeVariabeles
    // Maakt alle variabelen en voegt deze toe aan de CFG.
    void makeVariables(CFG &cfg);

    // Functie makeStartProductions:
    // Maakt de start productions, dus alle mogelijkheden S -> [qZ0x] (x element van Q)
    void makeStartProductions(CFG &cfg);

    void makeRestProductions(CFG &cfg);

    void makeBodies(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int i);

    void addProductionsToCfg(CFG &cfg, const vector<pair<string, vector<string>>> &deelProductions, const string &stateToString1, const string &stateToString2);
};


#endif //CFG1_PDA_H
