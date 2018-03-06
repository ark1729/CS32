// implements a stack (LIFO ADT) using a linked list

#include <iostream>
using namespace std;

struct node
{
	int m_data;
	node *m_next;
};

class Stack
{
  public:
	Stack();
	void push(int data);
	int pop();
  private:
	node *top;
};

Stack::Stack()
{
	top = NULL;
}

void Stack::push(int data)
{
	node* item = new node;
	item->m_data = data;
	item->m_next = NULL;

	if(top == NULL)
		top = item;
	else
	{
		item->m_next = top;
		top = item; 
	}
}

int Stack::pop()
{
	node* temp = top;
	if(temp != NULL)
		top = temp->m_next;
	int num = temp->m_data;
	delete temp;
	return num;
}

void main()
{
	Stack* s = new Stack;
	node *temp;

	s->push(1);
	s->push(3);
	s->push(5);

	cout << "the elements (in reversed order, because it's a stack) are the following: " << endl;
	cout << s->pop() << " ";
	cout << s->pop() << " ";
	cout << s->pop() << endl;

	return;
}