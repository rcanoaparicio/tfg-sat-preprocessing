#include <iostream>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;







map<int, unordered_set<int> > G;
vector<unordered_set<int> > maximalCliques;


vector<vector<int> > clauses;
unordered_set<int> vars;

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
                        for (auto l : clause) vars.insert(l);
                }
        }
}


void constructGraph() {
        for (int i = 0; i < clauses.size(); ++i) {
                G[clauses[i][0]].insert(clauses[i][1]);
                G[clauses[i][1]].insert(clauses[i][0]);
        }
}




unordered_set<int> get_intersection(unordered_set<int>::iterator as, unordered_set<int>::iterator ae,unordered_set<int>::iterator bs,unordered_set<int>::iterator be) {
        unordered_set<int> r;

        while (as != ae and bs != be) {
                if (*as == *bs) {
                        r.insert(*as);
                        ++as; ++bs;
                }
                else if (*as < *bs) ++as;
                else ++bs;
        }

        return r;
}

unordered_set<int> get_intersection2(const unordered_set<int> &a, const unordered_set<int> &b) {
        unordered_set<int> r;
        auto ae = a.cend();
        auto be = b.cend();

        if (a.size() < b.size()) {
                for (auto cit = a.cbegin(); cit != ae; ++cit)
                        if (b.find(*cit) != be) r.insert(*cit);
        }
        else {
                for (auto cit = b.cbegin(); cit != be; ++cit)
                        if (a.find(*cit) != ae) r.insert(*cit);
        }

        return r;
}


void BronKerbosch(unordered_set<int> R, unordered_set<int> P, unordered_set<int> X) {
        if (P.empty() and X.empty()) maximalCliques.push_back(R);

        unordered_set<int> P_cp = P;
        for (auto v : P_cp) {
                unordered_set<int> R2 = R; R2.insert(v);
                unordered_set<int> P2 = get_intersection2(P, G[v]);
                unordered_set<int> X2 = get_intersection2(X, G[v]);
                BronKerbosch(R2, P2, X2);
                P.erase(v);
                X.insert(v);
        }
}



int main() {
        cout << "Reading binary clauses" << endl;
        readBinaryClauses();


        cout << "Constructing the graph" << endl;
        constructGraph();


        cout << "Looking for all the maximal cliques" << endl;
        unordered_set<int> R, X;  // P = vars

        BronKerbosch(R, vars, X);

        cout << "Done!" << endl;
        cout << maximalCliques.size() << endl;

        cout << "\nList:\n";
        /*for (auto clique : maximalCliques) {
                for (int v : clique) {
                        cout << v << " ";
                }
                cout << endl;
           }
         */

}
