#pragma once
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

// 仿函数
template <class T>
struct Less
{
	bool operator() (const T& l, const T& r)
	{
		return l < r;
	}
};

template <class T>
struct Greater
{
	bool operator() (const T& l, const T& r)
	{
		return l > r;
	}
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<class T, class Compare = Greater<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(const T* a, size_t size)
	{
		assert(a);
		for (size_t i = 0; i < size; ++i)
		{
			_a.push_back(a[i]);
		}

		// 建堆 N*lgN
		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			_AdjustDown(i);
		}
	}

	//
	void Push(const T& x)
	{
		_a.push_back(x);

		_AdjustUp(_a.size() - 1);
	}

	// 
	void Pop()
	{
		assert(!_a.empty());
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		_AdjustDown(0);
	}

	T& Top()
	{
		return _a[0];
	}
	size_t Size()
	{
		return _a.size();
	}

	bool Empty()
	{
		return _a.empty();
	}

protected:
	void _AdjustDown(size_t parent)
	{
		// 指向左孩子
		size_t child = parent * 2 + 1;

		Compare com;
		while (child < _a.size())
		{
			// 选出左右孩子里面大的那一个

			//if (child+1 < _a.size() 
			//	&&_a[child+1] > _a[child])
			if (child + 1 < _a.size()
				&& com(_a[child + 1], _a[child]))
			{
				++child;
			}

			// 如果孩子比父节点要大，则交换并继续往下调整
			//if (_a[child] > _a[parent])
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}

	void _AdjustUp(int child)
	{
		int parent = (child - 1) / 2;
		Compare com;

		while (child > 0)
			//while(parent >= 0)
		{
			//if (_a[child] > _a[parent])
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}
protected:
	vector<T> _a;
};



void HeapTest()
{
	int arr[] = { 4, 1, 3, 5, 7, 8, };
	int sz = sizeof(arr) / sizeof(*arr);
	Heap<int> hi(arr, sz);
	Heap<int, less<int> > hii(arr, sz);

}