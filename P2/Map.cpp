#include "Map.h"
#include <iostream>



Map::Map()
: m_size(0)
{
    //set head to "dummy" node
    m_head = new Node;
    m_head->m_prev = m_head;
    m_head->m_next = m_head;
    
}

Map::Map(const Map& other)
: m_size(0)
{
    //create "dummy" node
    m_head = new Node;
    m_head->m_prev = m_head;
    m_head->m_next = m_head;

    // copy the individual nodes
    for (Node* p = other.m_head->m_next; p != other.m_head; p = p->m_next)
        insert(p->m_data.m_key, p->m_data.m_value);
    
}

Map::~Map()
{
    while (!empty()) {
        //delete 1st real node
        KeyType key = m_head->m_next->m_data.m_key;
        erase(key);
    }
    //delete dummy variable
    delete m_head;
}

Map& Map::operator=(const Map& rhs)
{
    if (this != &rhs)
    {
        Map temp(rhs);
        swap(temp);
    }
    m_size = rhs.m_size;
    return *this;
}

bool Map::erase(const KeyType& key)
{
    
    Node* n = find(key);
    if ( n == nullptr)  // not found
        return false;
    
    m_size--;
    Node* prev = n->m_prev;
    Node* next = n->m_next;
    prev->m_next = next;
    next->m_prev = prev;
    
    delete n;
    
    return true;
     
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* n = find(key);
    if (n == nullptr)  // not found
        return false;
    value = n->m_data.m_value;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    
    if (i < 0 || i >= m_size)
        return false;
    //must be valid index past this point
    Node* n = m_head->m_next;
    
    for (int j = 0; n!=m_head && j!=i; n= n->m_next,j++)
        ;
    key = n->m_data.m_key;
    value = n->m_data.m_value;
    
    return true;
}

void Map::swap(Map& other)
{
    //switch head pointers
   
    Node *oldHead1 = m_head;
    m_head = other.m_head;
    other.m_head = oldHead1;
    
    //switch sizes
    int oldSize = m_size;
    m_size = other.m_size;
    other.m_size = oldSize;
}

Map::Node* Map::find(const KeyType& key) const
{
    for (Node* p = m_head->m_next; p != m_head; p = p->m_next)
        if (p->m_data.m_key == key)
            return p;
    
    return nullptr;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value,
                           bool mayInsert, bool mayUpdate)
{
    Node *desiredNode = find(key);
    if (desiredNode != nullptr) //found
    {
        if (mayUpdate)
            desiredNode->m_data.m_value = value;
        
        return mayUpdate;
    }
    
    if (!mayInsert)  // not found, and not allowed to insert
        return false;
    
    //insert to back
    
    Node* n = new Node;
    n->m_data.m_key = key;
    n->m_data.m_value = value;
    n->m_next = m_head;
    n->m_prev = m_head->m_prev;
    m_head->m_prev->m_next = n;
    m_head->m_prev = n;
    m_size++;
 
    return true;
}

bool combine(const Map& m1, const Map& m2, Map& result) {
    
    //default return is true
    bool allKeysAdded = true;
    
    // same map
    if (&m1 == &m2) {
        result = m1;
        return allKeysAdded;
    }
    //determine which map to append first
    const Map* startingMap = (&m2 == &result ? &m2 : &m1);
    const Map* secondMap = (&m2 == &result ? &m1 : &m2);
    
    //set result to one of the maps
    if (&result != startingMap)
        result = *startingMap;
    
    //add values of other map
    for (int i = 0; i < secondMap->size(); i++) {
        KeyType k;
        ValueType v;
        secondMap->get(i,k,v);
        
        //check if key already added
        if (startingMap->contains(k)) {
            ValueType v1;
            startingMap->get(k, v1);
            //if different values for same key, remove the key from the result
            if (v1 != v) {
                result.erase(k);
                allKeysAdded = false;
            }
        }
        else
            result.insert(k, v);
            
    }
    
    return allKeysAdded;
}

void subtract(const Map& m1, const Map& m2, Map& result) {
    Map resultCopy(m1);
    
    for (int i = 0; i < m2.size(); i++) {
        KeyType k;
        ValueType v;
        m2.get(i,k,v);
        
        if (m1.contains(k))
            resultCopy.erase(k);
    }
    
    result = resultCopy;
    
}
