#include "Multiset.h"

Multiset::Multiset() // Create an empty multiset. 
{
	m_count = 0;
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
		m_array[i].count = 0;
}

bool Multiset::empty() const  // Return true if the multiset is empty, otherwise false.
{
	if (m_count == 0)
		return true;
	else
		return false;
}
int Multiset::size() const
{
	int tempsize = 0;
	if (empty())
	{
		return 0;
	}
	else
	{
		for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
			tempsize += m_array[i].count;
		return tempsize;
	}
}
// Return the number of items in the multiset.  For example, the size
// of a multiset containing "cumin", "cumin", "cumin", "turmeric" is 4.

int Multiset::uniqueSize() const
{
	return m_count;
}
// Return the number of distinct items in the multiset.  For example,
// the uniqueSize of a multiset containing "cumin", "cumin", "cumin",
// "turmeric" is 2.

bool Multiset::insert(const ItemType& value)
{
	if (!contains(value) && uniqueSize() + 1 > DEFAULT_MAX_ITEMS)
		return false;
	else if (contains(value))
	{
		for (int i = 0; i < m_count; i++)
		{
			if (value == m_array[i].d_member)
				m_array[i].count++;
		}
	}
	else
	{
		m_array[m_count].d_member = value;
		m_array[m_count].count = 1;
		m_count++;
	}
	return true;
}
// Insert value into the multiset.  Return true if the value was
// actually inserted.  Return false if the value was not inserted
// (perhaps because the multiset has a fixed capacity and is full).

int Multiset::erase(const ItemType& value)
{
	if (contains(value))
	{
		for (int i = 0; i < m_count; i++)
		{
			if (m_array[i].d_member == value)
			{
				m_array[i].count--;
				if (m_array[i].count == 0)
					m_count--;
			}
		}
		return 1;
	}
	else
		return 0;
}

// Remove one instance of value from the multiset if present.
// Return the number of instances removed, which will be 1 or 0.

int Multiset::eraseAll(const ItemType& value)
{
	int temp = 0;
	if (contains(value))
	{
		for (int i = 0; i < m_count; i++)
		{
			if (m_array[i].d_member == value)
			{
				temp = m_array[i].count;
				m_array[i].count = 0;
				m_count--;
			}
		}
		return temp;
	}
	else
		return 0;
}
// Remove all instances of value from the multiset if present.
// Return the number of instances removed.

bool Multiset::contains(const ItemType& value) const
{
	for (int i = 0; i < m_count; i++)
	{
		if (m_array[i].d_member == value)
			return true;
	}
	return false;
}
// Return true if the value is in the multiset, otherwise false.

int Multiset::count(const ItemType& value) const
{
	for (int i = 0; i < m_count; i++)
	{
		if (m_array[i].d_member == value)
			return m_array[i].count;
	}
	return 0;
}
// Return the number of instances of value in the multiset.

int Multiset::get(int i, ItemType& value) const
{
	if (i >= 0 && i < uniqueSize())
	{
		value = m_array[i].d_member;
		return m_array[i].count;
	}
	else
		return 0;
}
// If 0 <= i < uniqueSize(), copy into value an item in the
// multiset and return the number of instances of that item in
// the multiset.  Otherwise, leave value unchanged and return 0.
// (See below for details about this function.)

void Multiset::swap(Multiset& other)
{
	distinct temp[DEFAULT_MAX_ITEMS];
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
		temp[i] = m_array[i];
	for (int j = 0; j < DEFAULT_MAX_ITEMS; j++)
		m_array[j] = other.m_array[j];
}
// Exchange the contents of this multiset with the other one.
