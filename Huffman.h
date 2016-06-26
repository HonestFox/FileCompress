#pragma once;
#include <iostream>
#include "heap.h"


template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const T &data)
		:_left(NULL)
		, _right(NULL)
		, _weight(data)
	{}
	HuffmanTreeNode<T> *_left;
	HuffmanTreeNode<T> *_right;
	T _weight;
};

template <class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	HuffmanTree(T *a, size_t n, const T &invalid)
	{
		struct NodeCompare
		{
			bool operator()(const Node *left, const Node *right)
			{
				return((left->_weight) < (right->_weight));
			}
		};
		Heap<Node*, NodeCompare> minHeap;
		for (size_t i = 0; i < n; ++i)
		{
			if (a[i] != invalid)
			{
				minHeap.Push(new Node(a[i]));
			}
		}
		while (minHeap.Size() > 1)
		{
			Node *left = minHeap.Top();
			minHeap.Pop();
			Node *right = minHeap.Top();
			minHeap.Pop();

			Node *parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			minHeap.Push(parent);
		}
		_root = minHeap.Top();
	}
	Node *GetRoot()
	{
		return _root;
	}
protected:
	Node *_root;
};