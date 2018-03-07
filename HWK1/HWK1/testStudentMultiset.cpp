#include <iostream>
#include "StudentMultiset.h"

using namespace std;

int main()
{
	StudentMultiset sm;
	sm.add(78);
	sm.add(78);
	sm.add(78);
	sm.add(62);
	sm.add(12);
	cout << sm.size() << endl;
	sm.print();
}