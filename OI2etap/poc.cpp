// Maciej Pacut 
#include <stdio.h>
#include <algorithm>
#include <set>
using namespace std;

#define loop(i, n) for(int i = 0; i < n; ++i)
#define foreach(i, cont) for(set<int>::iterator i = cont.begin(); i != cont.end(); ++i)

int numtrains, numwagons, numswaps;

char trains[1000][100];
set<int> sim[1000];

void detach(int a, int b) {
	sim[a].erase(b);
	sim[b].erase(a); // jak nizej vvv
}

void attach(int a, int b) {
	sim[a].insert(b);
	sim[b].insert(a); // jesli a == b to nie wstawi dwa razy bo set nie ma duplikatow
}

int simmax[1000];

bool similarp(int a, int b) {
	return equal(trains[a], trains[a] + numwagons, trains[b]);
}

void recalc(int a) {
	simmax[a] = max(simmax[a], (int)sim[a].size());
}

void recalcAll() {
	loop(i, numtrains)
		recalc(i);
}

void printSim() {
	loop(i, numtrains) {
		printf("%d: ", i+1);
		foreach(j, sim[i])
			printf("%d ", *j+1);
		printf("| %d\n", simmax[i]);
	}
}

void precalcSimilars() {
	loop(i, numtrains)
		loop(j, i + 1) // trojkat wraz z przekatna (i +1)
			if(similarp(i, j))
				attach(i, j);
	recalcAll();
}

void findSimilar(int changed) {
	foreach(i, sim[changed])
		detach(*i, changed); // zmiana wielkosci, ale bez recalc <- mniejsze niz max
	
	loop(i, numtrains)
		if(similarp(changed, i)) {
			attach(i, changed);
			recalc(i);
		}
	recalc(changed);
}

void findSimilars(int a, int b) {
	foreach(i, sim[a])
		detach(*i, a); // zmiana wielkosci, ale bez recalc <- mniejsze niz max
	
	foreach(i, sim[b])
		detach(*i, b); // zmiana wielkosci, ale bez recalc <- mniejsze niz max
	
	loop(i, numtrains) {
		if(similarp(a, i)) {
			attach(i, a);
			recalc(i);
		}
		if(similarp(b, i)) {
			attach(i, b);
			recalc(i);
		}
		recalc(i);
	}
	recalc(a);
	recalc(b);
}

void swapWagons(int p1, int w1, int p2, int w2) {
	swap(trains[p1][w1], trains[p2][w2]);
}

int main() {
	scanf("%d %d %d", &numtrains, &numwagons, &numswaps);
	
	loop(i, numtrains)
		simmax[i] = -1;
	
	loop(i, numtrains)
		scanf("%s", trains[i]);
		
	precalcSimilars();
	
	loop(s, numswaps) {
		int p1, w1, p2, w2;
		scanf("%d %d %d %d", &p1, &w1, &p2, &w2);
		--p1, --p2, --w1, --w2;
		
		if(trains[p1][w1] != trains[p2][w2]) {
			swapWagons(p1, w1, p2, w2);
			if(p1 != p2)
				findSimilars(p1, p2);
			else
				findSimilar(p1);
		}
	}
	
	loop(i, numtrains)
		printf("%d\n", simmax[i]);
	return 0;
}
