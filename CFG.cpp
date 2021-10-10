//
// Created by Maksim Karnaukh on 9/10/2021.
//

#include "CFG.h"

CFG::CFG(const string& filename) {

    ifstream input(filename);
    if (!input.is_open()) {
        cerr << "Error while reading file contents" << endl;
    }
    json j;
    input >> j;

    vector<string> v = j["Variables"];
    V = v;
    vector<string> t = j["Terminals"];
    T = t;

    auto p = j["Productions"];
    for (auto i = 0; i < p.size(); i++) {
        if (! p[i]["body"].empty()) {
            P.emplace_back(p[i]["head"], p[i]["body"]);
        }
        else {
            P.emplace_back(p[i]["head"], vector<string>{"e"});
        }
    }

    string s = j["Start"]; S = s;
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
