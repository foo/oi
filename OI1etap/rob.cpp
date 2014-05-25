// Maciej Pacut

#include <stdio.h>
#include <queue>
using namespace std;

#define loop(var, from, to) for(int var = from; var < to; ++var)
#define repeat(var, num) loop(var, 0, num)

struct Coord {
   Coord() {}
   Coord(int X, int Y) : x(X), y(Y) {}
   Coord(const Coord& c) : x(c.x), y(c.y) {}
   int x, y;
};

const int infinite = 999999999;
const int maxMapSz = 2000;

int mapSz;
char map[maxMapSz][maxMapSz];

void ReadMap() {
   scanf("%d", &mapSz);
   repeat(x, mapSz)
      scanf("%s", &map[x]);
}

char GetMap(int x, int y) {
   if(x >= 0 && y >= 0 && x < mapSz && y < mapSz)
      return map[y][x];
   return '?';
}
char GetMap(const Coord& c) {
   return GetMap(c.x, c.y);
}
bool Acq(const Coord& c) {
   return 'X' == GetMap(c);
}
bool Rob(const Coord& c) {
   return 'r' == GetMap(c);
}

bool translatedMap[maxMapSz][maxMapSz];
bool& GetTranslated(int x, int y) {
   return translatedMap[y][x];
}

class Boat {
public:
   int upSz;
   int downSz;
   int leftSz;
   int rightSz;
   Coord center;
   Coord firstTop;
   Coord rightmost;
   int thick;
   int height;
   int width;
   int left[maxMapSz];
   int right[maxMapSz];
   int up[maxMapSz];
   int down[maxMapSz];
private:
   Coord FirstTop() {
      repeat(y, mapSz)
	 repeat(x, mapSz)
	 if(GetMap(x, y) == 'r')
	    return Coord(x, y);
   }
   Coord Rightfirst(Coord c) {
      while(!Rob(c))
	 ++c.x;
      return c;
   }
   Coord Rightmost(Coord c) {
      while(Rob(c))
	 ++c.x;
      --c.x;
      return c;
   }
   Coord Leftmost(Coord c) {
      do
	 --c.x;
      while(Rob(c));
      ++c.x;
      return c;
   }
   Coord Leftfirst(Coord c) {
      while(!Rob(c) && c.x >= 0)
	 --c.x;
      return c;
   }
    Coord Downmost(Coord c) {
      while(Rob(c))
	 ++c.y;
      --c.y;
      return c;
   }
public:
   void ReadLeft() {
      left[0] = firstTop.x;
      for(int y = 1; y < height; ++y) {
	 Coord lim;
	 lim.y = y + firstTop.y;
	 lim.x = left[y - 1];

	 if(Rob(lim))
	    left[y] = Leftmost(lim).x;
	 else
	    left[y] = Rightfirst(lim).x;
      }

      repeat(i, height)
       left[i] = center.x - left[i];
   }
   void ReadRight() {
      right[0] = rightmost.x;
      for(int y = 1; y < height; ++y) {
	 Coord lim;
	 lim.y = y + firstTop.y;
	 lim.x = right[y - 1];

	 if(Rob(lim))
	    right[y] = Rightmost(lim).x;
	 else
	    right[y] = Leftfirst(lim).x;
      }

      repeat(i, height)
	 right[i] = right[i] - center.x;
      }
   void ReadCenter() {
      thick = 0;
      loop(i, 1, height)
	 if(left[thick] + right[thick] < left[i] + right[i])
	    thick = i;
      center.y = thick + firstTop.y;

      leftSz = left[thick];
      rightSz = right[thick];
      upSz = thick;
      downSz = height - thick - 1;
      width = leftSz + rightSz + 1;
   }
   void ReadUp() {
      int inserter = 0;
      int top = -1;
      int before = left[thick];
      {
	 for(int i = thick; i >= 0; --i) {
	    int actual = left[i];
	    int delta = before - actual;
	    repeat(j, delta)
	       up[inserter++] = top;
	    ++top;
	    before = actual;
	 }
      }
      
      repeat(i, left[0] + right[0] + 1)
	 up[inserter++] = top;
      --top;
      {
	 before = right[0];
	 for(int i = 1; i < thick; ++i) {
	    int actual = right[i];
	    int delta = actual - before;
	    repeat(j, delta)
	       up[inserter++] = top;
	    --top;
	    before = actual;
	 }
      }
   }
   void ReadDown() {
      int inserter = 0;
      int top = -1;
      int before = left[thick];
      {
	 for(int i = thick; i < height; ++i) {
	    int actual = left[i];
	    int delta = before - actual;
	    repeat(j, delta)
	       down[inserter++] = top;
	    ++top;
	    before = actual;
	 }
      }
      
      repeat(i, left[height - 1] + right[height - 1] + 1)
	 down[inserter++] = top;
      --top;
      {
	 before = right[height - 1];
	 for(int i = height - 2; i >= thick; --i) {
	    int actual = right[i];
	    int delta = actual - before;
	    repeat(j, delta)
	       down[inserter++] = top;
	    --top;
	    before = actual;
	 }
      }
   }
   void Read() {
      firstTop = FirstTop();
      rightmost = Rightmost(firstTop);
      center = firstTop;
      center.x = (firstTop.x + rightmost.x) / 2;
      Coord downmost = Downmost(center);
      height = downmost.y - center.y + 1;

      ReadLeft();
      ReadRight();
      ReadCenter();
      ReadUp();
      ReadDown();
   }
};

