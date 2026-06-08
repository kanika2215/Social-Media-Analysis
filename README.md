# Social Media Network Analysis

A C++ application that models and analyzes a social media network using Graph Theory and Data Structures & Algorithms (DSA). The project supports user profile management, friend connections, network traversal, shortest-path analysis, influencer ranking, friend recommendations, community detection, and report generation.

---

## Features

### User & Network Management
- Load users from CSV files
- Load friendships/connections from CSV files
- Search users by:
  - User ID
  - Exact name
  - Partial name matching
- Display detailed user profiles

### Graph Algorithms

#### Breadth-First Search (BFS)
- Explore network connections by degree
- Find shortest path based on hop count

#### Depth-First Search (DFS)
- Traverse deeply connected regions
- Explore communities and network structure

#### Dijkstra's Algorithm
- Compute weighted shortest paths
- Support relationship strength via edge weights

### Social Network Analysis
- Mutual friend detection
- Friend recommendation system
- Connected component (community) analysis
- Graph statistics:
  - Number of users
  - Number of connections
  - Average degree
  - Graph density
  - Largest connected community

### Influencer Analytics
Custom influencer scoring based on:
- Number of direct connections
- Reachability within two hops
- Follower count

Generates rankings of top influencers in the network.

### Reporting
Export network analysis reports containing:
- Graph summary
- Top influencers
- Community statistics
- Connected component information

---

## Technologies Used

- C++14
- Graph Theory
- BFS (Breadth-First Search)
- DFS (Depth-First Search)
- Dijkstra's Algorithm
- STL Containers
  - vector
  - unordered_map
  - unordered_set
  - queue
  - stack
  - priority_queue
- File Handling (CSV I/O)

---

## Project Structure

```text
.
├── Project1.cpp
├── users.csv
├── edges.csv
├── report.txt
└── README.md
```

### User Dataset Format (users.csv)

```csv
id,name,location,age,interest,followers
1,Aarav_1,Mumbai,24,Technology,1200
2,Priya_2,Delhi,28,Music,950
```

### Edge Dataset Format (edges.csv)

```csv
from_id,to_id,weight
1,2,4
2,5,2
3,4,8
```

## Installation

### Prerequisites

- GCC 6.x or later
- Any C++14 compatible compiler

## Menu Options

```text
1. Search user profile
2. List all friends of a user
3. Find mutual friends
4. BFS - connections by degree
5. DFS - deep network traversal
6. Shortest path (BFS)
7. Shortest path (Dijkstra)
8. Top influencers
9. Friend suggestions
10. Community analysis
11. Graph statistics
12. Save analysis report
0. Exit
```

---

## Core Algorithms

| Algorithm | Purpose |
|------------|----------|
| BFS | Network traversal and shortest path by hops |
| DFS | Deep graph exploration |
| Dijkstra | Weighted shortest path |
| Connected Components | Community detection |
| Friend Recommendation | Friends-of-friends analysis |
| Influencer Ranking | Network influence estimation |

---

## Influencer Score Formula

```text
Influence Score =
0.4 × Degree +
0.4 × ReachWithin2Hops +
0.2 × (Followers / 100)
```

Where:

- Degree = Number of direct connections
- ReachWithin2Hops = Number of users reachable within two levels
- Followers = User follower count from dataset

---

## Sample Workflow

1. Start the application.
2. Load users.csv and edges.csv.
3. Search for a user profile.
4. Explore friends and mutual connections.
5. Find shortest paths between users.
6. View influencer rankings.
7. Analyze communities.
8. Export a report.

If dataset files are missing, the application automatically generates:

- 520 sample users
- 1800 sample connections

---

## Example Output

```text
Graph Statistics

Users (nodes)   : 520
Connections     : 1800
Graph Density   : 0.0134
Average Degree  : 6.92
Most Connected  : Rahul_102
Components      : 1
Largest Component : 520 users
```

## Key Concepts Demonstrated

- Graph Representation using Adjacency Lists
- Breadth-First Search (BFS)
- Depth-First Search (DFS)
- Dijkstra's Shortest Path Algorithm
- Community Detection
- Friend Recommendation Systems
- Network Influence Analysis
- CSV Data Processing
- File Input/Output Operations
- Command Line Interface (CLI) Design

---

## Future Enhancements

- Graph visualization support
- PageRank-based influencer ranking
- Dynamic addition/removal of users
- Directed graph support
- Real social media datasets
- GUI version using Qt
- Database integration (MySQL/PostgreSQL)
- Social sentiment analysis
- Network centrality metrics
- Real-time network updates

---

## Learning Outcomes

This project demonstrates practical applications of:

- Data Structures and Algorithms
- Graph Theory
- Social Network Analysis
- Object-Oriented Programming in C++
- File Handling
- Algorithm Design
- Network Analytics

