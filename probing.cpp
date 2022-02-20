#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
using namespace std;

#define UNDEF -1
#define TRUE 1
#define FALSE 0

uint numVars;
uint numClauses;
vector<vector<int> > clauses;
vector<int> model;
vector<int> modelStack;
uint indexOfNextLitToPropagate;
uint decisionLevel;

unordered_map<int, vector<int> > occurrences;
set<vector<int> > deducedClauses;



void readClauses( ){
        // Skip comments
        char c = cin.get();
        while (c == 'c') {
                while (c != '\n') c = cin.get();
                c = cin.get();
        }
        // Read "cnf numVars numClauses"
        string aux;
        cin >> aux >> numVars >> numClauses;

        // Read clauses
        for (uint i = 0; i < numClauses; ++i) {
                int lit;
                vector<int> clause;
                while (cin >> lit and lit != 0) {
                        clause.push_back(lit);
                }
                if (clause.size() == 2) {
                        for (int l : clause) occurrences[l].push_back(clauses.size());
                        clauses.push_back(clause);
                }
        }
}



int currentValueInModel(int lit){
        if (lit >= 0) return model[lit];
        else {
                if (model[-lit] == UNDEF) return UNDEF;
                else return 1 - model[-lit];
        }
}


void setLiteralToTrue(int lit){
        modelStack.push_back(lit);
        if (lit > 0) model[lit] = TRUE;
        else model[-lit] = FALSE;
}


void propagate() {
        while ( indexOfNextLitToPropagate < modelStack.size() ) {
                //std::cout << "while " << indexOfNextLitToPropagate << " of " << modelStack.size() << ": " << modelStack[indexOfNextLitToPropagate] << '\n';
                //std::cout << " > " << modelStack[indexOfNextLitToPropagate] << " of " << occurrences.size() << '\n';
                vector<int> clauses_idxs = occurrences[-modelStack[indexOfNextLitToPropagate]];
                int len = clauses_idxs.size();
                for (uint i = 0; i < len; ++i) {
                        //std::cout << "for " << i << " of " << len << ": " << clauses_idxs[i] << " of " << clauses.size() << '\n';
                        int idx = clauses_idxs[i];
                        int val1 = currentValueInModel(clauses[idx][0]);
                        int val2 = currentValueInModel(clauses[idx][1]);
                        if (val1 == FALSE and val2 == UNDEF) {
                                setLiteralToTrue(clauses[idx][1]);
                        }
                        else if (val2 == FALSE and val1 == UNDEF) {
                                setLiteralToTrue(clauses[idx][0]);
                        }
                }
                ++indexOfNextLitToPropagate;
        }
}

void backtrack() {
        indexOfNextLitToPropagate = 0;
        for (int i = 0; i < modelStack.size(); i++) model[abs(modelStack[i])] = UNDEF;
        modelStack.clear();
}

void deduceClauses() {
        int lit = -modelStack[0];
        for (int i = 1; i < modelStack.size(); i++) {
                if (modelStack[i] < lit) deducedClauses.insert({modelStack[i], lit});
                else deducedClauses.insert({lit, modelStack[i]});
        }
}

int main(){
        readClauses();
        model.resize(numVars+1,UNDEF);
        indexOfNextLitToPropagate = 0;
        decisionLevel = 0;

        numClauses = clauses.size();

        for (int i = 1; i <= numVars; i++) {
                setLiteralToTrue(i);
                propagate();
                deduceClauses();
                backtrack();

                setLiteralToTrue(-i);
                propagate();
                deduceClauses();
                backtrack();
        }

        cout << "p cnf " << numVars << " " << deducedClauses.size()<< '\n';
        for (const auto clause : deducedClauses) {
                for (const int l : clause) cout << l << " ";
                cout << "0\n";
        }

}
