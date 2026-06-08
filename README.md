# Social-Media-Analysis

A command-line social network analyser built in C++ using core graph theory
algorithms — BFS, DFS, and Dijkstra — to model user connections, detect
influencers, find mutual friends, and compute shortest paths across a 500+ user
network loaded from CSV files.

---

## Features

- Load network from CSV files (users + weighted connections)
- Auto-generates a 520-user, 1800-edge dataset if no CSV files are found
- BFS — level-wise traversal, hop-count shortest path, 2-hop reach scoring
- DFS — deep network traversal, connected community detection
- Dijkstra — weighted shortest path using a min-heap priority queue
- Influencer scoring — degree + 2-hop reach + follower count combined
- Mutual friends between any two users
- Friend suggestions based on 2nd-degree connections
- Connected component analysis
- Graph statistics — density, average degree, largest component
- Save full analysis report to a .txt file
- Interactive CLI with 12 query options
- Color-coded terminal output

---

## Project Structure

DSA_Project/
│
├── social_network.cpp     # Full source code
├── users.csv              # Auto-generated user dataset (520 users)
├── edges.csv              # Auto-generated connections (1800 edges)
├── report.txt             # Output report (generated on demand)
└── README.md

## Usage

On first run, if `users.csv` and `edges.csv` are not present in the same
folder as the executable, the program automatically generates them with
520 users and 1800 weighted connections.
╔══════════════════════════════════════════════════╗
║     Social Media Network Analyser  v1.0          ║
║     C++ | Graph Theory | BFS | DFS | Dijkstra   ║
╚══════════════════════════════════════════════════╝
┌─ MENU ───────────────────────────────────────┐
│  1.  Search user profile                     │
│  2.  List all friends of a user              │
│  3.  Find mutual friends (two users)         │
│  4.  BFS — connections by degree             │
│  5.  DFS — deep network traversal            │
│  6.  Shortest path — BFS (hop count)         │
│  7.  Shortest path — Dijkstra (weighted)     │
│  8.  Top influencers                         │
│  9.  Friend suggestions                      │
│  10. Community / component analysis          │
│  11. Graph statistics                        │
│  12. Save analysis report                    │
│  0.  Exit                                    │
└──────────────────────────────────────────────┘
---

## CSV Format

If you want to load your own data, create these two files in the same
folder as the executable:

**users.csv**
id,name,location,age,interest,followers
1,Aarav_1,Mumbai,24,Technology,1200
2,Priya_2,Delhi,29,Music,850

**edges.csv**
from_id,to_id,weight
1,2,7
2,3,4

Weight represents relationship strength on a scale of 1–10.

---

## Algorithms

| Algorithm | Time Complexity | Use in this project |
|-----------|----------------|---------------------|
| BFS | O(V + E) | Shortest path by hops, level-wise connections, 2-hop reach |
| DFS | O(V + E) | Deep traversal, connected component detection |
| Dijkstra | O((V + E) log V) | Weighted shortest path via min-heap priority queue |

---

## Influencer Score

Each user is scored using a weighted formula:
score = degree × 0.4  +  2-hop reach × 0.4  +  (followers / 100) × 0.2
- **degree** — number of direct connections
- **2-hop reach** — number of users reachable within 2 connections via BFS
- **followers** — normalised follower count from the dataset

---

## Sample Output
Graph Statistics
─────────────────────────────────────
Users (nodes)   : 520
Connections     : 1800
Graph density   : 0.0133
Avg degree      : 6.92
Most connected  : Rahul_47  (degree 18)
Components      : 1
Largest comp.   : 520 users
Top 3 Influencers:
Rank  Name                Score       Degree   Followers
──────────────────────────────────────────────────────
1     Rahul_47            38.40       18       4821
2     Priya_12            35.60       16       4103
3     Aarav_203           33.20       15       3950
