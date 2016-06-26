#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <cstdio>
#include <Windows.h>
#include "Huffman.h"
#include "compress.h"

void TestHuffmanTree()
{
	int arr[] = { 0, 1,1,1, 2, 3, 4, 5, 6,6,6,6, 7, 8, 9 };
	int sz = sizeof(arr) / sizeof(*arr);
	HuffmanTree<int> ht(arr, sz, 0);
}


void TestFileCompress()
{
	FileCompress fc;
	fc.Compress("Input");
}

void TestFileUncompress()
{
	FileCompress fc;
	fc.Uncompress("Input");
}

void TestBigFileCompress()
{
	FileCompress fc;
	fc.Compress("BigFile");
}

void TestBigFileUncompress()
{
	FileCompress fc;
	fc.Uncompress("BigFile");
}


int main()
{
	int begin = GetTickCount();
	TestBigFileCompress();
	int end = GetTickCount();
	cout << "压缩用时 ： " << end - begin << endl;


	begin = GetTickCount();
	TestBigFileUncompress();
	end = GetTickCount();
	cout << "解压用时 ： " << end - begin << endl;
	return 0;	
}

//int main()
//{
//	int begin = GetTickCount();
//	TestFileCompress();
//	int end = GetTickCount();
//	cout << "压缩用时 ： " << end - begin << endl;
//
//
//	begin = GetTickCount();
//	TestFileUncompress();
//	end = GetTickCount();
//	cout << "解压用时 ： " << end - begin << endl;
//	return 0;	
//}