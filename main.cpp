#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <map>


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
        std::vector<int> vars;

        int getQ() const {
                return vars.size() - k;
        }

        bool operator<(const Constraint& a) const
        {
                int q = getQ();
                int aq = a.getQ();
                return q != aq ? (q < aq) : (k < a.k);
        }

        bool operator==(const Constraint& a) const
        {
                if (a.k != k || a.vars.size() != vars.size()) return false;
                for (int i = 0; i < vars.size(); ++i) if (vars[i] != a.vars[i]) return false;
                return true;
        }
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
vector<Constraint> constraints2;

unordered_set<int> set_vars1;
unordered_set<int> set_vars2;

int q1i = 0;
int q1f = 0;
int q2i = 0;
int q2f = 0;






void readClauses() {
        std::string s;
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

        sort(constraints.begin(), constraints.end());

        for (int i = 0; i < constraints.size() && q1f == 0; ++i) {
                if (constraints[i].getQ() == 1) constraints1.push_back(constraints[i]);
                else if (constraints[i].getQ() == 2) constraints2.push_back(constraints[i]);
                else if (constraints[i].getQ() > 2) break;
        }

        for (int i = 0; i < q1i; ++i)
                for (const auto v : constraints1[i].vars) set_vars1.insert(v);

        for (int i = 0; i < constraints2.size(); ++i)
                for (const auto v : constraints2[i].vars) set_vars2.insert(v);

}



std::string toString(const Constraint c) {
        std::string r = "";
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
        int q = 1;
        while (consts.size() != 0) {
                cout << "Q: " << q << endl;
                /*
                   for (auto c : consts) {
                        cout << toString(c) << endl;
                   }
                   cout << endl;
                 */
                Trie trie;
                for (int i = 0; i < consts.size(); ++i) insert(trie, consts[i]);

                vector<Constraint> next_consts;
                for (int i = 0; i < consts.size(); ++i) {
                        vector<int> candidate_vars = getCandidateVariables(trie, consts[i].vars, 0, q);
                        for (const int var : candidate_vars)  {
                                Constraint new_const = consts[i];
                                new_const.vars.push_back(var);
                                new_const.k++;
                                next_consts.push_back(new_const);
                        }
                }
                consts = next_consts;
                q++;
        }
}







int main() {

        cout << "Reading clauses..." << endl;
        readClauses();


        learnConstraintsQ1();

}
