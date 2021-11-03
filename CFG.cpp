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
            P.emplace_back(p[i]["head"], vector<string>{""});
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

    cout << "S = " << S << endl;
}

bool CFG::accepts(const string &str) {

    vector<vector<vector<string>>> table;

    vector<int> maxLength; // hoogste aantal symbolen in een entry in de tabel per kolom
    for (int i = 0; i < str.size(); i++)  {
        maxLength.push_back(0);
    }

    vector<vector<string>> v;
    for (int i = 0; i < str.size(); i++) {
        vector<string> a = {string(1, str[i])};
        v.push_back(a);
    }
    table.push_back(v);


    for (int i = 0; i < str.size(); i++) {

        vector<vector<string>> layer;

        for (int j = 0; j < str.size()-i; j++) {

            vector<string> t;
            string str1 = str.substr(j, i+1);

            vector<string> X;
            if (i == 0) {
                t.push_back(str1);
                X = isProduction(t);
                layer.push_back(X);

            }

            else {
                string str2;
                string str3;

                vector<string> products;
                for (int k = 1; k < str1.size(); k++) {
                    str2 = str1.substr(0, k);
                    str3 = str1.substr(k, str1.size()-(k));

                    vector<string> x1; // eerste deelstring
                    if (str2.size() == 1) {
                        x1 = table[1][j];
                    }
                    else {
                        x1 = table[str2.size()][j];
                    }
                    vector<string> x2; // tweede deelstring
                    if (str3.size() == 1) {
                        x2 = table[1][j+i];
                    }
                    else {
                        x2 = table[str3.size()][j+str2.size()];
                    }

                    for (int i1 = 0; i1 < x1.size(); i1++) {
                        vector<string> product;
                        for (int i2 = 0; i2 < x2.size(); i2++) {
                            product.push_back(x1[i1]);
                            product.push_back(x2[i2]);

                            product = isProduction(product); // overwrite

                            for (int s = 0; s < product.size(); s++) {
                                if (! count(products.begin(), products.end(), product[s])) { // duplicaten niet toelaten
                                    products.push_back(product[s]);
                                }
                            }
                            product.clear();
                        }
                    }
                }
                X = products;
                layer.push_back(X);
                if (X.size() > maxLength[j]) {
                    maxLength[j] = X.size();
                }
            }
        }
        table.push_back(layer);
    }

    //// printing output part

    bool belongs = false;

    for (int i = 0; i < table.size(); i++) {

        for (int j = 0; j < table[i].size(); j++) {


        }
    }

    cout << belongs << endl;
    return belongs;
}

//| {A, C, S}  |
//| {}         | {A, C, S}  |
//| {}         | {B}        | {B}     |
//| {A, S}     | {B}        | {C, S}  | {A, S}  |
//| {B}        | {A, C}     | {A, C}  | {B}     | {A, C}  |
//true
//| {}      |
//| {}      | {A}  |
//| {C, S}  | {}   | {A, S}  |
//| {A, C}  | {B}  | {B}     | {A, C}  |
//false

vector<string> CFG::isProduction(const vector<string> &body) {

    vector<string> heads;
    for (int i = 0; i < this->P.size(); i++) {
        if (this->P[i].second == body) {
            heads.push_back(this->P[i].first);
        }
    }
    return heads;
}

CFG::CFG() = default;
