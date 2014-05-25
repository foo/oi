// Maciej Pacut

#include <iostream>
#include <list>
using namespace std;

#define loop(var, from, to) for(int var = from; var < to; ++var)
#define repeat(var, num) loop(var, 0, num)
#define trace(var) cout << #var << "=[ " << (var) << " ]" << endl
#define for_each(type, var, container) for(type::iterator var = container.begin(); var != container.end(); ++var)

const long long max_nodes = 100000;

template<typename Type, int max_data>
class Stack {
   Type arr[max_data];
   int height;
public:
   Type pop() { return arr[(height--) - 1]; }
   void push(int t) { arr[height++] = t; }
   void clear() { height = 0; }
   bool empty() { return 0 == height; } 
};

Stack<int, max_nodes> stack;

int num_nodes;
int num_edges;

struct Node {
   list<int> adj;
   int prev;
   bool vis;
};

Node nodes[max_nodes];

void ReadGraph() {
   cin >> num_nodes >> num_edges;
   repeat(e, num_edges) {
      int from, to;
      cin >> from >> to;
      --from;
      --to;
      nodes[from].adj.push_back(to);
      nodes[to].adj.push_back(from);
   }
   repeat(n, num_nodes) {
      nodes[n].prev = -1;
      nodes[n].vis = false;
   }
}

void PrintPrev() {
   repeat(n, num_nodes)
      cout << nodes[n].prev + 1 << endl;
}

struct NoCycleException {};

int FindNodeInCycle(int start) {
  stack.clear();
  stack.push(start);
  nodes[start].vis = true;
  while(true) {
    if(stack.empty())
      throw NoCycleException();
	 
    int actual_number = stack.pop();
    Node& actual_node = nodes[actual_number];

    for_each(list<int>, next, actual_node.adj) {
      int next_number = *next;
      if(next_number != actual_node.prev) {
	node& next_node = nodes[next_number];
	if(next_node.vis)
	  return actual_number;
	else {
	  next_node.vis = true;
	  next_node.prev = actual_number;
	  stack.push(next_number);
	}
      }
    }
  }
}

void DFS(int start) {
   repeat(n, num_nodes)
      nodes[n].vis = false;
   
   stack.clear();
   stack.push(start);
   nodes[start].vis = true;
   while(!stack.empty()) {
      int actual_number = stack.pop();
      Node& actual_node = nodes[actual_number];

      for_each(list<int>, next, actual_node.adj) {
	 int next_number = *next;
	 if(next_number != actual_node.prev) {
	    Node& next_node = nodes[next_number];
	    if(!next_node.vis) {
	       next_node.vis = true;
	       next_node.prev = actual_number;
	       stack.push(next_number);
	    }
	 }
      }
   }
}

int main() {
	ios_base::sync_with_stdio(0);
   
   ReadGraph();
   
   try {
      repeat(n, num_nodes) {
	 if(nodes[n].vis)
	    continue;
	 DFS(FindNodeInCycle(n));
      }

      cout << "TAK" << endl;
      PrintPrev();
   }
   catch(NoCycleException&) {
      cout << "NIE" << endl;
   }

   return 0;
}
