#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;


vector<vector<int> > clauses;
set<int> vars;
map<int, int> alias;

void readBinaryClauses(){
        // Skip comments
        char c = cin.get();
        while (c == 'c') {
                while (c != '\n') c = cin.get();
                c = cin.get();
        }
        // Read "cnf numVars numClauses"
        uint numVars;
        uint numClauses;
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
                        clauses.push_back(clause);
                        for (auto l : clause) {
                                if (alias.find(l) == alias.end()) alias[l] = alias.size();
                                vars.insert(l);
                        }
                }
        }
}


int main() {
        readBinaryClauses();

        cout << vars.size() << '\n' << (2*clauses.size()) << '\n';
        for (int i = 0; i < clauses.size(); ++i) {
                cout << alias[clauses[i][0]] << ',' << alias[clauses[i][1]] << '\n';
                cout << alias[clauses[i][1]] << ',' << alias[clauses[i][0]];
                if (i < clauses.size()-1) cout << endl;
        }
}
