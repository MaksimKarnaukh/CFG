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

    for (int i = 0; i < this->States.size(); i++) {
        for (int j = 0; j < this->States.size(); j++) {
            for (int symbol = 0; symbol < this->StackAlphabet.size(); symbol++) {
                string stateToString = "[" + this->States[i] + this->StackAlphabet[symbol] + this->States[j] + "]";
                cfg.V.push_back(stateToString);
            }
        }
    }

    for (int i = 0; i < this->States.size(); i++) { // voor startsymbool
        string stateToString = "[" + this->StartState + this->StartStack + this->States[i] + "]";
        cfg.P.emplace_back("S", vector<string>{stateToString});
    }

    for (int t = 0; t < this->Transitions.size(); t++) { // voor de rest transities

        if (this->Transitions[t]["replacement"].empty()) { // bij poppen van stack (epsilon)
            string stateToString1 = "[";
            stateToString1 += this->Transitions[t]["from"];
            stateToString1 += this->Transitions[t]["stacktop"];
            stateToString1 += this->Transitions[t]["to"];
            stateToString1 += "]";
            string symbol = this->Transitions[t]["input"];

            cfg.P.emplace_back(stateToString1, vector<string>{symbol});
        }
        else {
            int nrOfIterations = 1;
            if (this->Transitions[t]["replacement"].size() > 1) {
                nrOfIterations = this->States.size();
            }

            for (int i = 0; i < nrOfIterations; i++) { // combinatie van alle states (vanachter) bij head

                for (int k = 0; k < this->States.size(); k++) { // combinatie van alle states (vanachter) bij body
                    string stateToString1 = "[" + to_string(this->Transitions[t]["from"]) + to_string(this->Transitions[t]["stacktop"]) + this->States[k] + "]"; // head
                    string stateToString2 = to_string(this->Transitions[t]["input"]); // inputsymbool
                    string stateToString3;
                    vector<string> prod = {stateToString2};

                    string temp;

                    for (int j = 0; j < this->Transitions[t]["replacement"].size(); j++) { // bij body, combinatie van alle stacksymbolen uit "replacements".
                        if (j == 0) {
                            stateToString3 += "[" + to_string(this->Transitions[t]["to"]) + to_string(this->Transitions[t]["replacement"][j]) + this->States[k] + "]";
                            temp = this->States[k];
                        }
                        else if (j == this->Transitions[t]["replacement"].size()-1) {
                            stateToString3 += "[" + temp + to_string(this->Transitions[t]["replacement"][j]) + this->States[k] + "]"; // vanachter k ipv i

                        }
                        else {
                            stateToString3 += "[" + temp + to_string(this->Transitions[t]["replacement"][j]) + this->States[i] + "]"; // k nr i
                            temp = this->States[i]; // ook
                        }
                        prod.push_back(stateToString3);
                        stateToString3 = "";
                    }
                    cfg.P.emplace_back(stateToString1, prod);
                }
            }
        }
    }

    return cfg;
}
