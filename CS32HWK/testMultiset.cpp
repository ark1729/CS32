/*#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Multiset ms;
	assert(ms.empty());
	string x = "dill";
	assert(ms.get(0, x) == 0 && x == "dill");  // x unchanged by get failure
	assert(!ms.contains("tamarind"));
	ms.insert("tamarind");
	ms.insert("tamarind");
	assert(ms.size() == 2 && ms.uniqueSize() == 1);
	assert(ms.get(1, x) == 0 && x == "dill");  // x unchanged by get failure
	assert(ms.get(0, x) == 2 && x == "tamarind");
	cout << "Passed all tests" << endl;
}*/

#include "Multiset.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

/*int main()
{
	Multiset ms;
	ms.insert("fennel");
	ms.insert("fennel");
	ms.insert("fenugreek");
	ms.insert("fennel");
	for (int k = 0; k < ms.uniqueSize(); k++)
	{
		string x;
		int n = ms.get(k, x);
		cout << x << " occurs " << n << " times." << endl;
	}
}*/


void test()
{
	Multiset ulms;
	assert(ulms.insert(20));
	assert(ulms.insert(10));
	assert(ulms.insert(20));
	assert(ulms.insert(30));
	assert(ulms.insert(20));
	assert(ulms.insert(10));
	assert(ulms.size() == 6 && ulms.uniqueSize() == 3);
	assert(ulms.count(10) == 2);
	assert(ulms.count(20) == 3);
	assert(ulms.count(30) == 1);
	assert(ulms.count(40) == 0);
}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}