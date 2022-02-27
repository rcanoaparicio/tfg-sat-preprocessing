#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <chrono>

using namespace std;

int numVars;
int numClauses;

typedef vector<vector<int> > Matrix;


bool contains(const vector<int> v, const int n) {
        int l = 0, r = v.size() - 1, m;
        while (l <= r) {
                m = l + (r - l) / 2;
                if (v[m] == n) return true;
                if (v[m] < n) l = m + 1;
                else r = m - 1;
        }
        return false;
}

void insert(vector<int> &v, const int n) {
        int l = 0, r = v.size() - 1, m;
        while (l <= r) {
                m = l + (r - l) / 2;
                if (v[m] == n) return;
                if (v[m] < n) l = m + 1;
                else r = m - 1;
        }
        v.insert(v.begin() + l, n);
}

int getIndex(int v) {
        return v + numVars;
}



typedef struct Constraint {
        int k;
        vector<int> vars;
} Constraint;




struct TrieNode;
typedef map<int, TrieNode> Trie;

typedef struct TrieNode {
        int varNumber;
        Trie next;
} TrieNode;


void insert(Trie &trie, const Constraint &constraint, const int constraint_idx) {
        if (constraint_idx == constraint.vars.size()) return;
        int num = constraint.vars[constraint_idx];
        if (trie.find(num) == trie.end()) {
                trie[num].varNumber = num;
        }

        insert(trie[num].next, constraint, constraint_idx + 1);
}

void insert(Trie &trie, const Constraint &constraint) {
        insert(trie, constraint, 0);
}


void toList(const Trie &trie,  vector<Constraint> &res, const Constraint &constraint) {
        if (trie.size() == 0) res.push_back(constraint);
        for (auto const &cit : trie) {
                Constraint new_constraint = constraint;
                new_constraint.vars.push_back(cit.second.varNumber);
                toList(cit.second.next, res, new_constraint);
        }
}

void toList(const Trie &trie, vector<Constraint> &res) {
        toList(trie, res, Constraint());
}


Matrix clauses;
unordered_set<int> set_vars;
vector<Constraint> constraints;

vector<Constraint> constraints1;

unordered_set<int> set_vars1;






void readClauses() {
        string s;
        while (cin >> s && s == "c") getline(cin, s);

        cin >> s >> numVars >> numClauses;

        clauses = Matrix(numClauses);
        constraints = vector<Constraint>(numClauses);
        int next;


        for (int i = 0; i < numClauses; ++i) {
                do {
                        cin >> next;
                        if (next != 0) {
                                clauses[i].push_back(next);
                                set_vars.insert(next);
                        }
                        sort(clauses[i].begin(), clauses[i].end());
                        constraints[i] = {1, clauses[i]};

                } while (next != 0);
        }

        for (int i = 0; i < constraints.size(); ++i) {
                if (constraints[i].vars.size() == 2) constraints1.push_back(constraints[i]);
        }

        for (int i = 0; i < constraints1.size(); ++i)
                for (const auto v : constraints1[i].vars) set_vars1.insert(v);


}



string toString(const Constraint c) {
        string r = "";
        for (int i = 0; i < c.vars.size(); ++i) {
                r += to_string(c.vars[i]) + " ";
                if (i < c.vars.size() - 1) r += "+ ";
                else r += ">= ";
        }
        r += to_string(c.k);
        return r;
}

vector<int> getIntersection(const vector<int> &a, const vector<int> &b) {
        vector<int> intersection;

        int i = 0, j = 0;
        while (i < a.size() and j < b.size()) {
                if (a[i] < b[j]) i++;
                else if (a[i] > b[j]) j++;
                else {
                        intersection.push_back(a[i++]);
                        j++;
                }
        }

        return intersection;
}



vector<int> getCandidateVariables(const Trie &trie, const vector<int> &vars, int idx, int left) {
        vector<int> res;

        if (left == 0) {
                for (auto const &cit : trie) {
                        res.push_back(cit.second.varNumber);
                }
                return res;
        }

        if (trie.find(vars[idx]) == trie.end()) return res;


        vector<int> next_candidates = getCandidateVariables(trie.at(vars[idx]).next, vars, idx+1, left-1);
        if (idx+left >= vars.size()) {
                return next_candidates;
        }
        vector<int> sibl_candidates = getCandidateVariables(trie, vars, idx+1, left);
        return getIntersection(next_candidates, sibl_candidates);

}

void learnConstraintsQ1() {

        vector<Constraint> consts = constraints1;
        int k = 1;

        while (consts.size() != 0) {
                cout << "K: " << k << endl;

                /*for (auto c : consts) {
                        cout << toString(c) << '\n';
                   }

                   cout << consts.size() << "\n\n";
                 */
                chrono::steady_clock::time_point begin = chrono::steady_clock::now();
                Trie trie;


                for (int i = 0; i < consts.size(); ++i) {
                        insert(trie, consts[i]);
                }

                vector<Constraint> next_consts;
                for (int i = 0; i < consts.size(); ++i) {
                        vector<int> candidate_vars = getCandidateVariables(trie, consts[i].vars, 0, k);
                        for (const int var : candidate_vars)  {
                                Constraint new_const = consts[i];
                                new_const.vars.push_back(var);
                                new_const.k++;
                                next_consts.push_back(new_const);
                        }
                }
                consts.clear();
                consts = next_consts;
                k++;
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                cout << consts.size() << " constraints." << endl;
                cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds> (end - begin).count() << "[ms]" << endl << endl;
        }
}




int main() {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout << "Reading clauses..." << endl;
        readClauses();


        learnConstraintsQ1();

}
