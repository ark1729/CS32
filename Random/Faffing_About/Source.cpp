#include <iostream>
using namespace std;

int main()
{
	int foo[5];
	for (int i = 1; i <= 5; i++)
		cout << foo[i - 1];
	cout << endl << sizeof(foo);
}