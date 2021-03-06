//
// Created by Maksim Karnaukh on 28/10/2021.
//

#include "PDA.h"

PDA::PDA(const string &filename) {

    ifstream input(filename);
    if (!input.is_open()) {
        cerr << "Error while reading file contents" << endl;
    }
    json j;
    input >> j;

    vector<string> s0 = j["States"];
    States = s0;
    vector<string> a = j["Alphabet"];
    Alphabet = a;
    vector<string> s1 = j["StackAlphabet"];
    StackAlphabet = s1;

    Transitions = j["Transitions"];
    StartState = j["StartState"];
    StartStack = j["StartStack"];

}

CFG PDA::toCFG() {

    CFG cfg;

    cfg.T = this->Alphabet; // blijft hetzelfde
    cfg.S = "S"; // standaard S?
    cfg.V.emplace_back("S");

    makeVariables(cfg);
    makeStartProductions(cfg);
    makeRestProductions(cfg);

    return cfg;
}

string PDA::createBracketState(const string& from, const string& stacktop, const string& to) {
    string bracketState;
    bracketState += "[";
    bracketState += from;
    bracketState += ",";
    bracketState += stacktop;
    bracketState += ",";
    bracketState += to;
    bracketState += "]";
    return bracketState;

}

void PDA::makeVariables(CFG &cfg) {

    for (int i = 0; i < this->States.size(); i++) { // variabelen aanmaken
        for (int j = 0; j < this->States.size(); j++) {
            for (int symbol = 0; symbol < this->StackAlphabet.size(); symbol++) {
                string stateToString = createBracketState(this->States[i], this->StackAlphabet[symbol], this->States[j]);
                cfg.V.push_back(stateToString);
            }
        }
    }
}

void PDA::makeStartProductions(CFG &cfg) {
    for (int i = 0; i < this->States.size(); i++) { // voor startsymbool
        string stateToString = createBracketState(this->StartState, this->StartStack, this->States[i]);
        cfg.P.emplace_back("S", vector<string>{stateToString});
    }
}

void PDA::makeRestProductions(CFG &cfg) {
    for (int t = 0; t < this->Transitions.size(); t++) { // voor de rest transities

        if (this->Transitions[t]["replacement"].empty()) { // bij poppen van stack (epsilon)
            string stateToString1 = createBracketState(this->Transitions[t]["from"], this->Transitions[t]["stacktop"], this->Transitions[t]["to"]);
            string symbol = this->Transitions[t]["input"];
            cfg.P.emplace_back(stateToString1, vector<string>{symbol});
        }
        else {

            for (int i = 0; i < this->States.size(); i++) { // combinatie van alle states (vanachter) bij head

                string stateToString1 = createBracketState(this->Transitions[t]["from"], this->Transitions[t]["stacktop"], this->States[i]); // dit is de head
                string stateToString2 = this->Transitions[t]["input"]; // inputsymbool

                string temp = this->Transitions[t]["to"]; // temp houdt het symbool bij dat vanachter bij de vorige bracket state werd gebruikt.
                                                          // Zo weten we bij de volgende bracket state wat onze 'begin state' is.
                                                          // We geven dit dan ook mee aan de functie makeBodies, waar we deze temp nodig hebben.

                vector<pair<string, vector<string>>> deelProductions; // pair van <temp, production body>, in deelProductions houden wij al onze production bodies bij.

                makeBodies(temp, deelProductions, t, i); // hier maken wij dan effectief onze production bodies

                addProductionsToCfg(cfg, deelProductions, stateToString1, stateToString2); // vervolgens voegen we de production bodies
                                                                                              // (opgeslagen in deelProductions) aan de productions van onze CFG toe.

            }
        }
    }
}

void PDA::makeBodies(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int i) { // minder dan 50 lijnen
    string stateToString3;
    for (int j = 0; j < this->Transitions[t]["replacement"].size(); j++) { // bij body, combinatie van alle stacksymbolen uit "replacements".
        pair<string, vector<string>> dP; // temporary container voor een temp string (laatste symbool bij vorige bracket) en een production body
        if (j == this->Transitions[t]["replacement"].size()-1) { // endbracket
            stateToString3 = createBracketState(temp, this->Transitions[t]["replacement"][j], this->States[i]); // eindstate = beginstate van eerste bracket
            if (j == 0) { // geval met maar ????n replacement symbol
                dP.first = temp;
                dP.second.push_back(stateToString3);
                deelProductions.emplace_back(dP.first, dP.second);
            }
            else { // we zitten in de laatste bracket state, deze wordt dus gewoon vanachter bij de body toegevoegd, temp moet niet meer worden bijgehouden bij de body.
                createBackBracket(temp, deelProductions, t, j, i);
            }
        }
        else {
            if (j == 0) { // beginbracket
                createBeginBracket(temp, deelProductions, t, j, dP); // j is eigenlijk 0 hier, maar we voegen de parameter toe omwille van gewoonte in de code.
            }
            else { // middenbracket
                createMiddleBracket(temp, deelProductions, t, j);
            }
        }
        stateToString3 = "";
    }
}

void PDA::addProductionsToCfg(CFG &cfg, const vector<pair<string, vector<string>>> &deelProductions, const string &stateToString1,
                              const string &stateToString2) {

    for (int a = 0; a < deelProductions.size(); a++) {
        vector<string> prod = {stateToString2}; // = ????n production: [...] => a [...] ... [...] met a = stateToString2.
        for (int b = 0; b < deelProductions[a].second.size(); b++) {
            prod.push_back(deelProductions[a].second[b]);
        }
        cfg.P.emplace_back(stateToString1, prod); // stateToString1 is de head.

    }
}

void PDA::createMiddleBracket(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int j) {
    vector<pair<string, vector<string>>> deelProdCopy; // temporary container gebruikt om de bracket state toe te voegen aan elke production body.
    for (int c = 0; c < deelProductions.size(); c++) {
        for (int k = 0; k < this->States.size(); k++)  {

            temp = deelProductions[c].first; // temp (vorige symbool) is telkens anders, daarom dat we dat eerst eruit moeten halen (we hebben dit namelijk o.a. hiervoor opgeslagen).
            string stateToString3 = createBracketState(temp, this->Transitions[t]["replacement"][j], this->States[k]);
            temp = this->States[k];

            vector<string> dp = deelProductions[c].second; // dp (deelproductie), bevat de (tot nu toe) complete production body (zie ook volgende lijn).
            dp.push_back(stateToString3);

            deelProdCopy.emplace_back(temp, dp);
        }
    }
    deelProductions = deelProdCopy;
}

void PDA::createBeginBracket(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int j,
                             pair<string, vector<string>> &dP) {

    for (int k = 0; k < this->States.size(); k++) {
        string stateToString3 = createBracketState(this->Transitions[t]["to"], this->Transitions[t]["replacement"][j], this->States[k]);
        temp = this->States[k];

        dP.first = temp;
        dP.second.push_back(stateToString3);
        deelProductions.emplace_back(dP.first, dP.second);
        dP.second.clear();
    }

}

void PDA::createBackBracket(string &temp, vector<pair<string, vector<string>>> &deelProductions, int t, int j, int i) {
    for (int k = 0; k < deelProductions.size(); k++) {
        temp = deelProductions[k].first;
        string stateToString3 = createBracketState(temp, this->Transitions[t]["replacement"][j], this->States[i]); // eindstate = beginstate van eerste bracket
        deelProductions[k].second.push_back(stateToString3); // production waar we aan toevoegen elke keer vanachter (sinds dit de laatste bracket state is in de body).
    }
}

