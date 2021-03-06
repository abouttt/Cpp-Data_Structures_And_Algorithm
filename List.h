#pragma once

#include <cassert>

template<typename T>
class List
{
private:
	class Node
	{
	public:
		friend class List;

	public:
		Node()
			: mPrev(nullptr)
			, mNext(nullptr)
			, mData(T())
		{}

		Node(const T& value)
			: mPrev(nullptr)
			, mNext(nullptr)
			, mData(value)
		{}

		Node(T&& value) noexcept
			: mPrev(nullptr)
			, mNext(nullptr)
			, mData(std::move(value))
		{}

	private:
		Node* mPrev;
		Node* mNext;
		T mData;
	};

	class Iterator
	{
	public:
		Iterator()
			: mNode(nullptr)
		{}

		Iterator(Node* node)
			: mNode(node)
		{}

		Iterator& operator++()
		{
			mNode = mNode->mNext;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator temp = *this;
			mNode = mNode->mNext;
			return temp;
		}

		Iterator& operator--()
		{
			mNode = mNode->mPrev;
			return *this;
		}

		Iterator operator--(int)
		{
			Iterator temp = *this;
			mNode = mNode->mPrev;
			return temp;
		}

		T& operator*()
		{
			return mNode->mData;
		}

		bool operator==(const Iterator& other)
		{
			return mNode == other.mNode;
		}

		bool operator!=(const Iterator& other)
		{
			return mNode != other.mNode;
		}

	private:
		Node* mNode;
	};

public:
	using iterator = Iterator;

	// constructor

	explicit List()
		: mHead(new Node())
		, mTail(new Node())
		, mSize(0)
	{
		mHead->mNext = mTail;
		mTail->mPrev = mHead;
	}

	explicit List(size_t count, const T& value = T())
		: mHead(new Node())
		, mTail(new Node())
		, mSize(0)
	{
		mHead->mNext = mTail;
		mTail->mPrev = mHead;

		for (size_t i = 0; i < count; i++)
		{
			addNode(mTail, value);
		}
	}

	List(std::initializer_list<T> init)
		: mHead(new Node())
		, mTail(new Node())
		, mSize(0)
	{
		mHead->mNext = mTail;
		mTail->mPrev = mHead;

		for (const T* i = init.begin(); i != init.end(); i++)
		{
			addNode(mTail, (*i));
		}
	}

	List(const List& other)
		: mHead(new Node())
		, mTail(new Node())
		, mSize(0)
	{
		mHead->mNext = mTail;
		mTail->mPrev = mHead;

		for (List::iterator it = other.begin(); it != other.end(); it++)
		{
			addNode(mTail, (*it));
		}
	}

	List(List&& other) noexcept
		: mHead(std::move(other.mHead))
		, mTail(std::move(other.mTail))
		, mSize(std::move(other.mSize))
	{
		other.mHead = nullptr;
		other.mTail = nullptr;
		other.mSize = 0;
	}

	//

	// destructor

	~List()
	{
		clear();

		if (mHead)
		{
			delete mHead;
		}

		if (mTail)
		{
			delete mTail;
		}
	}

	//

	// operator =

	List& operator=(const List& other)
	{
		if (this != &other)
		{
			List temp(other);
			swap(temp);
		}

		return *this;
	}

	List& operator=(List&& other) noexcept
	{
		List temp(std::move(other));
		swap(temp);

		return *this;
	}

	List& operator=(std::initializer_list<T> ilist)
	{
		List temp(ilist);
		swap(temp);

		return *this;
	}

	inline void assign(size_t count, const T& value)
	{
		List temp(count, value);
		swap(temp);
	}

	inline void assign(std::initializer_list<T> ilist)
	{
		List temp(ilist);
		swap(temp);
	}

	//

	// Element access

	inline T& front()
	{
		assert(mSize > 0);

		return mHead->mNext->mData;
	}

	inline const T& front() const
	{
		assert(mSize > 0);

		return mHead->mNext->mData;
	}

