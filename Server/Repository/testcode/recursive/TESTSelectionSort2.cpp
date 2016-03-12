#include "SelectionSort.h"
#include <iostream>


using namespace std;

void SelectionSort::IterationSortArray(int Array[], int SizeOfArray, bool Ascending)
{
	if(Ascending)
	{
		for(int m=0; m<SizeOfArray; m++)
		{
			MIN = 10000;
			for(int n=m; n<SizeOfArray; n++)
			{
				if(Array[n] <= MIN) 
				{
					MIN = Array[n];
					Element = n;
				}
			}
			Temp = Array[m];
			Array[m] = Array[Element];
			Array[Element] = Temp;
		}
	}
	else
	{
		for(int m=0; m<SizeOfArray; m++)
		{				
			MAX = 0;
			for(int n=m; n<SizeOfArray; n++)
			{
				if(Array[n] >= MAX) 
				{
					MAX = Array[n];
					Element = n;
				}
			}
			Temp = Array[m];
			Array[m] = Array[Element];
			Array[Element] = Temp;
		}
	}
}
void SelectionSort::IterationSortVector(vector<int> &Vector, bool Ascending)
{
	if (Ascending)
	{
		for (int m = 0; m < Vector.size(); m++)
		{
			MIN = 10000;
			for (int n = m; n < Vector.size(); n++)
			{
				if (Vector.at(n) <= MIN)
				{
					MIN = Vector.at(n);
					Element = n;
				}
			}
			Temp = Vector.at(m);
			Vector.at(m) = Vector.at(Element);
			Vector.at(Element) = Temp;
		}
	}
}