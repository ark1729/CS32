#include "provided.h"
#include <string>
#include <iostream> //I put for cerr
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if(hostIn.empty())
		return false; 
	hostOut = "";
	vector<unsigned short> compressed;			//compress and encode
	Compressor::compress(msg, compressed);
	string encodedstring = BinaryConverter::encode(compressed);
	int numOfLines = 0;
	for (int k = 0; k < hostIn.length(); k++)   
	{
		if (hostIn[k] == '\r')
		{
			k++;
			if (hostIn[k] == '\n')
			{
				numOfLines++;
			}										//find number of lines
		}
		else if (hostIn[k] == '\n')
		{
			numOfLines++;
		}
	}
	if (hostIn.back() != '\n')
	{
		numOfLines++;
	}
	
	string* hidden = new string[numOfLines];	//dynamically allocate array of string equivalent to number of lines
	string ele;

	 int numOfLines1 = 0;

	for (int k = 0; k < hostIn.length(); k++)   //iterate through hostIn string and get lines
	{
		if (hostIn[k] == '\r')
		{
			k++;
			if (hostIn[k] == '\n')
			{
				
				hidden[numOfLines1] = ele;
				numOfLines1++;
				ele = "";
			}
		}
		else if (hostIn[k] == '\n')
		{
			
			hidden[numOfLines1] = ele;
			numOfLines1++;
			ele = "";
		}
		else
			ele += hostIn[k];	
	}
	if (hostIn.back() != '\n')
	{
		hidden[numOfLines1] = ele;
		numOfLines1++;
		ele = "";
	}

	for (int j = 0; j < numOfLines; j++)			//remove trailing spaces and tabs
	{
		for (int i = hidden[j].length() - 1; i >= 0; i--)
		{
			if (hidden[j].at(i) == '\t' || hidden[j].at(i) == ' ')
				hidden[j].erase(i);
			else
				break;
		}
	}

	/*for (int j = 0; j < numOfLines; j++)
	{
		//cerr << hidden[j] << endl;
	}*/

	/*if (numOfLines == numOfLines1)
		//cerr << "continue" <<endl;*/
	
	string* encodesub;
	encodesub = new string[numOfLines];
	int encodesublength1 = encodedstring.length() % numOfLines;
	int subOfLength1 = 0;
	int numEncodesub = 0;

	for (int n = 0; n < encodedstring.length(); n++)				//split up encoded string to add to end of lines
	{
		if (subOfLength1 < encodesublength1)
		{
			for (int i = 0; i < (encodedstring.length()) / numOfLines + 1; i++) //for first size
			{
				ele += encodedstring[n];
				if (i + 1 != (encodedstring.length()) / numOfLines + 1)
					n++;
			}
			encodesub[numEncodesub] = ele;
			ele = "";
			numEncodesub++;
			subOfLength1++;
		}
		else
		{
			for (int i = 0; i < (encodedstring.length()) / numOfLines; i++)	// for second size
			{
				ele += encodedstring[n];
				if (i + 1 != (encodedstring.length()) / numOfLines)
					n++;
			}
			encodesub[numEncodesub] = ele;
			ele = "";
			numEncodesub++;
		}
	}

	/*if (numOfLines == numEncodesub)
		//cerr << "continue" << endl;*/

	for (int j = 0; j < numOfLines; j++)	//add encoded pieces
	{
		hidden[j] += encodesub[j];
	}
	/*for (int j = 0; j < numOfLines; j++)
	{
		//cerr << hidden[j] << endl;
	}*/
	for (int j = 0; j < numOfLines; j++)
	{
		hostOut = hostOut + hidden[j] + '\n'; //append message to host out
	}
	delete[] hidden; //destroy dynamically allocated arrays
	delete[] encodesub;
	return true;
}

bool Steg::reveal(const string& hostIn, string& msg) 
{
	int numOfLines = 0;
	for (int k = 0; k < hostIn.length(); k++)	//find number of lines
	{
		if (hostIn[k] == '\r')
		{
			k++;
			if (hostIn[k] == '\n')
			{
				numOfLines++;
			}
		}
		else if (hostIn[k] == '\n')
		{
			numOfLines++;
		}
	}
	if (hostIn.back() != '\n')
	{
		numOfLines++;
	}

	string* hidden = new string[numOfLines];
	string ele;

	int numOfLines1 = 0;

	for (int k = 0; k < hostIn.length(); k++)   //iterate through hostIn string and get lines
	{
		if (hostIn[k] == '\r')
		{
			k++;
			if (hostIn[k] == '\n')
			{

				hidden[numOfLines1] = ele;
				numOfLines1++;
				ele = "";
			}
		}
		else if (hostIn[k] == '\n')
		{

			hidden[numOfLines1] = ele;
			numOfLines1++;
			ele = "";
		}
		else
			ele += hostIn[k];
	}
	if (hostIn.back() != '\n')
	{
		hidden[numOfLines1] = ele;
		numOfLines1++;
		ele = "";
	}

	string encoded = "";

	for (int j = numOfLines - 1; j >= 0; j--)
	{
		for (int i = hidden[j].length() - 1; i >= 0; i--)	//read in strings backwards at end of evry line
		{
			if (hidden[j].at(i) == '\t' || hidden[j].at(i) == ' ')
				encoded = encoded + hidden[j].at(i);
			else
				break;
		}
	}

	string temp;
	string trueEncoded = string(encoded.rbegin(), encoded.rend()); //reverse to get true encoded message
	//cerr << encoded << endl << trueEncoded;
	vector<unsigned short> decoded;
	if (BinaryConverter::decode(trueEncoded, decoded) == false)	//decode decompress
		return false;
	if (Compressor::decompress(decoded, temp) == false)
		return false;
	msg = temp;
	delete[] hidden;	//delete dynamic items
	return true;
}
