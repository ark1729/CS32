// implements a queue (FIFO ADT) using a linked list

#include <iostream>
using namespace std;

struct node
{
	int info;
	node *next;
};

class Queue
{
  private:
	node *rear;
	node *front;
  public:
	Queue();
	void enqueue();
	void dequeue();
	void display();
};

Queue::Queue()
{
	rear = NULL;
	front = NULL;
}

void Queue::enqueue()
{
	int data;
	node* temp = new node;
	cout << "Enter the data to enqueue: ";
	cin >> data;
	temp->info = data;
	temp->next = NULL;
	if (front == NULL)
		front = temp;
	else
		rear->next = temp;
	rear = temp;
}

void Queue::dequeue()
{
	node* temp;
	if (front == NULL)
		cout << "The queue is empty\n";
	else
	{
		temp = front;
		front = front->next;
		cout << "The data dequeued is " << temp->info << endl;
		delete temp;
	}
}

void Queue::display()
{
	node* p;
	p = front;
	if (front == NULL)
		cout << "Nothing to Display\n";
	else
	{
		cout << "Here is your queue: ";
		while (p != NULL)
		{
			cout << p->info << " ";
			p = p->next;
		}
		cout << endl;
	}
}

void main()
{
	Queue q;
	int c;
	while (true){
		cout << "\nEnter your choice (1->Enqueue, 2->Dequeue, 3->Display, 4->Quit): ";
		cin >> c;
		switch(c){
		case 1:
			q.enqueue();
			break;
		case 2:
			q.dequeue();
			break;
		case 3:
			q.display();
			break;
		case 4:
			return;
			break;
		default:
			cout << "Invalid Input! \n";
		}
	}
	return;
}