#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <random>

using namespace std;

//  CONSTANTS

const int    INF     = numeric_limits<int>::max();
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string CYAN    = "\033[36m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string RED     = "\033[31m";
const string DIM     = "\033[2m";

//  DATA STRUCTURES

struct User {
    int    id;
    string name;
    string location;
    int    age;
    string interest;
    int    followers;
};

struct Edge {
    int to;
    int weight;
};

//  GRAPH CLASS

class SocialGraph {
public:
    unordered_map<int, User>         users;
    unordered_map<int, vector<Edge>> adj;
    unordered_map<string, int>       nameToId;
    int edgeCount = 0;

    void addUser(const User& u) {
        users[u.id]      = u;
        adj[u.id];
        nameToId[u.name] = u.id;
    }

    void addEdge(int u, int v, int w = 1) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        edgeCount++;
    }

    bool hasUser(int id)          const { return users.count(id); }
    bool hasUser(const string& n) const { return nameToId.count(n); }
    int  getId  (const string& n) const { return nameToId.at(n); }

    int degree(int id) const {
        auto it = adj.find(id);
        return it == adj.end() ? 0 : (int)it->second.size();
    }

    vector<int> neighbors(int id) const {
        vector<int> res;
        auto it = adj.find(id);
        if (it != adj.end())
            for (int i = 0; i < (int)it->second.size(); i++)
                res.push_back(it->second[i].to);
        return res;
    }

    bool connected(int u, int v) const {
        auto it = adj.find(u);
        if (it == adj.end()) return false;
        for (int i = 0; i < (int)it->second.size(); i++)
            if (it->second[i].to == v) return true;
        return false;
    }

    // ── BFS ─────────────────────────────────
    unordered_map<int,int> bfs(int src) const {
        unordered_map<int,int> dist;
        queue<int> q;
        dist[src] = 0;
        q.push(src);
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            vector<int> nbs = neighbors(cur);
            for (int i = 0; i < (int)nbs.size(); i++) {
                if (!dist.count(nbs[i])) {
                    dist[nbs[i]] = dist[cur] + 1;
                    q.push(nbs[i]);
                }
            }
        }
        return dist;
    }

    vector<int> bfsPath(int src, int dst) const {
        unordered_map<int,int> parent;
        parent[src] = -1;
        queue<int> q;
        q.push(src);
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            if (cur == dst) break;
            vector<int> nbs = neighbors(cur);
            for (int i = 0; i < (int)nbs.size(); i++) {
                if (!parent.count(nbs[i])) {
                    parent[nbs[i]] = cur;
                    q.push(nbs[i]);
                }
            }
        }
        if (!parent.count(dst)) return {};
        vector<int> path;
        for (int v = dst; v != -1; v = parent[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    }

    // ── DFS ─────────────────────────────────
    void dfsHelper(int cur, unordered_set<int>& vis, vector<int>& order) const {
        vis.insert(cur);
        order.push_back(cur);
        vector<int> nbs = neighbors(cur);
        for (int i = 0; i < (int)nbs.size(); i++)
            if (!vis.count(nbs[i]))
                dfsHelper(nbs[i], vis, order);
    }

    vector<int> dfs(int src) const {
        unordered_set<int> vis;
        vector<int> order;
        dfsHelper(src, vis, order);
        return order;
    }

    // ── DIJKSTRA ────────────────────────────
    pair<unordered_map<int,int>, unordered_map<int,int>>
    dijkstra(int src) const {
        unordered_map<int,int> dist, parent;
        for (auto it = users.begin(); it != users.end(); ++it) {
            dist[it->first]   = INF;
            parent[it->first] = -1;
        }
        dist[src] = 0;

        priority_queue<pair<int,int>,
                       vector<pair<int,int>>,
                       greater<pair<int,int>>> pq;
        pq.push(make_pair(0, src));

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            if (d > dist[u]) continue;
            auto it = adj.find(u);
            if (it == adj.end()) continue;
            for (int i = 0; i < (int)it->second.size(); i++) {
                int to = it->second[i].to;
                int nd = d + it->second[i].weight;
                if (nd < dist[to]) {
                    dist[to]   = nd;
                    parent[to] = u;
                    pq.push(make_pair(nd, to));
                }
            }
        }
        return make_pair(dist, parent);
    }

    vector<int> dijkstraPath(int src, int dst) const {
        pair<unordered_map<int,int>,
             unordered_map<int,int>> result = dijkstra(src);
        unordered_map<int,int> dist   = result.first;
        unordered_map<int,int> parent = result.second;
        if (dist[dst] == INF) return {};
        vector<int> path;
        for (int v = dst; v != -1; v = parent[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    }

    // ── MUTUALS ─────────────────────────────
    vector<int> mutuals(int u, int v) const {
        unordered_set<int> nu;
        vector<int> nu_list = neighbors(u);
        for (int i = 0; i < (int)nu_list.size(); i++) nu.insert(nu_list[i]);
        vector<int> result;
        vector<int> nv_list = neighbors(v);
        for (int i = 0; i < (int)nv_list.size(); i++)
            if (nu.count(nv_list[i]) && nv_list[i] != u && nv_list[i] != v)
                result.push_back(nv_list[i]);
        return result;
    }

    // ── SUGGESTIONS ─────────────────────────
    vector<pair<int,int>> suggestions(int uid, int topN = 5) const {
        unordered_set<int> direct;
        vector<int> nb_list = neighbors(uid);
        for (int i = 0; i < (int)nb_list.size(); i++) direct.insert(nb_list[i]);
        direct.insert(uid);

        unordered_map<int,int> score;
        for (int i = 0; i < (int)nb_list.size(); i++) {
            vector<int> fof = neighbors(nb_list[i]);
            for (int j = 0; j < (int)fof.size(); j++)
                if (!direct.count(fof[j]))
                    score[fof[j]]++;
        }
        vector<pair<int,int>> ranked(score.begin(), score.end());
        sort(ranked.begin(), ranked.end(),
             [](const pair<int,int>& a, const pair<int,int>& b){
                 return a.second > b.second;
             });
        if ((int)ranked.size() > topN) ranked.resize(topN);
        return ranked;
    }

    // ── INFLUENCER SCORE ────────────────────
    double influencerScore(int uid) const {
        int deg = degree(uid);
        unordered_map<int,int> dist = bfs(uid);
        int reach2 = 0;
        for (auto it = dist.begin(); it != dist.end(); ++it)
            if (it->second <= 2 && it->first != uid) reach2++;
        int fol = users.at(uid).followers;
        return deg * 0.4 + reach2 * 0.4 + (fol / 100.0) * 0.2;
    }

    // ── TOP INFLUENCERS ─────────────────────
    vector<pair<int,double>> topInfluencers(int topN = 10) const {
        vector<pair<int,double>> scores;
        for (auto it = users.begin(); it != users.end(); ++it)
            scores.push_back(make_pair(it->first, influencerScore(it->first)));
        sort(scores.begin(), scores.end(),
             [](const pair<int,double>& a, const pair<int,double>& b){
                 return a.second > b.second;
             });
        if ((int)scores.size() > topN) scores.resize(topN);
        return scores;
    }

    // ── COMPONENTS ──────────────────────────
    vector<vector<int>> components() const {
        unordered_set<int> vis;
        vector<vector<int>> comps;
        for (auto it = users.begin(); it != users.end(); ++it) {
            int id = it->first;
            if (!vis.count(id)) {
                vector<int> comp;
                stack<int> st;
                st.push(id);
                while (!st.empty()) {
                    int cur = st.top(); st.pop();
                    if (vis.count(cur)) continue;
                    vis.insert(cur);
                    comp.push_back(cur);
                    vector<int> nbs = neighbors(cur);
                    for (int i = 0; i < (int)nbs.size(); i++) st.push(nbs[i]);
                }
                comps.push_back(comp);
            }
        }
        sort(comps.begin(), comps.end(),
             [](const vector<int>& a, const vector<int>& b){
                 return a.size() > b.size();
             });
        return comps;
    }

    // ── STATS ───────────────────────────────
    void printStats() const {
        int n = (int)users.size();
        int m = edgeCount;
        double density = n > 1 ? (2.0 * m) / ((double)n * (n - 1)) : 0;
        double avgDeg  = n ? (2.0 * m / n) : 0;

        int maxDeg = 0, maxNode = -1;
        for (auto it = users.begin(); it != users.end(); ++it) {
            int d = degree(it->first);
            if (d > maxDeg) { maxDeg = d; maxNode = it->first; }
        }

        cout << GREEN << BOLD
             << "\n  Graph Statistics\n" << RESET
             << DIM << "  ─────────────────────────────────────\n" << RESET
             << "  Users (nodes)   : " << BOLD << n     << RESET << "\n"
             << "  Connections     : " << BOLD << m     << RESET << "\n"
             << "  Graph density   : " << BOLD << fixed << setprecision(4) << density << RESET << "\n"
             << "  Avg degree      : " << BOLD << fixed << setprecision(2) << avgDeg  << RESET << "\n";
        if (maxNode != -1)
            cout << "  Most connected  : " << BOLD << CYAN
                 << users.at(maxNode).name << RESET
                 << "  (degree " << maxDeg << ")\n";

        vector<vector<int>> comps = components();
        cout << "  Components      : " << BOLD << comps.size() << RESET << "\n"
             << "  Largest comp.   : " << BOLD << comps[0].size()
             << " users" << RESET << "\n";
    }
};

//  NAME SEARCH HELPERS

string toLower(const string& s) {
    string res = s;
    for (int i = 0; i < (int)res.size(); i++)
        res[i] = tolower(res[i]);
    return res;
}

// Returns all user IDs whose name contains the query (case-insensitive)
vector<int> findUserByName(const SocialGraph& g, const string& query) {
    string q = toLower(query);
    vector<int> matches;
    for (auto it = g.users.begin(); it != g.users.end(); ++it)
        if (toLower(it->second.name).find(q) != string::npos)
            matches.push_back(it->first);
    // sort by name for consistent display
    sort(matches.begin(), matches.end(),
         [&g](int a, int b){
             return g.users.at(a).name < g.users.at(b).name;
         });
    return matches;
}

int resolveUser(const SocialGraph& g, const string& input) {
    // Try numeric ID
    try {
        int id = stoi(input);
        if (g.hasUser(id)) return id;
        cout << RED << "  No user with ID " << id << ".\n" << RESET;
        return -1;
    } catch (...) {}

    // Exact name match
    if (g.hasUser(input)) return g.getId(input);

    // Partial case-insensitive search
    vector<int> matches = findUserByName(g, input);

    if (matches.empty()) {
        cout << RED << "  No user found matching \""
             << input << "\".\n" << RESET;
        return -1;
    }

   
    if ((int)matches.size() == 1) {
        cout << GREEN << "  Found: " << g.users.at(matches[0]).name
             << "  (ID: " << matches[0] << ")\n" << RESET;
        return matches[0];
    }

    // Multiple matches — show list and ask user to pick
    cout << YELLOW << "  " << matches.size()
         << " users match \"" << input << "\":\n" << RESET;
    int limit = (int)min((int)matches.size(), 10);
    for (int i = 0; i < limit; i++) {
        const User& u = g.users.at(matches[i]);
        cout << "    " << CYAN << i+1 << ". "
             << left << setw(22) << u.name << RESET
             << "  ID: "       << setw(5) << u.id
             << "  Age: "      << setw(4) << u.age
             << "  ["          << u.location << " | " << u.interest << "]\n";
    }
    if ((int)matches.size() > 10)
        cout << DIM << "  ...and " << matches.size()-10
             << " more. Narrow your search.\n" << RESET;

    cout << "  Pick (1-" << limit << ") or 0 to cancel: ";
    string pick;
    getline(cin, pick);
    int idx = -1;
    try { idx = stoi(pick); } catch (...) {}
    if (idx < 1 || idx > limit) {
        cout << DIM << "  Cancelled.\n" << RESET;
        return -1;
    }
    return matches[idx - 1];
}

//  FILE I/O

bool loadUsers(SocialGraph& g, const string& path) {
    ifstream f(path);
    if (!f.is_open()) return false;
    string line;
    getline(f, line);   // skip header
    int count = 0;
    while (getline(f, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string tok;
        User u;
        try {
            getline(ss, tok, ','); u.id        = stoi(tok);
            getline(ss, tok, ','); u.name      = tok;
            getline(ss, tok, ','); u.location  = tok;
            getline(ss, tok, ','); u.age       = stoi(tok);
            getline(ss, tok, ','); u.interest  = tok;
            getline(ss, tok, ','); u.followers = stoi(tok);
            g.addUser(u);
            count++;
        } catch (...) {}
    }
    cout << GREEN << "  [OK] Loaded " << count
         << " users from " << path << RESET << "\n";
    return true;
}

bool loadEdges(SocialGraph& g, const string& path) {
    ifstream f(path);
    if (!f.is_open()) return false;
    string line;
    getline(f, line);   // skip header
    int count = 0;
    while (getline(f, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string tok;
        try {
            int u, v, w = 1;
            getline(ss, tok, ','); u = stoi(tok);
            getline(ss, tok, ','); v = stoi(tok);
            if (getline(ss, tok, ',')) w = stoi(tok);
            if (g.hasUser(u) && g.hasUser(v)) {
                g.addEdge(u, v, w);
                count++;
            }
        } catch (...) {}
    }
    cout << GREEN << "  [OK] Loaded " << count
         << " edges from " << path << RESET << "\n";
    return true;
}

void saveReport(const SocialGraph& g, const string& path = "report.txt") {
    ofstream f(path);
    if (!f.is_open()) {
        cout << RED << "  [X] Cannot write report\n" << RESET;
        return;
    }

    f << "============================================================\n";
    f << "  Social Network Analysis Report\n";
    f << "  Generated: " << __DATE__ << " " << __TIME__ << "\n";
    f << "============================================================\n\n";

    f << "GRAPH SUMMARY\n";
    f << "  Users       : " << g.users.size() << "\n";
    f << "  Connections : " << g.edgeCount    << "\n\n";

    f << "TOP 10 INFLUENCERS\n";
    f << left << setw(5)  << "Rank"
              << setw(20) << "Name"
              << setw(12) << "Score"
              << setw(10) << "Degree"
              << "Followers\n";
    f << string(55, '-') << "\n";

    int rank = 1;
    vector<pair<int,double>> top = g.topInfluencers(10);
    for (int i = 0; i < (int)top.size(); i++) {
        int id        = top[i].first;
        double score  = top[i].second;
        const User& u = g.users.at(id);
        f << left << setw(5)  << rank++
                  << setw(20) << u.name
                  << setw(12) << fixed << setprecision(2) << score
                  << setw(10) << g.degree(id)
                  << u.followers << "\n";
    }

    f << "\nCONNECTED COMPONENTS\n";
    vector<vector<int>> comps = g.components();
    f << "  Total: " << comps.size() << "\n";
    int limit = (int)min((int)comps.size(), 5);
    for (int i = 0; i < limit; i++) {
        f << "  Component " << i+1
          << " (" << comps[i].size() << " users): ";
        int show = (int)min((int)comps[i].size(), 5);
        for (int j = 0; j < show; j++)
            f << g.users.at(comps[i][j]).name
              << (j+1 < show ? ", " : "");
        if ((int)comps[i].size() > 5) f << " ...";
        f << "\n";
    }
    f.close();
    cout << GREEN << "  [OK] Report saved -> " << path << RESET << "\n";
}

//  SAMPLE DATA GENERATOR

void generateSampleData(int numUsers = 520, int numEdges = 1800,
                        const string& uFile = "users.csv",
                        const string& eFile = "edges.csv") {
    vector<string> firstNames = {
        "Aarav","Aisha","Arjun","Priya","Rahul","Sneha","Vikram","Ananya",
        "Rohan","Divya","Karan","Meera","Amit","Pooja","Raj","Nisha",
        "Dev","Kavya","Siddharth","Riya","Harsh","Simran","Nikhil","Tanya",
        "Varun","Shruti","Aditya","Neha","Yash","Puja","Akash","Swati",
        "Ravi","Preeti","Ajay","Anjali","Suresh","Rekha","Deepak","Sunita",
        "Manish","Komal","Gaurav","Pallavi","Kunal","Monika","Vishal","Shweta"
    };
    vector<string> cities = {
        "Mumbai","Delhi","Jodhpur","Bangalore","Chennai",
        "Hyderabad","Kolkata","Pune","Ahmedabad","Jaipur"
    };
    vector<string> interests = {
        "Technology","Music","Sports","Travel","Food",
        "Art","Gaming","Finance","Fashion","Science"
    };

    mt19937 rng(42);

    ofstream uf(uFile);
    uf << "id,name,location,age,interest,followers\n";
    for (int i = 1; i <= numUsers; i++) {
        string name = firstNames[rng() % firstNames.size()]
                    + "_" + to_string(i);
        uf << i << ","
           << name << ","
           << cities[rng() % cities.size()] << ","
           << (18 + rng() % 38) << ","
           << interests[rng() % interests.size()] << ","
           << (50 + rng() % 4951) << "\n";
    }
    uf.close();

    set<pair<int,int>> seen;
    ofstream ef(eFile);
    ef << "from_id,to_id,weight\n";
    int written = 0;

    for (int i = 1; i <= numUsers && written < numEdges; i++) {
        int links = 2 + rng() % 5;
        for (int k = 0; k < links && written < numEdges; k++) {
            int j = 1 + rng() % numUsers;
            if (j == i) continue;
            pair<int,int> key = make_pair(min(i,j), max(i,j));
            if (!seen.count(key)) {
                seen.insert(key);
                ef << i << "," << j << ","
                   << (1 + rng() % 10) << "\n";
                written++;
            }
        }
    }
    while (written < numEdges) {
        int i = 1 + rng() % numUsers;
        int j = 1 + rng() % numUsers;
        if (i == j) continue;
        pair<int,int> key = make_pair(min(i,j), max(i,j));
        if (!seen.count(key)) {
            seen.insert(key);
            ef << i << "," << j << ","
               << (1 + rng() % 10) << "\n";
            written++;
        }
    }
    ef.close();

    cout << GREEN
         << "  [OK] Generated " << uFile
         << " (" << numUsers << " users)\n"
         << "  [OK] Generated " << eFile
         << " (" << written  << " edges)\n"
         << RESET;
}

//  DISPLAY HELPERS

void printUser(const SocialGraph& g, int id) {
    if (!g.hasUser(id)) {
        cout << RED << "  User not found.\n" << RESET;
        return;
    }
    const User& u = g.users.at(id);
    cout << CYAN << BOLD << "  " << u.name << RESET
         << "  (ID: " << id << ")\n"
         << DIM
         << "  Location  : " << u.location  << "\n"
         << "  Age       : " << u.age       << "\n"
         << "  Interest  : " << u.interest  << "\n"
         << "  Followers : " << u.followers << "\n"
         << "  Degree    : " << g.degree(id) << " connections\n"
         << RESET;
}

void printPath(const SocialGraph& g, const vector<int>& path) {
    if (path.empty()) {
        cout << RED << "  No path found.\n" << RESET;
        return;
    }
    cout << "  ";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << CYAN << g.users.at(path[i]).name << RESET;
        if (i + 1 < (int)path.size())
            cout << YELLOW << " -> " << RESET;
    }
    cout << "\n  Hops: " << BOLD << path.size() - 1 << RESET << "\n";
}

void banner() {
    cout << CYAN << BOLD << "\n"
         << "  +==================================================+\n"
         << "  |   Social Media Network Analyser  v1.0           |\n"
         << "  |   C++ | Graph Theory | BFS | DFS | Dijkstra    |\n"
         << "  +==================================================+\n"
         << RESET;
}

void menu() {
    cout << BOLD
         << "\n  +-- MENU ------------------------------------------+\n"
         << RESET
         << "  |  1.  Search user profile                        |\n"
         << "  |  2.  List all friends of a user                 |\n"
         << "  |  3.  Find mutual friends (two users)            |\n"
         << "  |  4.  BFS  -- connections by degree              |\n"
         << "  |  5.  DFS  -- deep network traversal             |\n"
         << "  |  6.  Shortest path -- BFS (hop count)           |\n"
         << "  |  7.  Shortest path -- Dijkstra (weighted)       |\n"
         << "  |  8.  Top influencers                            |\n"
         << "  |  9.  Friend suggestions                         |\n"
         << "  |  10. Community / component analysis             |\n"
         << "  |  11. Graph statistics                           |\n"
         << "  |  12. Save analysis report                       |\n"
         << "  |  0.  Exit                                       |\n"
         << BOLD
         << "  +-------------------------------------------------+\n"
         << RESET
         << "  Choice: ";
}


void runCLI(SocialGraph& g) {
    string choice, input;

    while (true) {
        menu();
        getline(cin, choice);
        int cmd = -1;
        try { cmd = stoi(choice); } catch (...) {}
        cout << "\n";

        // 0. Exit
        if (cmd == 0) {
            cout << GREEN << "  Goodbye!\n" << RESET;
            break;
        }

        // 1. Search user
        else if (cmd == 1) {
            cout << "  Enter name or ID: ";
            getline(cin, input);
            int id = resolveUser(g, input);
            if (id != -1) printUser(g, id);
        }

        // 2. Friends list
        else if (cmd == 2) {
            cout << "  Enter name or ID: ";
            getline(cin, input);
            int id = resolveUser(g, input);
            if (id == -1) { continue; }
            vector<int> nbs = g.neighbors(id);
            cout << BOLD << "  Friends of " << g.users.at(id).name
                 << " (" << nbs.size() << "):\n" << RESET;
            for (int i = 0; i < (int)nbs.size(); i++)
                cout << "    - " << CYAN
                     << g.users.at(nbs[i]).name << RESET
                     << "  [" << g.users.at(nbs[i]).location << "]\n";
        }

        // 3. Mutual friends
        else if (cmd == 3) {
            cout << "  Enter first  user (name/ID): ";
            getline(cin, input);
            int a = resolveUser(g, input);
            if (a == -1) continue;

            cout << "  Enter second user (name/ID): ";
            getline(cin, input);
            int b = resolveUser(g, input);
            if (b == -1) continue;

            vector<int> m = g.mutuals(a, b);
            cout << BOLD << "  Mutual friends between "
                 << g.users.at(a).name << " & "
                 << g.users.at(b).name
                 << " (" << m.size() << "):\n" << RESET;
            if (m.empty())
                cout << DIM << "  None found.\n" << RESET;
            for (int i = 0; i < (int)m.size(); i++)
                cout << "    - " << CYAN
                     << g.users.at(m[i]).name << RESET << "\n";
        }

        // 4. BFS
        else if (cmd == 4) {
            cout << "  Enter source user (name/ID): ";
            getline(cin, input);
            int src = resolveUser(g, input);
            if (src == -1) continue;

            cout << "  Show up to degree: ";
            getline(cin, input);
            int maxDeg = 3;
            try { maxDeg = stoi(input); } catch (...) {}

            unordered_map<int,int> dist = g.bfs(src);
            cout << BOLD << "\n  BFS from "
                 << g.users.at(src).name << ":\n" << RESET;

            for (int deg = 1; deg <= maxDeg; deg++) {
                vector<string> level;
                for (auto it = dist.begin(); it != dist.end(); ++it)
                    if (it->second == deg)
                        level.push_back(g.users.at(it->first).name);
                if (level.empty()) break;
                sort(level.begin(), level.end());
                cout << YELLOW << "  Degree " << deg
                     << " (" << level.size() << " users): " << RESET;
                int shown = 0;
                for (int i = 0; i < (int)level.size(); i++) {
                    if (shown >= 8) {
                        cout << "...+" << level.size()-8 << " more";
                        break;
                    }
                    cout << level[i]
                         << (i+1 < (int)level.size() && shown < 7 ? ", " : "");
                    shown++;
                }
                cout << "\n";
            }
        }

        // 5. DFS
        else if (cmd == 5) {
            cout << "  Enter source user (name/ID): ";
            getline(cin, input);
            int src = resolveUser(g, input);
            if (src == -1) continue;

            vector<int> order = g.dfs(src);
            cout << BOLD << "\n  DFS from "
                 << g.users.at(src).name
                 << " (" << order.size() << " nodes visited):\n"
                 << RESET << "  ";
            int lim = (int)min((int)order.size(), 20);
            for (int i = 0; i < lim; i++)
                cout << CYAN << g.users.at(order[i]).name << RESET
                     << (i+1 < lim ? " -> " : "");
            if ((int)order.size() > 20)
                cout << DIM << " ...+"
                     << order.size()-20 << " more" << RESET;
            cout << "\n";
        }

        // 6. BFS shortest path
        else if (cmd == 6) {
            cout << "  Enter source (name/ID): ";
            getline(cin, input);
            int src = resolveUser(g, input);
            if (src == -1) continue;

            cout << "  Enter target (name/ID): ";
            getline(cin, input);
            int dst = resolveUser(g, input);
            if (dst == -1) continue;

            cout << BOLD << "\n  BFS shortest path (hops):\n" << RESET;
            printPath(g, g.bfsPath(src, dst));
        }

        // 7. Dijkstra
        else if (cmd == 7) {
            cout << "  Enter source (name/ID): ";
            getline(cin, input);
            int src = resolveUser(g, input);
            if (src == -1) continue;

            cout << "  Enter target (name/ID): ";
            getline(cin, input);
            int dst = resolveUser(g, input);
            if (dst == -1) continue;

            vector<int> path = g.dijkstraPath(src, dst);
            cout << BOLD << "\n  Dijkstra shortest path (weighted):\n"
                 << RESET;
            printPath(g, path);
            if (!path.empty()) {
                pair<unordered_map<int,int>,
                     unordered_map<int,int>> res = g.dijkstra(src);
                cout << "  Total weight : "
                     << BOLD << res.first[dst] << RESET << "\n";
            }
        }

        // 8. Top influencers
        else if (cmd == 8) {
            cout << "  How many? (default 10): ";
            getline(cin, input);
            int n = 10;
            try { n = stoi(input); } catch (...) {}

            vector<pair<int,double>> top = g.topInfluencers(n);
            cout << BOLD << "\n  Top " << n << " Influencers:\n" << RESET
                 << DIM << "  "
                 << left << setw(5)  << "Rank"
                          << setw(22) << "Name"
                          << setw(10) << "Score"
                          << setw(9)  << "Degree"
                          << "Followers\n" << RESET
                 << "  " << string(52, '-') << "\n";

            for (int i = 0; i < (int)top.size(); i++) {
                int id        = top[i].first;
                double score  = top[i].second;
                const User& u = g.users.at(id);
                cout << "  "
                     << YELLOW << left << setw(5)  << i+1
                     << CYAN          << setw(22) << u.name
                     << GREEN         << setw(10)
                     << fixed << setprecision(2) << score
                     << RESET         << setw(9)  << g.degree(id)
                     << u.followers   << "\n";
            }
        }

        // 9. Friend suggestions
        else if (cmd == 9) {
            cout << "  Enter user (name/ID): ";
            getline(cin, input);
            int uid = resolveUser(g, input);
            if (uid == -1) continue;

            vector<pair<int,int>> sugg = g.suggestions(uid);
            cout << BOLD << "\n  Suggestions for "
                 << g.users.at(uid).name << ":\n" << RESET;
            if (sugg.empty()) {
                cout << DIM << "  No suggestions.\n" << RESET;
                continue;
            }
            for (int i = 0; i < (int)sugg.size(); i++) {
                int id    = sugg[i].first;
                int score = sugg[i].second;
                cout << "    - "
                     << CYAN << left << setw(22)
                     << g.users.at(id).name << RESET
                     << DIM << score << " mutual friend(s)" << RESET
                     << "  [" << g.users.at(id).interest << "]\n";
            }
        }

        // 10. Communities
        else if (cmd == 10) {
            vector<vector<int>> comps = g.components();
            cout << BOLD << "\n  Connected Components: "
                 << comps.size() << "\n" << RESET;
            int limit = (int)min((int)comps.size(), 8);
            for (int i = 0; i < limit; i++) {
                cout << YELLOW << "  Component " << i+1
                     << " (" << comps[i].size() << " users): " << RESET;
                int show = (int)min((int)comps[i].size(), 5);
                for (int j = 0; j < show; j++)
                    cout << CYAN
                         << g.users.at(comps[i][j]).name << RESET
                         << (j+1 < show ? ", " : "");
                if ((int)comps[i].size() > 5)
                    cout << DIM << " ...+"
                         << comps[i].size()-5 << " more" << RESET;
                cout << "\n";
            }
            if ((int)comps.size() > 8)
                cout << DIM << "  ...and " << comps.size()-8
                     << " more small components\n" << RESET;
        }

        // 11. Stats
        else if (cmd == 11) {
            g.printStats();
        }

        // 12. Save report
        else if (cmd == 12) {
            cout << "  Output file (default: report.txt): ";
            getline(cin, input);
            if (input.empty()) input = "report.txt";
            saveReport(g, input);
        }

        else {
            cout << RED << "  Invalid option.\n" << RESET;
        }

        cout << DIM << "\n  Press Enter to continue..." << RESET;
        getline(cin, input);
    }
}

int main() {
    banner();

    SocialGraph g;
    cout << BOLD << "\n  Loading dataset...\n" << RESET;

    bool usersOk = loadUsers(g, "users.csv");
    bool edgesOk = loadEdges(g, "edges.csv");

    if (!usersOk || !edgesOk) {
        cout << YELLOW
             << "\n  CSV files not found. Generating sample data...\n"
             << RESET;
        generateSampleData(520, 1800);
        g = SocialGraph();
        loadUsers(g, "users.csv");
        loadEdges(g, "edges.csv");
    }

    cout << "\n";
    g.printStats();
    runCLI(g);
    return 0;
}