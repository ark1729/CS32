#include "provided.h"
#include <string>
#include <vector>
#include <iostream> //for cerr I added
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	string out;
	string temp;
	for (int i = 0; i < numbers.size(); i++)
	{
		temp += convertNumberToBitString(numbers[i]);	//encode numbers as space and tab using function given
	}
	//cerr << temp << endl;
	for (int j = 0; j < temp.length(); j++)
	{
		if (temp[j] == '0')
			out += ' ';
		if (temp[j] == '1')
			out += '\t';
	}
	//cerr << out << endl;
	return out;  // This compiles, but may not be correct
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
	if (bitString.length() % 16 != 0)	//check if coorect size of string 
		return false;
	//cerr << bitString.length() % 16;
	int j = 1;
	for (int i = 1; i <= bitString.length() / 16; i++)	//run through for each independent number
	{
		string temp;
		
		for (; j <= i*16; j++)
		{
			if (bitString[j-1] == '\t')	//turn white space into 01 string
				temp += '1';
			else if (bitString[j-1] == ' ')
				temp += '0';
			else
				return false;
		}
		unsigned short value;
		convertBitStringToNumber(temp, value);
		numbers.push_back(value);
	}	
	return true;
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}


