#include <iostream>
#include <list>
#include <queue>
using namespace std;

const int MAX_DATA = 100000;

#define T(a) cout << #a << "=[ " << a << " ]" << endl

struct Node {
  list<int> adj;
  bool vis;
};

int numnodes;
int numedges;
Node nodes[MAX_DATA];

void join(int a, int b) {
  nodes[a - 1].adj.push_back(b - 1);
}

void prGraph() {
  for(int i = 0; i < numnodes; ++i) {
    cout << i + 1 << ": ";
    for(list<int>::iterator j = nodes[i].adj.begin(); j != nodes[i].adj.end(); ++j)
      cout << *j + 1 << ' ';
    cout << endl;
  }
}

int excluded;
int bfs(int start) {
  int counter = 0;
  
  queue<int> q;
  q.push(start);
  while(!q.empty()) {
    int f = q.front();
    q.pop();
    if(nodes[f].vis || f == excluded)
      continue;
    ++counter;
    nodes[f].vis = true;
    for(list<int>::iterator i = nodes[f].adj.begin(); i != nodes[f].adj.end(); ++i) {
      q.push(*i);
    }
  }
  return counter;
}

int main() {
	ios_base::sync_with_stdio(0);
  cin >> numnodes >> numedges;

  for(int i = 0; i < numedges; ++i) {
    int a, b;
    cin >> a >> b;
    join(a, b);
    join(b, a);
  }
  
  for(excluded = 0; excluded < numnodes; ++excluded) {
  	
  for(int i = 0; i < numnodes; ++i) {
    nodes[i].vis = false;
  }
  
  	unsigned long long minus = 0;
  	for(list<int>::iterator i = nodes[excluded].adj.begin(); i != nodes[excluded].adj.end(); ++i) {
  	  if(!nodes[*i].vis) {
  	  	int b = bfs(*i);
  	  	int rest = numnodes - b - 1;
  	  	minus += b * 2; // e
  	  	minus += b * rest;
      }
    }
    cout << minus  << endl;
  }
  
  return 0;
}
