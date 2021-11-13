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

    addBottomLayer(table, str);

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

                if (X.size() > maxLength[j]) {
                    maxLength[j] = X.size();
                }
            }
            else {

                vector<string> products;
                giveProductsFromSubstrings(products, str1, table, i, j);

                X = products;
                layer.push_back(X);
                if (X.size() > maxLength[j]) {
                    maxLength[j] = X.size();
                }
            }
        }
        table.push_back(layer);
    }
    return printTable(table, maxLength);
}

vector<string> CFG::isProduction(const vector<string> &body) {

    vector<string> heads;
    for (int i = 0; i < this->P.size(); i++) {
        if (this->P[i].second == body) {
            heads.push_back(this->P[i].first);
        }
    }
    return heads;
}

bool CFG::printTable(vector<vector<vector<string>>>& table, const vector<int>& maxLength) const {

    bool belongs = false;
    for (int i = (int) table.size()-1; i > 0; i--) {
        if (i == table.size()-1) {
            if (count(table[i][0].begin(), table[i][0].end(), S)) {
                belongs = true;
            }
        }
        for (int j = 0; j < table[i].size(); j++) {
            int totalStringLength = maxLength[j]+(maxLength[j]-1)*2+1+2; // vanaf {
            int amountOfSpaces = 0; // vanaf }
            if (table[i][j].empty()) {
                amountOfSpaces = totalStringLength-1;
            }
            else {
                amountOfSpaces = totalStringLength-((int)table[i][j].size()+((int)table[i][j].size()-1)*2+1);
            }
            cout << "| ";
            cout << "{";
            sort(table[i][j].begin(), table[i][j].end());
            for (int k = 0; k < table[i][j].size(); k++) {
                if (table[i][j].empty()) {
                    cout << "";
                }
                else {
                    cout << table[i][j][k];
                    if (k != table[i][j].size()-1) {
                        cout << ", ";
                    }
                }
            }
            cout << "}";
            for (int k = 0; k < amountOfSpaces; k++) {
                cout << " ";
            }
        }
        cout << "|" << endl;
    }
    if (belongs) {
        cout << "true" << endl;
    }
    else {
        cout << "false" << endl;
    }
    return belongs;
}

void CFG::addBottomLayer(vector<vector<vector<string>>> &table, const string &str) {
    vector<vector<string>> v;
    for (int i = 0; i < str.size(); i++) {
        vector<string> a = {string(1, str[i])};
        v.push_back(a);
    }
    table.push_back(v);
}

void CFG::makeCrossProduct(const vector<string> &x1, const vector<string> &x2, vector<string> &products) {
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

void CFG::giveProductsFromSubstrings(vector<string> &products, const string &str1, const vector<vector<vector<string>>> &table, int i, int j) {
    for (int k = 1; k < str1.size(); k++) {
        string str2 = str1.substr(0, k);
        string str3 = str1.substr(k, str1.size()-(k));

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

        makeCrossProduct(x1, x2, products);
    }
}

void CFG::ll() {
//    algoritme hoofdzakelijk volgens https://www.geeksforgeeks.org/construction-of-ll1-parsing-table/

    // berekenen van FIRST
    map<string, set<string>> first;
    for (int i = 0; i < this->P.size(); i++) {
        auto it = first.find(this->P[i].first);

        if (it == first.end()) { // variabele is nog niet in de map
            vector<vector<string>> bodies = findBodiesOfVar(this->P[i].first);

            set<string> firstForSymbol;
            for (auto x = 0; x < bodies.size(); x++) {
                findFirst(firstForSymbol, bodies[x]);
            }
            first.insert(make_pair(this->P[i].first, firstForSymbol));
        }
    }

    // berekenen van FOLLOW
    map<string, set<string>> follow;

    // make table
}

void CFG::findFirst(set<string> &first, vector<string> &body) {
//    https://www.geeksforgeeks.org/first-set-in-syntax-analysis/
//    If x is a terminal, then FIRST(x) = { ‘x’ }
//    If x-> Є, is a production rule, then add Є to FIRST(x).
//    If X->Y1 Y2 Y3….Yn is a production,
//    FIRST(X) = FIRST(Y1)
//    If FIRST(Y1) contains Є then FIRST(X) = { FIRST(Y1) – Є } U { FIRST(Y2) }
//    If FIRST (Yi) contains Є for all i = 1 to n, then add Є to FIRST(X).

    if (body.size() == 1 && body[0].empty()) {
        first.insert(" ");
    }
    else {
        for (auto j = 0; j < body.size(); j++) {
            if (count(this->T.begin(), this->T.end(), body[j])) {
                first.insert(body[j]);
                break;
            }
            else {
                vector<vector<string>> bodiesVanVariabele = findBodiesOfVar(body[j]);
                set<string> firstTemp;
                for (auto x = 0; x < bodiesVanVariabele.size(); x++) {

                    if (bodiesVanVariabele[x][0] != body[0]) { // voorkomen van infinite recursion => {"head": "A", "body": ["A", "c"]},
                                                               //                                     {"head": "A", "body": ["A", "a", "d"]},...

                        findFirst(firstTemp, bodiesVanVariabele[x]);
                    }
                }

                bool epsilonPresent = false;
                for (auto it = firstTemp.begin(); it != firstTemp.end(); it++) {

                    if (*it == " ") {
                        epsilonPresent = true;
                    }
                    first.insert(*it);
                }
                if (! epsilonPresent) {
                    break;
                }
            }
        }
    }
}

vector<vector<string>> CFG::findBodiesOfVar(string &var) {

    vector<vector<string>> bodies;
    for (int i = 0; i < this->P.size(); i++) {
        if (var == this->P[i].first) {
            bodies.push_back(this->P[i].second);
        }
    }
    return bodies;
}

CFG::CFG() = default;
