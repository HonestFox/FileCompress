#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "Huffman.h"

typedef unsigned long LongType;

struct CharInfo
{
public:
	CharInfo(LongType count = 0)
		:_count(count)
	{}

	bool operator!=(const CharInfo &info) const
	{
		return _count != info._count;
	}

	bool operator<(const CharInfo &info) const
	{
		return _count < info._count;
	}

	CharInfo operator+(const CharInfo &info)
	{
		return CharInfo(_count + info._count);
	}
public:
	unsigned char _ch;	//�ַ�
	LongType _count;	//���ֵĴ���
	string _code;			//����
};

class FileCompress
{
public:
	FileCompress()
	{
		for (size_t i = 0; i < 256; ++i)
		{
			_infos[i]._ch = (unsigned char)i;
			_infos[i]._count = 0;
		}
	}
public:
	void GenerateHuffmanCode(HuffmanTreeNode<CharInfo> *root, string code)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->_left == NULL && root->_right == NULL)
		{
			_infos[root->_weight._ch]._code = code;
			return;
		}
		GenerateHuffmanCode(root->_left, code + '0');
		GenerateHuffmanCode(root->_right, code + '1');
	}

	void Compress(const char *filename)
	{
		FILE *fOut = fopen(filename, "rb");
		assert(fOut);
		//1��ͳ���ļ����ַ����ֵĴ���
		char ch = fgetc(fOut);
		while (ch != EOF)
		{
			++(_infos[(unsigned char)ch]._count);
			ch = fgetc(fOut);
		}

		//����HuffmanTree
		CharInfo invalid;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		//����Huffman����
		string code;
		GenerateHuffmanCode(tree.GetRoot(), code);

		//ѹ��
		string compressFilename = filename;
		compressFilename += ".compress";
		FILE *fIn = fopen(compressFilename.c_str(), "wb");
		assert(fIn);

		fseek(fOut, 0, SEEK_SET);
		ch = fgetc(fOut);
		unsigned char value = 0;
		int size = 0;
		while (1)
		{
			if (feof(fOut))
			{
				break;
			}
			string &code = _infos[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				if (code[i] == '1')
				{
					value = value | 1;
				}
				++size;
				if (size == 8)
				{
					fputc(value , fIn);
					value = 0;
					size = 0;
				}
				value = value << 1;
			}
			ch = fgetc(fOut);
		}

		//��λ
		if (size > 0)
		{
			value <<= (7 - size);
			fputc(value, fIn);
		}

		//д�����ļ�
		string ConfigFile = filename;
		ConfigFile += ".config";
		FILE *fConfig = fopen(ConfigFile.c_str(), "wb");
		assert(fConfig);
		string line;
		char buffer[128];
		for (size_t i = 0; i < 256; ++i)
		{
			if (_infos[i]._count > 0)
			{
				line += _infos[i]._ch;
				line += ",";

				_itoa(_infos[i]._count, buffer, 10);
				line += buffer;
				line += '\n';

				fputs(line.c_str(), fConfig);
			}
			line.clear();
		}

		
		_fcloseall();
	}


	bool ReadLine(FILE *fOut, string &line)		//���ж�ȡ�����ļ�
	{
		char ch = fgetc(fOut);
		if (ch == EOF)
		{
			return false;   //��ʾ������
		}

		while (ch != EOF && ch != '\n')
		{
			line += ch;
			ch = fgetc(fOut);
		}
		return true;
	}

	void Uncompress(const char *filename)
	{
		//��ȡ�����ļ����õ��ַ����ֵĴ���   
		string ConfigFile = filename;
		ConfigFile += ".config";
		FILE *fConfig = fopen(ConfigFile.c_str(), "rb");
		assert(fConfig);
		LongType CharCount = 0;

		string line;
		while (ReadLine(fConfig, line))
		{
			if (line.empty() == true)   //����ͳ�ƻ��з����ֵĴ���
			{
				line += '\n';
			}
			else
			{
				unsigned char ch = (unsigned char)line[0];
				LongType count = atoi(line.substr(2).c_str());
				_infos[ch]._count = count;
				CharCount += count;
				line.clear();
			}
		}

		//������������
		CharInfo invalid;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		//��ȡѹ���ļ������л�ԭ
		string compressFile = filename;
		compressFile += ".compress";
		FILE *fOut = fopen(compressFile.c_str(), "rb");
		assert(fOut);

 		string uncompressFile = filename;
		uncompressFile += ".uncompress";
		FILE *fIn = fopen(uncompressFile.c_str(), "wb");
		assert(fIn);

		HuffmanTreeNode<CharInfo> *root = tree.GetRoot();
		HuffmanTreeNode<CharInfo> *cur = root;
		char ch = fgetc(fOut);
		int pos = 7;
		
		while (1)
		{
		//while (1)
		//{
		//	if (feof(fOut))
		//	{
		//		break;
		//	}
			if (ch & (1 << pos))
			{
				cur = cur->_right;
			}
			else
			{
				cur = cur->_left;
			}
			if (cur->_left == NULL && cur->_left == NULL)
			{
				fputc(cur->_weight._ch, fIn);
				cur = root;
				if (--CharCount == 0)
				{
					break;
				}
			}
			if (pos-- == 0)				//һ���ַ�������ϣ�������һ���ַ�
			{
				ch = fgetc(fOut);
				pos = 7;
			}
		}

		_fcloseall();
	}
protected:
	HuffmanTree<CharInfo> _root;
	CharInfo _infos[256];
};



/*
	��������
	�ļ�I\O
	λ����
*/

/*
����1��
��ѹ����û����(0xff)
���:
close������ˢ�»�����
*/

/*
����2��
ѹ�����ٽ�ѹ����������(aaaabbbccd)
�����
ͳ��һ���ַ�����Ŀ����������whileѭ���Ĵ���
*/

/*
����3��
��һ����ļ�ѹ�����ٽ�ѹ��������
�����
ͬ��
*/


/*
����4��
���������ļ��󣬴�һ����ļ�ѹ�����ٽ�ѹ��������Խ�磩
�����
Ҫ���� '\n'(0x00000010) ��Ӧ�����
*/

//Ч�ʣ�λ���㡢
//				��ѹ��ʱ��Ҫ�����������ˣ�ѹ���ѱ�ŵ���ŵ������ļ��У���ѹֱ�Ӷ������ļ�


//LZ�㷨  http://blog.csdn.net/zhangskd/article/details/17009111
// http://book.2cto.com/201309/32715.html