Boat boat;

int TranslateRight(int start, int row) {
   int x = start;
   do {
      int offset = x - start;
      int index = boat.leftSz - offset;
      if(index < 0)
	 return x;
      int up = boat.down[index];
      if(row - up >= 0)
	 GetTranslated(x, row - up) = true;
      
      int down = boat.up[index];
      if(row + down < mapSz)
	 GetTranslated(x, row + down) = true;
      ++x;
      if(x == mapSz)
	 break;
   } while(GetMap(x, row) != 'X');
   return x;
}

void TranslateRowRight(int row) {
   repeat(x, mapSz)
      if(GetMap(x, row) == 'X') {
	 x = TranslateRight(x, row);
      }
}
int TranslateLeft(int start, int row) {
   int x = start;
   do {
      int offset = start - x;
      int index = boat.leftSz + offset;
      if(index >= boat.width)
	 return x;
      int up = boat.down[index];
      if(row - up >= 0)
	 GetTranslated(x, row - up) = true;
      
      int down = boat.up[index];
      if(row + down < mapSz)
	 GetTranslated(x, row + down) = true;
      --x;
      if(x == -1)
	 break;
   } while(GetMap(x, row) != 'X');
   return x;
}
void TranslateRowLeft(int row) {
   for(int x = mapSz - 1; x >= 0; --x)
      if(GetMap(x, row) == 'X') {
	 x = TranslateLeft(x, row);
      }
}

int TranslateUp(int start, int col) {
   int y = start;
   do {
      int offset = start - y;
      int index = boat.upSz + offset;
      if(index >= boat.height)
	 return y;
      int right = boat.left[index];
      if(col + right < mapSz)
	 GetTranslated(col + right, y) = true;
      
      int left = boat.right[index];
      if(col - left >= 0)
	 GetTranslated(col - left, y) = true;
      //printf("col = %d, y = %d, left = %d, right = %d\n", col, y, left, right);
      --y;
      if(y == -1)
	 break;
   } while(GetMap(col, y) != 'X');
   return y;
}

void TranslateColUp(int col) {
   for(int y = mapSz - 1; y >= 0; --y)
      if(GetMap(col, y) == 'X') {
	 y = TranslateUp(y, col);
      }
}
int TranslateDown(int start, int col) {
   int y = start;
   do {
      int offset = y - start;
      int index = boat.upSz - offset;
      if(index < 0)
	 return y;
      int right = boat.left[index];
      if(col + right < mapSz)
	 GetTranslated(col + right, y) = true;
      
      int left = boat.right[index];
      if(col - left >= 0)
	 GetTranslated(col - left, y) = true;
      //printf("col = %d, y = %d, left = %d, right = %d\n", col, y, left, right);
      ++y;
      if(y == mapSz)
	 break;
   } while(GetMap(col, y) != 'X');
   return y;
}

