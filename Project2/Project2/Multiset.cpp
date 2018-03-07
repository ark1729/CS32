#include "Multiset.h"

void exchange(int& a, int& b)	//exchange function for switching size values of multisets
{
	int t = a;
	a = b;
	b = t;
}

Multiset::Multiset()		//empty multiset head pointer to null
	:m_size(0), m_unique(0)
{
	head = nullptr;
}
Multiset::~Multiset()		//destructor to properly remove list
{
	Node* p;
	p = head;
	while (p != nullptr)
	{
		Node* n = p->next;
		delete p;
		p = n;
	}
}
Multiset::Multiset(const Multiset& other) //copy constructor
{
	if (other.empty())	//copy empty multiset head pointer to null
		head = nullptr;
	else					//otherwise copy nodes of other
	{
		Node* p;
		for (p = other.head; p->next != nullptr; p = p->next) //from back as insert acts like stack
			;
		head = nullptr;
		for (p; p != nullptr; p = p->prev)
		{
			for (int i = 0; i < p->count; i++)
			{
				insert(p->value);
			}
		}
	}
	
}

Multiset& Multiset::operator=(const Multiset& rhs)		//copy swap implementation
{
	if (this != &rhs)		//check aliasing
	{
		Multiset temp(rhs);
		swap(temp);
	}
	return *this;
}

bool Multiset::empty() const
{
	return size() == 0;
}
int Multiset::size() const
{
	return m_size;
}
int Multiset::uniqueSize() const
{
	return m_unique;
}
bool Multiset::insert(const ItemType& value)
{
	Node* p; 
	for (p = head; p != nullptr && p->value != value; p = p->next) //check for value
		;
	if (p != nullptr) //if not null value exists increment count of value and size of multiset
	{
		p->count++;
		m_size++;
	}
	else if (head == nullptr) //if empty multiset create first node
	{
		Node* n = new Node;
		n->value = value;
		n->count = 1;
		n->next = nullptr;
		n->prev = nullptr;
		head = n;
		m_size++;
		m_unique++;
	}
	else				//else add node on top becomes head
	{
		Node* n = new Node;
		n->value = value;
		n->count = 1;
		n->next = head;
		n->prev = nullptr;
		head->prev = n;
		head = n;
		m_size++;
		m_unique++;
	}
	return true; //always returns true
}
int Multiset::erase(const ItemType& value)
{
	Node* p;
	for (p = head; p != nullptr && p->value != value; p = p->next) //check for value
		;
	if (p == nullptr)  //return 0 if no value
		return 0;
	else
	{
		p->count--;  //decrement count of value
		m_size--;	 //decrement size of multiset
		if (p->count == 0 && p->next == nullptr && p == head) //if value has 0 instances remove 
		{														//case with only one node
			delete p;
			head = nullptr;
			m_unique--;
		}
		else if (p->count == 0 && p->next == nullptr) //case if tail deleted
		{
			p->prev->next = p->next;
			delete p;
			m_unique--;
		}
		else if (p->count == 0 && p == head) //case if head deleted
		{
			p->next->prev = p->prev;
			head = p->next;
			delete p;
			m_unique--;
		}
		else if (p->count == 0) //case if middle node deleted
		{
			p->next->prev = p->prev;
			p->prev->next = p->next;
			delete p;
			m_unique--;
		}
		
		return 1; //always returns 1 if value in multiset
	}
}
int Multiset::eraseAll(const ItemType& value)
{
	Node* p;
	for (p = head; p != nullptr && p->value != value; p = p->next) //find value
		;
	if (p == nullptr)  //no value return 0
		return 0;
	else
	{
		if (p->next == nullptr && p == head) //case if one node
		{
			int n = p->count;
			delete p;
			head = nullptr;
			m_unique--;  //decrement number of unique items
			m_size -= n; //reduce size by number of items deleted
			return n;
		}
		else if (p->next == nullptr) //case if tail node
		{
			int n = p->count;
			p->prev->next = p->next;
			delete p;
			m_unique--;
			m_size -= n;
			return n;
		}
		else if (p == head) //case if head node
		{
			int n = p->count;
			p->next->prev = p->prev;
			head = p->next;
			delete p;
			m_unique--;
			m_size -= n;
			return n;
		}
		else //case if middle node
		{
			int n = p->count;
			p->next->prev = p->prev;
			p->prev->next = p->next;
			delete p;
			m_unique--;
			m_size -= n;
			return n;
		}
	}
}
bool Multiset::contains(const ItemType& value) const
{
	Node* p;
	for (p = head; p != nullptr && p->value != value; p = p->next) //find value
		;
	if (p == nullptr) //if no value not contained return false
		return false;
	else
		return true;

}
int Multiset::count(const ItemType& value) const
{
	Node* p;
	for (p = head; p != nullptr && p->value != value; p = p->next) //find value
		;
	if (p == nullptr) //no value so count is 0
		return 0;
	else
		return p->count; //if value exists look at node for count
}
int Multiset::get(int i, ItemType& value) const //finds value at position i returns amount of value
{
	if (i < 0 || i >= m_unique)
	{
		return 0;
	}
	else
	{
		Node* p;
		p = head;
		for (int j = 0; p != nullptr && j != i; p = p->next, j++)
			;
		value = p->value;
		return p->count;
	}
}
void Multiset::swap(Multiset& other) //swap head pointers and size information
{
	Node* temphead = head;
	head = other.head;
	other.head = temphead;

	exchange(m_unique, other.m_unique);
	exchange(m_size, other.m_size);
}



void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	ItemType x;
	Multiset mst(ms1); //copy first multiset in temp multiset
	for (int i = 0; i < ms2.uniqueSize(); i++)
	{
		for (int j = 0; j < ms2.get(i, x); j++)
		{
			mst.erase(x);						//remove values that occur in ms2 from mst aka ms1
		}
	}
	mst.swap(result); //swap temp with result to run with aliasing 

}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	ItemType x;
	Multiset mst(ms1); 
	for (int i = 0; i < ms2.uniqueSize(); i++)
	{
		for (int j = 0; j < ms2.get(i, x); j++)
		{
			mst.insert(x); //add values that occur in ms2 to mst
		}
	}
	mst.swap(result);
}