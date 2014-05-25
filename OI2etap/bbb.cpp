#include <iostream>
#include <list>
using namespace std;

#define loop(i, n) for(int i = 0; i < n; ++i)
#define foreach(i, lst) for(list<bool>::iterator i = lst.begin(); i != lst.end(); ++i)
#define foreach_down(i, lst) for(list<bool>::reverse_iterator i = lst.rbegin(); i != lst.rend(); ++i)
#define T(a) cout << #a << " = [ " << a << " ]" << endl

int n, p, q, x, y;

list<bool> s;
list<bool> t;

int min_cost = -1;

void pr() {
	cout << "***" << endl << endl; 
	foreach(i, t)
		cout << *i;
	cout << endl;
}

void rev() {
	bool temp = s.back();
	s.pop_back();
	s.push_front(temp);
}

int inc(int d) {
	int c = 0;
	foreach(i, t) {
		if(*i == false) {
			*i = true;
			c += x;
			if(--d == 0)
				return c;
		}
	}
}

int dec(int d) {
	int c = 0;
	foreach_down(i, t) {
		//cout << '.';
		if(*i == true) {
			*i = false;
			c += x;
			if(--d == 0)
				return c;
		}
	}
}

int correctLvl() {
	int d = p - q;
	if(d > 0) {
		return inc(d);
	} else if(d < 0) {
		return dec(-d);
	}
	return 0; // p == q
}

list<bool>::reverse_iterator switchLastPlus(list<bool>::reverse_iterator r) {
	for(; r != t.rend(); ++r)
		if(*r == '+') {
			*r = '-';
			return r;
		}
}

int correctMinus() {
	list<bool>::reverse_iterator r = t.rbegin();
	int state = p;
	int cost = 0;
	foreach(i, t) {
		if(*i)
			state++;
		else
			state--;
		if(state < 0) {
			r = switchLastPlus(r);
			(*i) = false;
			cost += 2 * x;
		}
	}
	return cost;
}

int main() {
	ios_base::sync_with_stdio(0);
	
	cin >> n >> p >> q >> x >> y;
	loop(i, n) {
		char a;
		cin >> a;
		bool p = (a == '+');
		s.push_back(p);
	}
	
	loop(i, n) {
		t.clear();
		copy(s.begin(), s.end(), back_inserter(t));
		//pr();
		int rev_cost = y * i;
		int actual_cost = correctLvl() + correctMinus() + rev_cost;
		
		if(actual_cost < min_cost || min_cost == -1)
			min_cost = actual_cost;
		//T(rev_cost);
		//T(actual_cost);
		
		rev();
	}
	
	cout << min_cost << endl;
}
