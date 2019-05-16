#ifndef LIST_H
#define LIST_H

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Rectangle {
	// Declare the variables
private:

	double length;

	double width;

	// Constructors
public:

	explicit Rectangle(double l = 0.0, double w = 0.0) : length{ l }, width{ w }
	{ }

	// Returns the length
	double getLength() const
	{
		return length;
	}

	// Returns the width
	double getWidth() const
	{
		return width;
	}

	// Returns the area
	double getArea() const
	{
		return length * width;
	}

	// Returns the perimeter
	double getPerimeter() const
	{
		return 2 * (length + width);
	}

	// Output
	void print(ostream & out = cout) const
	{
		out << getLength() << " " << getWidth();
	}
	bool operator< (const Rectangle & rhs) const
	{
		return (getLength() < rhs.getLength()) and (getWidth() < rhs.getWidth());
	}
};

ostream& operator<< (ostream& out, const Rectangle& rhs)
{
	rhs.print(out);

	return out;
}

template <typename Object>
class List
{
private:
	// The basic doubly linked list node.
	// Nested inside of List, can be public
	// because the Node is itself private
	struct Node
	{
		Object data;

		Node* next;

		Node(const Object& d = Object{ }, Node* n = nullptr) : data{ d }, next{ n } { }

		Node(Object&& d, Node* n = nullptr) : data{ std::move(d) }, next{ n } { }
	};

public:
	class const_iterator
	{
	public:

		// Public constructor for const_iterator.
		const_iterator() : current{ nullptr }
		{ }

		// Return the object stored at the current position.
		// For const_iterator, this is an accessor with a
		// const reference return type.
		const Object& operator* () const
		{
			return retrieve();
		}

		const_iterator& operator++ ()
		{
			current = current->next;
			return *this;
		}

		const_iterator operator++ (int)
		{
			const_iterator old = *this;
			++(*this);
			return old;
		}

		const_iterator operator+ (int k)
		{
			const_iterator old = *this;
			for (int i = 0; i < k; i++)
			{
				++(*this);
			}
			return old;
		}

		const_iterator& operator-- ()
		{
			for (iterator i = head; i.current != current; i++)
			{
				current = i.current;
			}
			return *this;
		}

		const_iterator operator-- (int)
		{
			const_iterator old = *this;
			--(*this);
			return old;
		}

		const_iterator operator- (int k)
		{
			const_iterator old = *this;
			for (int i = 0; i < theSize - k; i++)
			{
				--(*this);
			}
			return old;
		}

		bool operator== (const const_iterator& rhs) const
		{
			return current == rhs.current;
		}

		bool operator!= (const const_iterator& rhs) const
		{
			return !(*this == rhs);
		}

	protected:
		Node* current;

		// Protected helper in const_iterator that returns the object
		// stored at the current position. Can be called by all
		// three versions of operator* without any type conversions.
		Object& retrieve() const
		{
			return current->data;
		}

		// Protected constructor for const_iterator.
		// Expects a pointer that represents the current position.
		const_iterator(Node * p) : current{ p }
		{ }

		friend class List<Object>;
	};

	class iterator : public const_iterator
	{
	public:

		// Public constructor for iterator.
		// Calls the base-class constructor.
		// Must be provided because the private constructor
		// is written; otherwise zero-parameter constructor
		// would be disabled.
		iterator()
		{ }

		Object& operator* ()
		{
			return const_iterator::retrieve();
		}

		// Return the object stored at the current position.
		// For iterator, there is an accessor with a
		// const reference return type and a mutator with
		// a reference return type. The accessor is shown first.
		const Object& operator* () const
		{
			return const_iterator::operator*();
		}

		iterator& operator++ ()
		{
			this->current = this->current->next;
			return *this;
		}

		iterator operator++ (int)
		{
			iterator old = *this;
			++(*this);
			return old;
		}

		iterator& operator-- ()
		{
			Node* p = new Node;
			for (iterator i = head; i.current != this->current; i++)
			{
				p = i.current;
			}
			this->current = p->next;
			return *this;
		}

		iterator operator-- (int)
		{
			iterator old = *this;
			--(*this);
			return old;
		}

	protected:
		// Protected constructor for iterator.
		// Expects the current position.
		iterator(Node* p) : const_iterator{ p }
		{ }

		friend class List<Object>;
	};

public:
	List()
	{
		init();
	}

	~List()
	{
		clear();
		delete head;
		delete tail;
	}

	List(const List& rhs)
	{
		init();
		for (auto& x : rhs)
			push_back(x);
	}

