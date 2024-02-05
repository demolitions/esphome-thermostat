/*
	AonLinkedList.h - V1.2 - Tiny modification of the awesome
	Generic LinkedList implementation from Ivan Seidel Gomes
	Works better with FIFO, because LIFO will need to
	search the entire List to find the last one;

	For instructions, go to https://github.com/ivanseidel/LinkedList

	Created by Ivan Seidel Gomes, March, 2013.
	Released into the public domain.
*/

#ifndef AonLinkedList_h
#define AonLinkedList_h

#include <stddef.h>

template <class T>
struct AonListNode
{
	T data;
	AonListNode<T> *next;
};

template <typename T>
class AonLinkedList
{

protected:
	int _size;
	AonListNode<T> *root;
	AonListNode<T> *last;

	// Helps "get" method, by saving last position
	AonListNode<T> *lastNodeGot;
	int lastIndexGot;
	// isCached should be set to FALSE
	// everytime the list suffer changes
	bool isCached;

	AonListNode<T> *getNode(int index);

	AonListNode<T> *findEndOfSortedString(AonListNode<T> *p, int (*cmp)(T &, T &));

public:
	AonLinkedList();
	AonLinkedList(int sizeIndex, T _t); //initiate list size and default value
	~AonLinkedList();

	/*
		Returns current size of AonLinkedList
	*/
	virtual int size();
	/*
		Adds a T object in the specified index;
		Unlink and link the AonLinkedList correcly;
		Increment _size
	*/
	virtual bool add(int index, T);
	/*
		Adds a T object in the end of the AonLinkedList;
		Increment _size;
	*/
	virtual bool add(T);
	/*
		Adds a T object in the start of the AonLinkedList;
		Increment _size;
	*/
	virtual bool unshift(T);
	/*
		Set the object at index, with T;
	*/
	virtual bool set(int index, T);
	/*
		Remove object at index;
		If index is not reachable, returns false;
		else, decrement _size
	*/
	virtual T remove(int index);
	/*
		Remove last object;
	*/
	virtual T pop();
	/*
		Remove first object;
	*/
	virtual T shift();
	/*
		Get the index'th element on the list;
		Return Element if accessible,
		else, return false;
	*/
	virtual T get(int index);

	/*
		Clear the entire array
	*/
	virtual void clear();

	/*
		Sort the list, given a comparison function
	*/
	virtual void sort(int (*cmp)(T &, T &));

	// add support to array brakets [] operator
	inline T &operator[](int index);
	inline T &operator[](size_t &i) { return this->get(i); }
	inline const T &operator[](const size_t &i) const { return this->get(i); }
};

// Initialize AonLinkedList with false values
template <typename T>
AonLinkedList<T>::AonLinkedList()
{
	root = NULL;
	last = NULL;
	_size = 0;

	lastNodeGot = root;
	lastIndexGot = 0;
	isCached = false;
}

// Clear Nodes and free Memory
template <typename T>
AonLinkedList<T>::~AonLinkedList()
{
	AonListNode<T> *tmp;
	while (root != NULL)
	{
		tmp = root;
		root = root->next;
		delete tmp;
	}
	last = NULL;
	_size = 0;
	isCached = false;
}

/*
	Actualy "logic" coding
*/

template <typename T>
AonListNode<T> *AonLinkedList<T>::getNode(int index)
{

	int _pos = 0;
	AonListNode<T> *current = root;

	// Check if the node trying to get is
	// immediatly AFTER the previous got one
	if (isCached && lastIndexGot <= index)
	{
		_pos = lastIndexGot;
		current = lastNodeGot;
	}

	while (_pos < index && current)
	{
		current = current->next;

		_pos++;
	}

	// Check if the object index got is the same as the required
	if (_pos == index)
	{
		isCached = true;
		lastIndexGot = index;
		lastNodeGot = current;

		return current;
	}

	return NULL;
}

template <typename T>
int AonLinkedList<T>::size()
{
	return _size;
}

template <typename T>
AonLinkedList<T>::AonLinkedList(int sizeIndex, T _t)
{
	for (int i = 0; i < sizeIndex; i++)
	{
		add(_t);
	}
}

template <typename T>
bool AonLinkedList<T>::add(int index, T _t)
{

	if (index >= _size)
		return add(_t);

	if (index == 0)
		return unshift(_t);

	AonListNode<T> *tmp = new AonListNode<T>(),
								 *_prev = getNode(index - 1);
	tmp->data = _t;
	tmp->next = _prev->next;
	_prev->next = tmp;

	_size++;
	isCached = false;

	return true;
}

