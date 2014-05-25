#include <iostream>
#include <list>
using namespace std;

#define PB push_back
#define T(a) cout << #a << " = [ " << a << " ]\n"
#define loop(i, n) for(int i = 0; i < n; ++i)
#define repeat(n) loop(REP, n)
#define foreach(i, lst) for(list<LT>::iterator i = lst.begin(); i != lst.end(); ++i)
#define TC(coord) cout << #coord " = ( " << (coord).x << ' ' << (coord).y << " )" << endl

const int MAX = 100;
bool vis[MAX][MAX];

int h, w;
int k;

struct Coord {
	int x, y;
	Coord() {}
	Coord(const Coord& c) : x(c.x), y(c.y) {}
	Coord(int X, int Y) : x(X), y(Y) {}
};

bool& Vis(const Coord& c) {
	return vis[c.x][c.y];
}

bool valid(Coord& c) {
	return c.x >= 0 && c.y >= 0 && c.y < h && c.x < w;
}

enum LT { L, R, U, D };

Coord move(LT lt, Coord& c) {
	switch(lt) {
	case L: return Coord(c.x - 1, c.y);
	case R: return Coord(c.x + 1, c.y);
	case U: return Coord(c.x, c.y - 1);
	case D: return Coord(c.x, c.y + 1);
	}
}

char* tr(LT lt) { switch (lt) { case L: return "L"; case R: return "R"; case D: return "D"; case U: return "U"; } }

list<LT> next(LT turn) {
	list<LT> l;
	switch(turn) {
	case L:
		l.PB(L);
		l.PB(U);
		break;
	case R:
		l.PB(R);
		l.PB(D);
		break;
	case U:
		l.PB(U);
		l.PB(R);
		break;
	case D:
		l.PB(D);
		l.PB(L);
		break;
	}
	return l;
}

void prVis() {
	loop(y, h) {
		loop(x, w) {
			cout << (vis[x][y] ? 'H' : '*');
		}
		cout << endl;
	}
	cout << endl;
}
Coord start;
Coord end;

bool north(const Coord& c) { return c.y < end.y; }
bool south(const Coord& c) { return c.y > end.y; }
bool west(const Coord& c) { return c.x < end.x; }
bool east(const Coord& c) { return c.x > end.x; }

//int s = -1;

int dfs(Coord a, LT lastTurn) {
	if(Vis(a)) 
		return 0;
	if(!valid(a))
		return 0;
	
	if(a.x == end.x && a.y == end.y) {
		//cout << endl << "end!" << endl;
		//Vis(a) = true;
		//prVis();
		Vis(a) = false;
		return 1;
	}
	
	switch(lastTurn) {
	case L: if(north(a)) return 0; break;
	case R: if(south(a)) return 0; break;
	case U: if(east(a)) return 0; break;
	case D: if(west(a)) return 0; break;
	}
	
	Vis(a) = true;
	//prVis();
	/*++s;
	repeat(s) cout << '.';
	cout << tr(lastTurn);
	TC(a);*/
	
	int sum = 0;
	
	list<LT> nxt = next(lastTurn);
	//foreach(i, nxt) { cout << tr(*i); } cout << endl;
	
	foreach(i, nxt) {
		Coord mv = move(*i, a);
		//T(*i);
		sum += dfs(mv, *i);
	}
	//--s;
	Vis(a) = false;
	
	return sum % k;
}

void read() {
	ios_base::sync_with_stdio(0);
	
	cin >> h >> w;
	start = Coord(0, h - 1);
	
	cin >> k;
	
	cin >> end.x >> end.y;
	
	--end.x, --end.y;
	
	loop(y, h) {
		loop(x, w) {
			char p;
			cin >> p;
			vis[x][y] = (p == '*');
		}
	}
}
int main() {
	read();
	
	cout << dfs(start, U) << endl;
	
	return 0;
}
