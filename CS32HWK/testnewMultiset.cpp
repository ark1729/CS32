
#include "newMultiset.h"
#include <iostream>
#include <cassert>
using namespace std;

/*int main()
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

/*#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
Multiset ms;
assert(ms.empty());
unsigned long x = 999;
assert(ms.get(0, x) == 0 && x == 999);  // x unchanged by get failure
assert(!ms.contains(42));
ms.insert(42);
ms.insert(42);
assert(ms.size() == 2 && ms.uniqueSize() == 1);
assert(ms.get(1, x) == 0 && x == 999);  // x unchanged by get failure
assert(ms.get(0, x) == 2 && x == 42);
cout << "Passed all tests" << endl;
}*/
int main()
{
	Multiset a(1000);   // a can hold at most 1000 distinct items
	Multiset b(5);      // b can hold at most 5 distinct items
	Multiset c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
	ItemType v[6] = { 1,2,3,4,5,6 };
	// No failures inserting 5 distinct items twice each into b
	for (int k = 0; k < 5; k++)
	{
		assert(b.insert(v[k]));
		assert(b.insert(v[k]));
	}
	assert(b.size() == 10 && b.uniqueSize() == 5 && b.count(v[0]) == 2);
	// Failure if we try to insert a sixth distinct item into b
	assert(!b.insert(v[5]));

	// When two Multisets' contents are swapped, their capacities are swapped
	// as well:
	a.swap(b);
	assert(!a.insert(v[5]) && b.insert(v[5]));

	cout <<  endl << "pass" << endl;
}
