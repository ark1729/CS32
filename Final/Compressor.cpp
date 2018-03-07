#include "provided.h"
#include <string>
#include <vector>
#include <iostream> //I added for cerr
#include "substituteHashTable.h"
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
	int length = s.length();
	int capacity;
	if (length / 2 + 512 < 16384)				//calculate capacity based on string length
		capacity = length / 2 + 512;
	else
		capacity = 16384;
	vector<unsigned short> V;
	if (length == 0)					// emptystringonlypush back capacity
	{
		V.push_back(capacity);
		numbers = V;
		return;
	}
	//cerr << capacity <<endl;
	HashTable<string, unsigned short> comp(2 * capacity, capacity); //load factor .5 by doubling buckets
	for (unsigned short j = 0; j <= 255; j++)
	{
		//cerr << static_cast<char>(j) << endl;
		string st = "";
		//char c = j;
		st += static_cast<char>(j);
		//cerr << st << endl;
		comp.set(st, j, true);	
	}
	int nextFreeID = 256;
	string runSoFar;
	
	unsigned short value;
	for (int k = 0; k < length; k++)
	{
		char c = s[k];
		string c1 = "";
		c1 += c;
		string expandedRun = runSoFar + c;			//pseudocode provided
		if (comp.get(expandedRun, value))
		{
			runSoFar = expandedRun;
		}
		else
		{
			comp.get(runSoFar, value);
			unsigned short x = value;
			V.push_back(x);
			comp.touch(runSoFar);
			runSoFar = "";
			comp.get(c1, value);
			unsigned short cv = value;
			V.push_back(cv);
			if (!comp.isFull())
			{
				comp.set(expandedRun, nextFreeID);
				nextFreeID++;
			}
			else
			{
				string key;
				unsigned short value1;
				comp.discard(key, value1);
				comp.set(expandedRun, value1);
			}
		}
	}
	if (runSoFar != "")
	{
		comp.get(runSoFar, value);
		unsigned short x = value;
		V.push_back(x);
	}
	V.push_back(capacity);
	numbers = V;						//swap number and V to remove any previous values in number
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	int capacity = numbers.back();
	//cerr << capacity << endl;
	HashTable<unsigned short, string> decomp(2 * capacity, capacity); //same load factor inplay
	for (unsigned short j = 0; j <= 255; j++)
	{
		//cerr << static_cast<char>(j) << endl;
		string st = "";
		//char c = j;
		st += static_cast<char>(j);
		//cerr << st << endl;
		decomp.set(j, st, true);
	}
	int nextFreeID = 256;
	string runSoFar = "";
	string output = "";
	unsigned short us;
	string value = "";
	string temp = "";
	unsigned short tempnum;
	for (int k = 0; k < numbers.size() - 1; k++)
	{
		us = numbers[k];
		if (us <= 255)
		{
			decomp.get(us, value);
			output += value;
			if (runSoFar.empty())
			{
				runSoFar += value;
			}
			else
			{
				string expandedRun = runSoFar + value;
				if (!decomp.isFull())
				{
					decomp.set(nextFreeID, expandedRun);
					nextFreeID++;
				}
				else
				{
					decomp.discard(tempnum, temp);
					decomp.set(tempnum, expandedRun);
				}
				runSoFar = "";
			}
		}
		else
		{
			if (!decomp.get(us, temp))				// false value if string does not belong to message as it is not in the hashtable
			{
				return false;
			}
			else
			{
				decomp.touch(us);
				string S = "";
				decomp.get(us, S);
				output += S;
				runSoFar = S;
			}
		}
	}
	s = output;
	return true;
}

unsigned int computeHash(string key);			//declared
unsigned int computeHash(unsigned short key);

unsigned int computeHash(string s)			//implemented method to give an int  from string based on character and length
{
	int i;
	int total = 0;
	for (i = 0; i < s.length(); i++)
		total = total + (i + 1)*s[i];
	return total;
}

unsigned int computeHash(unsigned short n)	//return the key value 
{
	return n;
}
