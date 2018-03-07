#ifndef HASH
#define HASH

template <typename KeyType, typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets, unsigned int capacity)
		:m_capacity(capacity), m_entries(0), m_young(nullptr), m_old(nullptr), buckets(numBuckets)
	{
		ht = new Node*[numBuckets];
		for (int i = 0; i < numBuckets; i++)   //Construct empty pointers
			ht[i] = nullptr;
	}
	~HashTable()
	{
		for (int i = 0; i < buckets; i++)
		{
			while (ht[i] != nullptr)
			{
				Node* temp = ht[i];
				if (temp->colList == nullptr)
				{
					Node* toKill = temp->colList; //Delete each pointer
					temp->colList = nullptr;
					delete toKill;
				}
				else temp = temp->colList;
			}
		}
		delete[] ht;
	}

	bool isFull()	const
	{
		return m_entries == m_capacity;
	}
	bool set(const KeyType&	key, const ValueType& value, bool permanent = false)
	{
		unsigned int computeHash(KeyType); //Prototype declaration
		unsigned int result = computeHash(key);
		unsigned int slot = result % buckets; 
		ValueType check;
		if (get(key, check))
		{
			Node* temp = ht[slot];
			while (temp != nullptr) //Create new association
			{
				if (temp->key == key)
				{
					if (m_old == temp)
						m_old = temp->after;
					temp->value = value;
					temp->after->before = temp->before;
					temp->after = nullptr;
					temp->before->after = temp->after;
					temp->before = m_young;
					m_young = temp;
					return true;
				}
				temp = temp->colList;
			}
			return false;
		}
		if (isFull())
			return false;
		Node* ins = new Node; //Create new node for new association
		ins->key = key;
		ins->value = value;
		ins->colList = nullptr; //Insert in table
		m_entries++;
		Node* check = ht[slot];
		if (check = nullptr)
			ht[slot] = ins;
		while (check != nullptr) //follow linked list from bucket
		{
			if (check->colList == nullptr)
				check->colList = in;
			check = check->colList;
		}
		if (permanent)
			return true; //Permanent is not put in order for deleteion
		if (m_old == nullptr)
			m_old = ins;
		ins->before = m_young; //Order of objects in hash table
		m_young = ins;
		return true;
	}
	bool get(const KeyType& key, ValueType& value)	const
	{
		unsigned int computeHash(KeyType); //ComputeHash prototype
		unsigned int result = computeHash(key);
		unsigned int slot = result % buckets; 
		Node* temp = ht[slot];
		while (temp != nullptr) //Check key bucket
		{						// Go through linked list at bucket
			if (temp->key == key)
			{
				value = temp->value; //set value if found
				return true;
			}
			temp = temp->colList;
		}
		return false; 
	}
	bool touch(const KeyType& key)
	{
		ValueType check;
		if (!get(key, check)) //See if item exists in hash table
			return false;
		unsigned int computeHash(KeyType);
		unsigned int result = computeHash(key);
		unsigned int slot = result % buckets; 
		Node* temp = ht[slot];
		while (temp != nullptr)
		{
			if (temp->key == key) //item found
			{
				if (m_old == temp && temp->after != nullptr)
					m_old = temp->after;
				temp->after->before = temp->before;
				temp->after = nullptr; //Change order of associatopns
				temp->before->after = temp->after;
				temp->before = m_young;
				m_young = temp;
				return true;
			}
		}
		return true;
	}
	bool discard(KeyType& key, ValueType& value)
	{
		if (m_old == nullptr) //check if non permanent value exists
			return false;
		key = m_old->key;
		value = m_old->value;
		Node* toKill = m_old; //Find oldest items
		m_old = m_old->after;
		delete toKill; //Remove oldest item
		m_entries--; //decrease entries
		return true;
	}
private:
	//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
	//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);
	int m_capacity;
	int m_entries;
	int buckets;
	
	struct Node
	{
		KeyType key;
		ValueType value;
		bool permanence;
		Node* colList;
		Node* after;
		Node* before;
	};
	Node** ht;
	Node* m_young;	//point to youngest association
	Node* m_old;	//point to oldest association
};

#endif //HASH