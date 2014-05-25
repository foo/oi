// Maciej Pacut
#include <iostream>
#include <algorithm>
using namespace std;

#define loop(var, from, to) for(int var = from; var < to; ++var)
#define repeat(var, times) loop(var, 0, times)

template<typename  Type>
class Stack
{
   Type arr[250000];
   int height;
public:
   Type top() { return arr[height - 1]; }
   void push(int t) { arr[height++] = t; }
   int size() { return height; }
   void cut(int new_height) { height = new_height; }
   int find(Type t) { return upper_bound(arr, arr + height, t) - arr; }
};

Stack<long long> stack;
int posters = 0;

void WalkUp(int height)
{
   stack.push(height);
}

void WalkDown(int height)
{
   int left_side = stack.find(height);
   int rects = stack.size() - left_side;
   posters += rects;
   stack.cut(left_side);
   if(stack.top() != height)
      stack.push(height);
}

int main()
{
   ios_base::sync_with_stdio(0);

   int buildings; cin >> buildings;

   int counter = 0;
   int width; // unused
   long long height;

   long long before = -1;
   
   repeat(b, buildings)
   {
      cin >> width >> height;

      if(before == height)
	 continue;
      before = height;
      
      if(height < stack.top())
	 WalkDown(height);
      else
	 WalkUp(height);
   }

   cout << posters + stack.size() << endl;
   
   return 0;
}