template <typename T>
bool AonLinkedList<T>::add(T _t)
{

	AonListNode<T> *tmp = new AonListNode<T>();
	tmp->data = _t;
	tmp->next = NULL;

	if (root)
	{
		// Already have elements inserted
		last->next = tmp;
		last = tmp;
	}
	else
	{
		// First element being inserted
		root = tmp;
		last = tmp;
	}

	_size++;
	isCached = false;

	return true;
}

template <typename T>
bool AonLinkedList<T>::unshift(T _t)
{

	if (_size == 0)
		return add(_t);

	AonListNode<T> *tmp = new AonListNode<T>();
	tmp->next = root;
	tmp->data = _t;
	root = tmp;

	_size++;
	isCached = false;

	return true;
}

template <typename T>
T &AonLinkedList<T>::operator[](int index)
{
	return getNode(index)->data;
}

template <typename T>
bool AonLinkedList<T>::set(int index, T _t)
{
	// Check if index position is in bounds
	if (index < 0 || index >= _size)
		return false;

	getNode(index)->data = _t;
	return true;
}

template <typename T>
T AonLinkedList<T>::pop()
{
	if (_size <= 0)
		return T();

	isCached = false;

	if (_size >= 2)
	{
		AonListNode<T> *tmp = getNode(_size - 2);
		T ret = tmp->next->data;
		delete (tmp->next);
		tmp->next = NULL;
		last = tmp;
		_size--;
		return ret;
	}
	else
	{
		// Only one element left on the list
		T ret = root->data;
		delete (root);
		root = NULL;
		last = NULL;
		_size = 0;
		return ret;
	}
}

template <typename T>
T AonLinkedList<T>::shift()
{
	if (_size <= 0)
		return T();

	if (_size > 1)
	{
		AonListNode<T> *_next = root->next;
		T ret = root->data;
		delete (root);
		root = _next;
		_size--;
		isCached = false;

		return ret;
	}
	else
	{
		// Only one left, then pop()
		return pop();
	}
}

template <typename T>
T AonLinkedList<T>::remove(int index)
{
	if (index < 0 || index >= _size)
	{
		return T();
	}

	if (index == 0)
		return shift();

	if (index == _size - 1)
	{
		return pop();
	}

	AonListNode<T> *tmp = getNode(index - 1);
	AonListNode<T> *toDelete = tmp->next;
	T ret = toDelete->data;
	tmp->next = tmp->next->next;
	delete (toDelete);
	_size--;
	isCached = false;
	return ret;
}

template <typename T>
T AonLinkedList<T>::get(int index)
{
	AonListNode<T> *tmp = getNode(index);

	return (tmp ? tmp->data : T());
}

template <typename T>
void AonLinkedList<T>::clear()
{
	while (size() > 0)
		shift();
}

template <typename T>
void AonLinkedList<T>::sort(int (*cmp)(T &, T &))
{
	if (_size < 2)
		return; // trivial case;

	for (;;)
	{

		AonListNode<T> **joinPoint = &root;

		while (*joinPoint)
		{
			AonListNode<T> *a = *joinPoint;
			AonListNode<T> *a_end = findEndOfSortedString(a, cmp);

			if (!a_end->next)
			{
				if (joinPoint == &root)
				{
					last = a_end;
					isCached = false;
					return;
				}
				else
				{
					break;
				}
			}

			AonListNode<T> *b = a_end->next;
			AonListNode<T> *b_end = findEndOfSortedString(b, cmp);

			AonListNode<T> *tail = b_end->next;

			a_end->next = NULL;
			b_end->next = NULL;

			while (a && b)
			{
				if (cmp(a->data, b->data) <= 0)
				{
					*joinPoint = a;
					joinPoint = &a->next;
					a = a->next;
				}
				else
				{
					*joinPoint = b;
					joinPoint = &b->next;
					b = b->next;
				}
			}

			if (a)
			{
				*joinPoint = a;
				while (a->next)
					a = a->next;
				a->next = tail;
				joinPoint = &a->next;
			}
			else
			{
				*joinPoint = b;
				while (b->next)
					b = b->next;
				b->next = tail;
				joinPoint = &b->next;
			}
		}
	}
}

template <typename T>
AonListNode<T> *AonLinkedList<T>::findEndOfSortedString(AonListNode<T> *p, int (*cmp)(T &, T &))
{
	while (p->next && cmp(p->data, p->next->data) <= 0)
	{
		p = p->next;
	}

	return p;
}

#endif
