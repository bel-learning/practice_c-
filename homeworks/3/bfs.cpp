#include<iostream>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<sstream>
#include<queue>

using namespace std;

vector<vector< char>> grid;
vector<vector < bool > > visited;
vector < vector < pair<int, int> > > parent;
typedef pair<int,int> pi;


vector<pi> getNeighbors(pi node) {
    int x = node.first;
    int y = node.second;
    vector<pi> neighbors;
    
    int length = grid[0].size();
    int height = grid.size();
    
    // find all possible knight moves and determine if they are valid neighbors
    int dx[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    int dy[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < length && ny >= 0 && ny < height && grid[ny][nx] != 'X' && !visited[ny][nx]) {
            neighbors.push_back(make_pair(nx, ny));
        }
    }
    
    return neighbors;
}


void bfs(pi start, pi end, vector<vector<pi>> & parent) {
    queue<pi> q;
    q.push(start);
    if(start == end) {
        parent[end.second][end.first] = start;
        return;
    }
    while(!q.empty()) {
        pi cur = q.front();
        q.pop();
        visited[cur.second][cur.first] = true;

        if(cur == end) {
            return;
        }

        vector<pi> neighs = getNeighbors(cur);
        for(pi neigh : neighs) {
            q.push(neigh);
            parent[neigh.second][neigh.first] = cur;
        }
    }
   
}

vector<pi> backtrackParent(vector<vector<pi >> parent, pi start, pi end) {
    pi cur = end;
  
    vector<pi> path;
    while(start != cur) {
        // cout << cur.first << " " << cur.second << endl;
        path.push_back(cur);
        cur = parent[cur.second][cur.first];
    }
    path.push_back(cur);
    // for(size_t i = 0; i < path.size(); i++) {
    //     cout << path[i].first << " " << path[i].second << endl;
    // }
    return path;
}

int main () {
    for(size_t i = 0; i < 20; i++) {
        vector<char> line;
        for(size_t j = 0; j < 20; j++) {
            line.push_back(' ');
        }
        grid.push_back(line);
    }
    for(size_t i = 0; i < 20; i++) {
        vector<bool> line;
        for(size_t j = 0; j < 20; j++) {
            line.push_back(false);
        }
        visited.push_back(line);
    }
    for(size_t i = 0; i < 20; i++) {
        vector<bool> line;
        for(size_t j = 0; j < 20; j++) {
            line.push_back(false);
        }
        visited.push_back(line);
    }
     for(size_t i = 0; i < 20; i++) {
        vector<pair<int,int>> line;
        for(size_t j = 0; j < 20; j++) {
            line.push_back(make_pair(-1, -1));
        }
        parent.push_back(line);
    }
    pair<int, int> start = make_pair(5, 10);
    pair<int, int> end = make_pair(10, 5);
    grid[5][10] = 'S';
    grid[10][5] = 'E';
    
    vector<pi> path;
    bfs(start, end, parent);
    if(parent[end.second][end.first] != make_pair(-1,-1)) {
        path = backtrackParent(parent, start, end);
        cout << "Path found" << endl;
        reverse(path.begin(), path.end());
        for(size_t i = 0; i < path.size(); i++) {
            cout << path[i].first << ":" << path[i].second << endl;
        }
    }
    else {
        cout << "No path" << endl;
    }
    
}