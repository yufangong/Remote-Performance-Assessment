#include <vector>
#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H
using namespace std;

class SelectionSort
{
public:
	
	void RecursiveSortArray(int, int [], int, bool);
	void RecursiveSortVector(int, vector<int> &, bool);
	void IterationSortArray(int[],int, bool);
	void IterationSortVector(vector<int> &, bool);

private:
	int MAX;
	int MIN;
	int Element;
	int Temp;
	
};
#endif
