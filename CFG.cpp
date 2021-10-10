//
// Created by Maksim Karnaukh on 9/10/2021.
//

#include "CFG.h"

CFG::CFG() {

    V = {"BINDIGIT", "S"};

    T = {"0", "1", "a", "b"};

    P.emplace_back("S", vector<string>{""});
    P.emplace_back("S", vector<string>{"a", "S", "b", "BINDIGIT"});
    P.emplace_back("BINDIGIT", vector<string>{"0"});
    P.emplace_back("BINDIGIT", vector<string>{"1"});

    S = "S";
}

void CFG::print() {
    cout << "V = {";
    sort(V.begin(), V.end());
    for (int variable = 0; variable < V.size(); variable++) {
        if (variable != V.size()-1) {
            cout << V[variable] << ", ";
        }
        else {
            cout << V[variable] << "}" << endl;
        }
    }

    cout << "T = {";
    sort(T.begin(), T.end());
    for (int terminal = 0; terminal < T.size(); terminal++) {
        if (terminal != T.size()-1) {
            cout << T[terminal] << ", ";
        }
        else {
            cout << T[terminal] << "}" << endl;
        }
    }

    cout << "P = {" << endl;
    sort(P.begin(), P.end());
    for (auto prodHead = 0; prodHead < P.size(); prodHead++) {
        cout << "\t" << P[prodHead].first << " -> " << "`";
        for (auto prodBody = 0; prodBody < P[prodHead].second.size(); prodBody++) {
            cout << P[prodHead].second[prodBody];
            if (prodBody != P[prodHead].second.size()-1) {
                cout << " ";
            }
        }
        cout << "`" << endl;
    }
    cout << "}" << endl;

    cout << "S = " << S;
}
