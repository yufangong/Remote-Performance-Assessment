#include <vector>
#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H
using namespace std;

class SelectionSort
{
public:
	void IterationSortArray(int[],int, bool);
	void IterationSortVector(vector<int> &, bool);

private:
	int MAX;
	int MIN;
	int Element;
	int Temp;
	
};
#endif
