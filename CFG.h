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

    CFG(); // default constructor

    CFG(const string& filename);

    // Functie print:
    // Printfunctie voor de CFG.
    void print();

    // Functie accepts:
    // Geeft terug of de meegegeven string behoort tot de taal van de grammatica, en print de tabel.
    bool accepts(const string &str);

    // Functie printTable:
    // Print de tabel voor het CYK algoritme.
    bool printTable(vector<vector<vector<string>>>& table, const vector<int>& maxLength) const;

    // Functie addBottomLayer
    // Voegt aan de container table als eerste layer de string zelf toe (één per één de aparte charachters).
    void addBottomLayer(vector<vector<vector<string>>> &table, const string &str);

    // Functie makeCrossProduct:
    // Hier doen we een cross product van deelstring x1 (opgeslagen als vector<string>)
    // en deelstring x2 (opgeslagen als vector<string>) en we stoppen het resultaat in de 'products' vector.
    void makeCrossProduct(const vector<string> &x1, const vector<string> &x2, vector<string> &products);

    // Functie giveProductsFromSubstrings:
    // Maakt substrings van de meegegeven str1 string, en berekent het product (cross-product) en stopt het resultaat in de products vector.
    // IndexI (= i) en IndexJ (= j) worden gebruikt om de juiste entry in de tabel te verkrijgen.
    void giveProductsFromSubstrings(vector<string> &products, const string &str1, const vector<vector<vector<string>>> &table, int indexI, int indexJ);

    // Functie isProduction:
    // Checkt of de meegegeven body ergens voorkomt in een production en geeft alle heads van deze productions terug.
    vector<string> isProduction(const vector<string> &body);
};


#endif //CFG1_CFG_H