	List& operator= (const List & rhs)
	{
		List copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	List(List && rhs)
		: theSize{ rhs.theSize }, head{ rhs.head }, tail{ rhs.tail }
	{
		rhs.theSize = 0;
		rhs.head = nullptr;
		rhs.tail = nullptr;
	}

	List& operator= (List && rhs)
	{
		std::swap(theSize, rhs.theSize);
		std::swap(head, rhs.head);
		std::swap(tail, rhs.tail);

		return *this;
	}

	// Return iterator representing beginning of list.
	// Mutator version is first, then accessor version.
	iterator begin()
	{
		return iterator(head->next);
	}

	const_iterator begin() const
	{
		return const_iterator(head->next);
	}

	// Return iterator representing endmarker of list.
	// Mutator version is first, then accessor version.
	iterator end()
	{
		return iterator(tail);
	}

	const_iterator end() const
	{
		return const_iterator(tail);
	}

	// Return number of elements currently in the list.
	int size() const
	{
		return theSize;
	}

	// Return true if the list is empty, false otherwise.
	bool empty() const
	{
		return size() == 0;
	}

	void clear()
	{
		while (!empty())
			pop_front();
	}

	// front, back, push_front, push_back, pop_front, and pop_back
	// are the basic double-ended queue operations.
	Object& front()
	{
		return *begin();
	}

	const Object& front() const
	{
		return *begin();
	}

	Object& back()
	{
		return *--end();
	}

	const Object& back() const
	{
		return *--end();
	}

	void push_front(const Object & x)
	{
		insert(begin(), x);
	}

	void push_back(const Object & x)
	{
		insert(end(), x);
	}

	void push_front(Object && x)
	{
		insert(begin(), std::move(x));
	}

	void push_back(Object && x)
	{
		insert(end(), std::move(x));
	}

	void pop_front()
	{
		erase(begin());
	}

	void pop_back()
	{
		erase(--end());
	}

	// Insert x before itr.
	iterator insert(iterator itr, const Object & x)
	{
		Node* p = new Node;
		Node* n = itr.current;
		for (iterator i = head; i.current != n; i++)
		{
			p = i.current;
		}
		++theSize;
		return iterator(p->next = new Node{ x, n });
	}

	// Insert x before itr.
	iterator insert(iterator itr, const Object && x)
	{
		Node* p = new Node;
		Node* n = itr.current;
		for (iterator i = head; i.current != n; i++)
		{
			p = i.current;
		}
		++theSize;
		return iterator(p->next = new Node{ std::move(x), n });
	}

	// Erase item at itr.
	iterator erase(iterator itr)
	{
		Node* p = new Node;
		Node* n = itr.current;
		iterator retVal(n->next);
		for (iterator i = head; i.current != n; i++)
		{
			p = i.current;
		}
		p->next = n->next;
		delete n;
		--theSize;

		return retVal;
	}

	iterator erase(iterator from, iterator to)
	{
		for (iterator itr = from; itr != to; )
			itr = erase(itr);

		return to;
	}

private:
	int theSize;
	Node* head;
	Node* tail;

	void init()
	{
		theSize = 0;
		head = new Node;
		tail = new Node;
		head->next = tail;
		tail->next = head;
	}
};

int main()
{
	float lij, wij;

	List<Rectangle> L, L1, L2, L3;

	List<Rectangle>::iterator itr = L.begin(), itr1 = L1.begin(), itr2 = L1.begin(), itr3 = L3.begin();

	Rectangle L1R = *itr1, L2R = *itr2, L3R = *itr3;

	for (int j = 0; j < 9; j++)
	{
		cin >> lij;
		cin >> wij;
		L1.push_back(Rectangle{ lij,wij });
	}

	for (int j = 0; j < 9; j++)
	{
		cin >> lij;
		cin >> wij;
		L2.push_back(Rectangle{ lij,wij });
	}

	if (L1.size() != L2.size())
	{
		cout << "Lists must be the same size" << endl;

		assert(L1.size() == L2.size());

		return 1;

		exit(1);
	}

	for (int j = 0; j < 9; j++)
	{
		cin >> lij;
		cin >> wij;
		L3.push_back(Rectangle{ lij,wij });
	}

	if (L2.size() != L3.size())
	{
		cout << "Lists must be the same size" << endl;

		assert(L2.size() == L3.size());

		return 1;

		exit(1);
	}

	cout << "(a)" << "\n";

	cout << "L1: ";
	for (itr1 = L1.begin(); itr1 != L1.end(); itr1++)
	{
		cout << *itr1 << "  ";
	}

	cout << "\n";
	cout << "\n";

	cout << "L2: ";
	for (itr2 = L2.begin(); itr2 != L2.end(); itr2++)
	{
		cout << *itr2 << "  ";
	}

	cout << "\n";
	cout << "\n";

	cout << "L3: ";
	for (itr3 = L3.begin(); itr3 != L3.end(); itr3++)
	{
		cout << *itr3 << "  ";
	}

	cout << "\n";
	cout << "\n";
	cout << "\n";

	itr1 = L1.begin();
	itr3 = L3.begin();

	L = L1;

	while ((itr1 != L1.end()) && (itr3 != L3.end()))
	{
		L1R = *itr1;
		L3R = *itr3;

		if ((L1R.getLength() != L3R.getLength()) || (L1R.getWidth() != L3R.getWidth()))
		{
			L.push_back(L3R);
		}

		itr1++;
		itr3++;
	}

	L1 = L;

	itr1 = L1.begin();

	cout << "(b)" << "\n";

	cout << "L1: ";
	for (itr1 = L1.begin(); itr1 != L1.end(); itr1++)
	{
		cout << *itr1 << "  ";
	}

	cout << "\n";
	cout << "\n";
	cout << "\n";

	itr2 = L2.begin();
	itr3 = L3.begin();

	for (itr2 = L2.begin(); itr2 != L2.end(); itr2++)
	{
		for (itr3 = L3.begin(); itr3 != L3.end(); itr3++)
		{
			L2R = *itr2;
			L3R = *itr3;
			if ((L2R.getLength() == L3R.getLength()) && (L2R.getWidth() == L3R.getWidth()))
			{
				L2.erase(itr2);
				itr2 = L2.begin();
				itr3 = L3.begin();
			}
		}
	}

	itr2 = L2.begin();

	cout << "(c)" << "\n";

	cout << "L3: ";
	for (itr2 = L2.begin(); itr2 != L2.end(); itr2++)
	{
		cout << *itr2 << "  ";
	}

	return 0;
}

#endif