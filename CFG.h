//
// Created by Maksim Karnaukh on 9/10/2021.
//

#ifndef CFG1_CFG_H
#define CFG1_CFG_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class CFG {
public:
    vector<string> V;
    vector<string> T;
    vector<pair<string, vector<string>>> P;
    string S;

    CFG();

    CFG(const string& filename);

    void print();

    void toCNF();

    bool accepts(const string &str);

    vector<string> isProduction(const vector<string> &body);
};


#endif //CFG1_CFG_H
