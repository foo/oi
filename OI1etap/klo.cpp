// Maciej Pacut
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

#define loop(var, from, to) for(int var = from; var < to; ++var)
#define repeat(var, times) loop(var, 0, times)

int n, m;

const int max_m = 100000;
const int max_h = 1000000;
int arr[max_m];
int sub[max_m];

map<int, int> low;
map<int, int> high;
long long low_sum = 0;
long long high_sum = 0;
int median;

long long min_cost;
int min_begin = 0;
int min_mediana = -1;

void InsertHigh(int a)
{
   ++high[a];
   high_sum += a;
}

void InsertLow(int a)
{
   ++low[a];
   low_sum += a;
}

void RemHigh(int a)
{
   --high[a];
   if(high[a] == 0)
      high.erase(a);
   high_sum -= a;
}

void RemLow(int a)
{
   --low[a];
   if(low[a] == 0)
      low.erase(a);
   low_sum -= a;
}

int LowLast()
{
   map<int, int>::iterator i = low.end();
   --i;
   return (*i).first;
}

int RemLowLast()
{
   int ll = LowLast();
   RemLow(ll);
   return ll;
}

int HighFirst()
{
   return (*high.begin()).first;
}

int RemHighFirst()
{ 
   int hf = HighFirst();
   RemHigh(hf);
   return hf;
}
int main()
{
   ios_base::sync_with_stdio(0);
   cin >> n >> m;

   repeat(i, n)
      cin >> arr[i];

   if(m == 1)
   {
      cout << 0 << endl;
      repeat(i, n)
	 cout << arr[i] << endl;
      return 0;
   }
   else
   {
      copy(arr, arr + m, sub);
      sort(sub, sub + m);

      if(m % 2 == 0)
      {
	 int inserter = 0;
	 while(inserter < m / 2)
	    InsertLow(sub[inserter++]);
	 
	 while(inserter < m)
	    InsertHigh(sub[inserter++]);
	 median = (HighFirst() + LowLast()) / 2;

	 int set_sz = m / 2;
	 long long cost = median * set_sz - low_sum + high_sum - median * set_sz;
	 min_cost = cost;
	 min_begin = 0;
	 min_mediana = median;
	 	 
	 loop(head, m, n)
	 {
	    int next = arr[head];
	    int old = arr[head - m];
	    
	    if(old < median)
	    {
	       RemLow(old);
	       
	       if(next < median)
	       {
		  InsertLow(next);
		  median = (HighFirst() + LowLast()) / 2;
	       }
	       else if(next > median)
	       {
		  InsertHigh(next);

		  InsertLow(RemHighFirst());
		  median = (HighFirst() + LowLast()) / 2;
	       }
	       else
	       {
		  InsertLow(next);
		  median = (HighFirst() + LowLast()) / 2;
	       }
	    }
	    else if(old > median)
	    {
	       RemHigh(old);
	       	       
	       if(next > median)
	       {
		  InsertHigh(next);
		  median = (HighFirst() + LowLast()) / 2;
	       }
	       else if(next < median)
	       {
		  InsertLow(next);

		  InsertHigh(RemLowLast());
		  median = (HighFirst() + LowLast()) / 2;
	       }
	       else
	       {
		  InsertHigh(next);
		  median = (HighFirst() + LowLast()) / 2;
	       }
	    }
	    else
	    {
	       if(next < median)
	       {
		  RemLowLast();
		  InsertLow(next);
		  median = (HighFirst() + LowLast()) / 2;
	       }
	       else if(next > median)
	       {
		  RemHighFirst();
		  InsertHigh(next);
		  median = (HighFirst() + LowLast()) / 2;
	       }
	       else
	       {
		  //nic sie nie dzieje jak odchodzi mediana i przychodzi taka sama liczba na jej miejsce
	       }
	    }

	    int set_sz = m / 2;
	    long long cost = median * set_sz - low_sum + high_sum - median * set_sz;
	    if(min_cost > cost)
	    {
	       min_cost = cost;
	       min_begin = head - m + 1;
	       min_mediana = median;
	    }
	    
	 }
      }
      else // nieparzyste
      {
	 int inserter = 0;
	 while(inserter < m / 2)
	    InsertLow(sub[inserter++]);
	 
	 median = sub[inserter++];
	 
	 while(inserter < m)
	    InsertHigh(sub[inserter++]);

	 int set_sz = ((m - 1) / 2);
	 long long cost = median * set_sz - low_sum + high_sum - median * set_sz;
	 min_cost = cost;
	 min_begin = 0;
	 min_mediana = median;
	 
	 loop(head, m, n)
	 {
	    int next = arr[head];
	    int old = arr[head - m];

	    if(old < median)
	    {
	       RemLow(old);
	       
	       if(next < median)
		  InsertLow(next);
	       else if(next > median)
	       {
		  InsertLow(median);
		  InsertHigh(next);

		  median = RemHighFirst();
	       }
	       else
		  InsertLow(next);
	    }
	    else if(old > median)
	    {
	       RemHigh(old);
	       	       
	       if(next > median)
		  InsertHigh(next);
	       else if(next < median)
	       {
		  InsertHigh(median);
		  InsertLow(next);

		  median = RemLowLast();
	       }
	       else
		  InsertHigh(next);
	    }
	    else
	    {
	       if(next < median)
	       {
		  InsertLow(next);
		  median = RemLowLast();
	       }
	       else if(next > median)
	       {
		  InsertHigh(next);
		  median = RemHighFirst();
	       }
	       else
	       {
		  //nic sie nie dzieje jak odchodzi mediana i przychodzi taka sama liczba na jej miejsce
	       }
	    }

	    int set_sz = ((m - 1) / 2);
	    long long cost = median * set_sz - low_sum + high_sum - median * set_sz;
	    if(min_cost > cost)
	    {
	       min_cost = cost;
	       min_begin = head - m + 1;
	       min_mediana = median;
	    }
	 }
      }
   }
   cout << min_cost << endl;
   loop(i, 0, min_begin)
      cout << arr[i] << endl;
   loop(i, min_begin, min_begin + m)
      cout << min_mediana << endl;
   loop(i, min_begin + m, n)
      cout << arr[i] << endl;
   
   return 0;
}
