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

    for (int i = 0; i < this->States.size(); i++) { // variabelen aanmaken
        for (int j = 0; j < this->States.size(); j++) {
            for (int symbol = 0; symbol < this->StackAlphabet.size(); symbol++) {
                string stateToString = createBracketState(this->States[i], this->StackAlphabet[symbol], this->States[j]);
                cfg.V.push_back(stateToString);
            }
        }
    }

    for (int i = 0; i < this->States.size(); i++) { // voor startsymbool
        string stateToString = createBracketState(this->StartState, this->StartStack, this->States[i]);
        cfg.P.emplace_back("S", vector<string>{stateToString});
    }

    for (int t = 0; t < this->Transitions.size(); t++) { // voor de rest transities

        if (this->Transitions[t]["replacement"].empty()) { // bij poppen van stack (epsilon)
            string stateToString1 = createBracketState(this->Transitions[t]["from"], this->Transitions[t]["stacktop"], this->Transitions[t]["to"]);
            string symbol = this->Transitions[t]["input"];
            cfg.P.emplace_back(stateToString1, vector<string>{symbol});
        }

        else {

            for (int i = 0; i < this->States.size(); i++) { // combinatie van alle states (vanachter) bij head

                string stateToString1 = createBracketState(this->Transitions[t]["from"], this->Transitions[t]["stacktop"], this->States[i]);

                string stateToString2 = this->Transitions[t]["input"]; // inputsymbool

                string stateToString3;

                string temp = this->Transitions[t]["to"];

                vector<pair<string, vector<string>>> deelProductions;

                for (int j = 0; j < this->Transitions[t]["replacement"].size(); j++) { // bij body, combinatie van alle stacksymbolen uit "replacements".

                    pair<string, vector<string>> dP;
                    if (j == this->Transitions[t]["replacement"].size()-1) { // endbracket
                        stateToString3 = createBracketState(temp, this->Transitions[t]["replacement"][j], this->States[i]); // eindstate = beginstate van eerste bracket
                        if (j == 0) { // geval met maar één replacement symbol
                            dP.first = temp;
                            dP.second.push_back(stateToString3);
                            deelProductions.emplace_back(dP.first, dP.second);
                        }
                        else {
                            for (int k = 0; k < deelProductions.size(); k++) {
                                temp = deelProductions[k].first;
                                stateToString3 = createBracketState(temp, this->Transitions[t]["replacement"][j], this->States[i]); // eindstate = beginstate van eerste bracket

                                deelProductions[k].second.push_back(stateToString3); // production waar we aan toevoegen elke keer

                            }
                        }
                    }
                    else {

                        if (j == 0) { // beginbracket
                            for (int k = 0; k < this->States.size(); k++) {
                                stateToString3 = createBracketState(this->Transitions[t]["to"], this->Transitions[t]["replacement"][j], this->States[k]);
                                temp = this->States[k];

                                dP.first = temp;
                                dP.second.push_back(stateToString3);
                                deelProductions.emplace_back(dP.first, dP.second);
                                dP.second.clear(); ////
                            }
                        }
                        else { // middenbracket

//                                temp = deelProductions[k].first; ////
//                                stateToString3 = createBracketState(temp, this->Transitions[t]["replacement"][j], this->States[k]);
//                                temp = this->States[k];

//                                deelProductions[k].first = temp; // k ?
//                                deelProductions[k].second.push_back(stateToString3); // production waar we aan toevoegen elke keer

//                                for (int l = 0; l < this->States.size(); l++) {
//                                    if (l != k) {
//                                        deelProductions.emplace_back(deelProductions[k].first, deelProductions[k].second);
//                                        stateToString3 = createBracketState(temp, this->Transitions[t]["replacement"][j], this->States[l]);
//                                        temp = this->States[l];
//
//                                    }
//                                }

                            vector<pair<string, vector<string>>> deelProdCopy;
                            for (int c = 0; c < deelProductions.size(); c++) {
//                                vector<pair<string, vector<string>>> deelProdCopy;
                                for (int k = 0; k < this->States.size(); k++)  {

                                    temp = deelProductions[c].first;
                                    stateToString3 = createBracketState(temp, this->Transitions[t]["replacement"][j], this->States[k]);
                                    temp = this->States[k];

                                    vector<string> dp = deelProductions[c].second;
                                    dp.push_back(stateToString3);

                                    deelProdCopy.emplace_back(temp, dp);
//                                    deelProductions = deelProdCopy;
                                }
                            }
                            deelProductions = deelProdCopy;
                        }
                    }
//                    prod.push_back(stateToString3);
                    stateToString3 = "";
                }
                for (int a = 0; a < deelProductions.size(); a++) {
                    vector<string> prod = {stateToString2}; // = één production [...] => a [...] ... [...] met a = stateToString2.
                    for (int b = 0; b < deelProductions[a].second.size(); b++) {
                        prod.push_back(deelProductions[a].second[b]);
                    }
                    cfg.P.emplace_back(stateToString1, prod);

                }
            }
        }
    }
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
