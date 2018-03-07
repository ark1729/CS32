#ifndef HASH_INCLUDED
#define HASH_INCLUDED
template <typename KeyType, typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets, unsigned int capacity)
		:stored(0), capac(capacity), head(nullptr), tail(nullptr), buckets(numBuckets)
	{
		arr = new Node*[buckets];
		for (unsigned int i = 0; i < numBuckets; i++) //Construct Array with empty pointers
			arr[i] = nullptr;
	}
	~HashTable()
	{
		for (int i = 0; i < buckets; i++)
		{
			while (arr[i] != nullptr)
			{
				Node* temp = arr[i]; //Go through array and remove pointers
				if (temp->list == nullptr)
				{
					Node* del = temp->list; //Delete each created pointer
					temp->list = nullptr;
					delete del;
				}
				else temp = temp->list;
			}
		}
		delete[] arr; //Delete Allocated array
	}
	bool isFull() const
	{
		return capac == stored; //If all items are used
	}
	bool set(const KeyType& key, const ValueType& value, bool permanent = false)
	{
		unsigned int computeHash(KeyType);//Prototype declaration
		unsigned int result = computeHash(key); 
		unsigned int buck = result % buckets; //Get bucket for each
		ValueType check;
		if (get(key, check)) //If item is already in table
		{
			Node* tem = arr[buck]; 
			while (tem != nullptr) //Create new association
			{
				if (tem->key == key)
				{
					if (head == tem)
						head = tem->after;
					tem->value = value;
					tem->after->before = tem->before;
					tem->after = nullptr;
					tem->before->after = tem->after;
					tem->before = tail;
					tail = tem;
					return true;
				}
				tem = tem->list;
			}
			return false;
		}
		else if (isFull()) //If not in table and table is full
			return false;
		Node* in = new Node; //Create new node for new association
		in->key = key;
		in->value = value;
		in->list = nullptr; //Insert in table
		stored++;
		Node* search = arr[buck];
		if (search = nullptr)
			arr[buck] = in;
		while (search != nullptr) //go down linked list from bucket
		{
			if (search->list == nullptr)
				search->list = in;
		}
		if (permanent)
			return true; //Permanent is not put in order linked list
		if (head == nullptr)
			head = in;
		in->before = tail; //Order in the linked list
		tail = in;
		return true;
	}
	bool get(const KeyType& key, ValueType& value) const
	{
		unsigned int computeHash(KeyType);	//ComputeHash prototype
		unsigned int result = computeHash(key);
		unsigned int buck = result % buckets; //get the number of buckets
		Node* tem = arr[buck];
		while (tem != nullptr) //Check correct bucket
		{						// Go through linked list
			if (tem->key == key)
			{
				value = tem->value; //Find item
				return true;
			}
			tem = tem->list;
		}
		return false; //Did not find item
	}
	bool touch(const KeyType& key)
	{
		ValueType check;
		if (!get(key, check)) //See if item is in Table
			return false;
		unsigned int computeHash(KeyType);
		unsigned int result = computeHash(key);
		unsigned int buck = result % buckets; //Find the bucket
		Node* tem = arr[buck];
		while (tem != nullptr)
		{
			if (tem->key == key) //Find the item
			{
				if (head == tem && tem->after != nullptr)
					head = tem->after;
				tem->after->before = tem->before;
				tem->after = nullptr; //Change order linked list and fix pointers
				tem->before->after = tem->after;
				tem->before = tail;
				tail = tem;
				return true;
			}
		}
		return true;
	}
	bool discard(KeyType& key, ValueType& value)
	{
		if (head == nullptr) //See if any non permanent items
			return false;
		key = head->key;
		value = head->value;
		Node* del = head; //Find oldest items
		head = head->after;
		delete del; //Remove oldest item
		stored--; //decrease capacity
		return true;
	}

private:
	struct Node
	{
		KeyType key;
		ValueType value;
		Node* list; //Used for table
		Node* before; //Used for ordering
		Node* after;
	};
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&); //Cannot use
	Node* head;
	Node** arr;
	Node* tail; //Used for going through ordering
	int stored;
	const int capac;
	int buckets;
};

#endif