#include "StudentMultiset.h"

StudentMultiset::StudentMultiset() //Create an empty student multiset.
{
	m_multiset = Multiset();
}
bool StudentMultiset::add(unsigned long id)
{
	if(m_multiset.insert(id))
		return true;
}
// Add a student id to the StudentMultiset.  Return true if and only
// if the id was actually added.

int StudentMultiset::size() const
{
	return m_multiset.size();
}
// Return the number of items in the StudentMultiset.  If an id was
// added n times, it contributes n to the size.

void StudentMultiset::print() const
{
	unsigned long x;
	int y = 0;
	for (int i = 0; i < m_multiset.uniqueSize(); i++)
	{
		y = m_multiset.get(i, x);
		for (int j = 0; j < y; j++)
			std::cout << x << std::endl;
	}
}
// Print to cout every student id in the StudentMultiset one per line;
// print as many lines for each id as it occurs in the StudentMultiset.
