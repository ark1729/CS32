#ifndef MULTISET   //include guard
#define MULTISET

#include <string>
#include "OurType.h"
typedef OurType ItemType;

class Multiset
{
public:
	Multiset();
	~Multiset();								//deconstructor
	Multiset(const Multiset& other);			//copy constructor
	Multiset& operator=(const Multiset& rhs);	//= operator
	bool empty() const;
	int size() const;
	int uniqueSize() const;
	bool insert(const ItemType& value);
	int erase(const ItemType& value);
	int eraseAll(const ItemType& value);
	bool contains(const ItemType& value) const;
	int count(const ItemType& value) const;
	int get(int i, ItemType& value) const;
	void swap(Multiset& other);
private:
	struct Node			//private node
	{
		ItemType value;		//value of node
		Node* next;			//pointer to next node
		Node* prev;			//pointer to previous node
		int count;			//number of values in multiset
	};
	Node* head;		//head pointer
	int m_unique;	//number of unique items in multiset
	int m_size;		//number of items in multiset
};


void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result);

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result);

#endif //MULTISET