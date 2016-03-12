#include "SelectionSort.h"
#include <iostream>


using namespace std;



void SelectionSort::RecursiveSortArray(int n, int Array[], int SizeofArray,bool Ascending)
{
	MAX = 0;
	MIN = 10000;
	if(n<SizeofArray-1)
	{
		if(Ascending)
		{
			for(int i=n; i<SizeofArray; i++)
			{
				if(Array[i] <= MIN)
				{
					MIN = Array[i];
					Element = i;
				}
			}
		}
		else
		{
			for(int i=n; i<SizeofArray; i++)
			{
				if(Array[i] >= MAX)
				{
					MAX = Array[i];
					Element = i;
				}
			}
		}
		Temp = Array[n];
		Array[n] = Array[Element];
		Array[Element] = Temp;
		RecursiveSortArray(n+1,Array,SizeofArray,Ascending);
	}
}
void SelectionSort::RecursiveSortVector(int n, vector<int> &Vector,bool Ascending)
{
	MAX = 0;
	MIN = 10000;
	if(n<Vector.size()-1)
	{
		if(Ascending)
		{
			for(int i=n; i<Vector.size(); i++)
			{
				if(Vector.at(i) <= MIN)
				{
					MIN = Vector.at(i);
					Element = i;
				}
			}
		}
		else
		{
			for(int i=n; i<	Vector.size(); i++)
			{
				if(Vector.at(i) >= MAX)
				{
					MAX =Vector.at(i);
					Element = i;
				}
			}
		}
		Temp = Vector.at(n);
		Vector.at(n) = Vector.at(Element);
		Vector.at(Element) = Temp;
		RecursiveSortVector(n+1, Vector, Ascending);
	}
}
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
	if(Ascending)
	{
		for(int m=0; m<Vector.size(); m++)
		{
			MIN = 10000;
			for(int n=m; n<Vector.size(); n++)
			{
				if(Vector.at(n) <= MIN) 
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
	else
	{
		for(int m=0; m<Vector.size(); m++)
		{				
			MAX = 0;
			for(int n=m; n<Vector.size(); n++)
			{
				if(Vector.at(n) >= MAX) 
				{
					MAX = Vector.at(n);
					Element = n;
				}
			}
			Temp = Vector.at(m);
			Vector.at(m) = Vector.at(Element);
			Vector.at(Element) = Temp;
		}
	}
}