void TranslateColDown(int col) {
   repeat(y, mapSz)
      if(GetMap(col, y) == 'X') {
	 y = TranslateDown(y, col);
      }
}
void TranslateMap() {
   repeat(row, mapSz) {
      TranslateRowRight(row);
      TranslateRowLeft(row);
   }
   repeat(col, mapSz) {
      TranslateColDown(col);
      TranslateColUp(col);
   }
}


int partialCost[maxMapSz][maxMapSz];
int& PartialCost(int x, int y) {
   return partialCost[y][x];
}
int& PartialCost(const Coord& c) {
   return partialCost[c.y][c.x];
}

bool Vis(const Coord& c) {
   return PartialCost(c) != infinite;
}

struct Queue
{
   Coord q[maxMapSz*maxMapSz];
   int b, e;
   Queue() : b(0), e(0) {}
   Coord& Pop()
      {
	 return q[b++];
      }
   void Push(const Coord& c)
      {
	 q[e++] = c;
      }
   bool Empty()
      {
	 return b == e;
      }
};

Queue q;

struct NoExitException {};

int minCost = infinite;

void BFS() {
   repeat(x, mapSz)
      repeat(y, mapSz)
      PartialCost(x, y) = infinite;

   PartialCost(boat.center) = 0;
   q.Push(boat.center); 
   while(!q.Empty()) {
      Coord& actual = q.Pop();

      if(actual.x == 0)
	 minCost = min(minCost, PartialCost(actual) + boat.rightSz + 1);
      
      if(actual.x == mapSz - 1)
	 minCost = min(minCost, PartialCost(actual) + boat.leftSz + 1);
      
      if(actual.y == 0)
	 minCost = min(minCost, PartialCost(actual) + boat.downSz + 1);

      if(actual.y == mapSz - 1)
	 minCost = min(minCost, PartialCost(actual) + boat.upSz + 1);

      if(PartialCost(actual) + boat.leftSz < minCost ||
	  PartialCost(actual) + boat.rightSz < minCost ||
	  PartialCost(actual) + boat.upSz < minCost ||
	  PartialCost(actual) + boat.downSz < minCost)
      {
	 {
	    Coord next = actual;
	    ++next.x;

	    if(!Vis(next) && !GetTranslated(next.x, next.y)) {
	       PartialCost(next) = PartialCost(actual) + 1;
	       q.Push(next);
	    }
	 }
      
	 {
	    Coord next = actual;
	    --next.x;

	    if(!Vis(next) && !GetTranslated(next.x, next.y)) {
	       PartialCost(next) = PartialCost(actual) + 1;
	       q.Push(next);
	    }
	 }
	 {
	    Coord next = actual;
	    ++next.y;

	    if(!Vis(next) && !GetTranslated(next.x, next.y)) {
	       PartialCost(next) = PartialCost(actual) + 1;
	       q.Push(next);
	    }
	 }
	 {
	    Coord next = actual;
	    --next.y;

	    if(!Vis(next) && !GetTranslated(next.x, next.y)) {
	       PartialCost(next) = PartialCost(actual) + 1;
	       q.Push(next);
	    }
	 }
      }
   }
}
void PrintMap() {
   repeat(y, mapSz) {
      repeat(x, mapSz) {
         printf("%c", GetMap(x, y));
      }
      printf("\n");
   }
   printf("\n");
   repeat(y, mapSz) {
      repeat(x, mapSz) {
         if(GetTranslated(x,y))
            printf("H");
         else if(Vis(Coord(x, y)))
            printf("*");
         else
            printf(".");
      }
      printf("\n");
   }
}

int main() {
   ReadMap();
   boat.Read();

   TranslateMap();

   BFS();
   if(minCost != infinite)
      printf("%d\n", minCost);
   else
      printf("NIE\n");

   PrintMap();

   return 0;
}
