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

    // Functie toCFG:
    // Converteert een PDA naar een (equivalente) CFG.
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

    // Functie makeRestProductions:
    // Maakt de rest productions, [qXrk] -> a[r0Y1r1][r1Y2r2]...[rk-1Ykrk]
    void makeRestProductions(CFG &cfg);

    // Functie makeBodies:
    // Wordt gebruikt om de bodies van de productions te maken
    // @param temp: opgeslagen laatste symbool bij tot nu toe laatste bracket state
    // @param deelProductions: container waar de productions (bodies) worden opgeslagen.
    // @param t: iterator van de loop over de transitions.
    // @param i: iterator van de loop over de terminal symbols.
    void makeBodies(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int i);

    // Functie addProductionsToCfg:
    // Voegt alle productions (met body bestaande uit stateToString2 en de juiste deelbody van deelProductions, en als head stateToString1) aan de CFG toe.
    void addProductionsToCfg(CFG &cfg, const vector<pair<string, vector<string>>> &deelProductions, const string &stateToString1, const string &stateToString2);

    // Functie createMiddleBracket:
    // Maakt een bracket state aan (niet in het begin of einde van body), en voegt deze toe aan de juiste production body.
    // @param temp: opgeslagen laatste symbool bij tot nu toe laatste bracket state
    // @param deelProductions: container waar de productions (bodies) worden opgeslagen.
    // parameters (int) t en (int) j zijn iterators.
    void createMiddleBracket(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int j);

    // Functie createBeginBracket:
    // Maakt een bracket state aan (in het begin van body)(, en voegt deze toe aan de juiste production body).
    // @param temp: opgeslagen laatste symbool bij tot nu toe laatste bracket state
    // @param deelProductions: container waar de productions (bodies) worden opgeslagen.
    // parameters (int) t en (int) j zijn iterators.
    // @param dP: temporary container voor een temp string (laatste symbool bij vorige bracket) en een production body (pair<temp, prod. body>).
    void createBeginBracket(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int j, pair<string, vector<string>> &dP);

    // Functie createBackBracket:
    // Maakt een bracket state aan (op het einde van een body), en voegt deze toe aan de juiste production body.
    // @param temp: opgeslagen laatste symbool bij tot nu toe laatste bracket state
    // @param deelProductions: container waar de productions (bodies) worden opgeslagen.
    // parameters (int) t, (int) j en (int) j zijn iterators.
    void createBackBracket(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int j, int i);
};


#endif //CFG1_PDA_H