	inline T& back()
	{
		assert(mSize > 0);

		return mTail->mPrev->mData;
	}

	inline const T& back() const
	{
		assert(mSize > 0);

		return mTail->mPrev->mData;
	}

	//

	// Iterators

	inline iterator begin() noexcept
	{
		assert(mSize > 0);

		return iterator(mHead->mNext);
	}

	inline iterator end() noexcept
	{
		assert(mSize > 0);

		return iterator(mTail);
	}

	//

	// Capacity

	inline bool empty() const noexcept
	{
		return mSize == 0;
	}

	inline size_t size() const noexcept
	{
		return mSize;
	}

	//

	// Modifiers

	inline void clear() noexcept
	{
		while (mSize > 0)
		{
			removeNode(mTail->mPrev);
		}
	}

	inline iterator insert(iterator pos, const T& value)
	{
		Node* node = addNode(pos.mNode, value);
		return iterator(node);
	}

	inline void insert(iterator pos, size_t count, const T& value)
	{
		while (count > 0)
		{
			addNode(pos.mNode, value);
			count--;
		}
	}

	inline iterator insert(iterator pos, std::initializer_list<T> ilist)
	{
		Node* beginNode = nullptr;
		for (const T* i = ilist.begin(); i != ilist.end(); i++)
		{
			if (i == ilist.begin())
			{
				beginNode = addNode(pos.mNode, *i);
			}
			else
			{
				addNode(pos.mNode, *i);
			}
		}

		return iterator(beginNode);
	}

	inline iterator erase(iterator pos)
	{
		Node* node = removeNode(pos.mNode);
		return iterator(node);
	}

	inline iterator erase(iterator first, iterator last)
	{
		List::iterator it = first;
		while (it != last)
		{
			it = iterator(erase(it));
		}

		return it;
	}

	inline void push_back(const T& value)
	{
		addNode(mTail, value);
	}

	inline void push_back(T&& value)
	{
		addNode(mTail, std::move(value));
	}

	inline void pop_back()
	{
		removeNode(mTail->mPrev);
	}

	inline void push_front(const T& value)
	{
		addNode(mHead->mNext, value);
	}

	inline void push_front(T&& value)
	{
		addNode(mHead->mNext, std::move(value));
	}

	inline void pop_front()
	{
		removeNode(mHead->mNext);
	}

	inline void resize(size_t newSize, const T& value = T())
	{
		size_t size = mSize;

		if (newSize == size)
		{
			return;
		}
		else if (newSize < mSize)
		{
			for (size_t i = 0; i < size - newSize; i++)
			{
				removeNode(mTail->mPrev);
			}
		}
		else if (newSize > mSize)
		{
			for (size_t i = 0; i < newSize - size; i++)
			{
				addNode(mTail, value);
			}
		}
	}

	inline void swap(List& other)
	{
		std::swap(mHead, other.mHead);
		std::swap(mTail, other.mTail);
		std::swap(mSize, other.mSize);
	}

	//

private:
	Node* addNode(Node* before, const T& value)
	{
		Node* newNode = new Node(value);
		Node* prevNode = before->mPrev;

		prevNode->mNext = newNode;
		newNode->mPrev = prevNode;

		newNode->mNext = before;
		before->mPrev = newNode;

		mSize++;

		return newNode;
	}

	Node* addNode(Node* before, T&& value)
	{
		Node* newNode = new Node(std::move(value));
		Node* prevNode = before->mPrev;

		prevNode->mNext = newNode;
		newNode->mPrev = prevNode;

		newNode->mNext = before;
		before->mPrev = newNode;

		mSize++;

		return newNode;
	}

	Node* removeNode(Node* node)
	{
		Node* prevNode = node->mPrev;
		Node* nextNode = node->mNext;

		prevNode->mNext = nextNode;
		nextNode->mPrev = prevNode;

		delete node;

		mSize--;

		return nextNode;
	}

private:
	Node* mHead;
	Node* mTail;
	size_t mSize;
};