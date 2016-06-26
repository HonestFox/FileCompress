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
	unsigned char _ch;	//字符
	LongType _count;	//出现的次数
	string _code;			//编码
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
		//1、统计文件中字符出现的次数
		char ch = fgetc(fOut);
		while (ch != EOF)
		{
			++(_infos[(unsigned char)ch]._count);
			ch = fgetc(fOut);
		}

		//构建HuffmanTree
		CharInfo invalid;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		//生成Huffman编码
		string code;
		GenerateHuffmanCode(tree.GetRoot(), code);

		//压缩
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

		//补位
		if (size > 0)
		{
			value <<= (7 - size);
			fputc(value, fIn);
		}

		//写配置文件
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


	bool ReadLine(FILE *fOut, string &line)		//按行读取配置文件
	{
		char ch = fgetc(fOut);
		if (ch == EOF)
		{
			return false;   //表示读完了
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
		//读取配置文件，得到字符出现的次数   
		string ConfigFile = filename;
		ConfigFile += ".config";
		FILE *fConfig = fopen(ConfigFile.c_str(), "rb");
		assert(fConfig);
		LongType CharCount = 0;

		string line;
		while (ReadLine(fConfig, line))
		{
			if (line.empty() == true)   //处理统计换行符出现的次数
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

		//构建哈夫曼树
		CharInfo invalid;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		//读取压缩文件，进行还原
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
			if (pos-- == 0)				//一个字符处理完毕，处理下一个字符
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
	哈夫曼树
	文件I\O
	位运算
*/

/*
问题1：
解压缩后没东西(0xff)
解决:
close掉才能刷新缓冲区
*/

/*
问题2：
压缩后再解压，存在冗余(aaaabbbccd)
解决：
统计一下字符的数目，用它控制while循环的次数
*/

/*
问题3：
大一点的文件压缩后再解压，不完整
解决：
同上
*/


/*
问题4：
用了配置文件后，大一点的文件压缩后再解压，崩溃（越界）
解决：
要处理 '\n'(0x00000010) 对应的情况
*/

//效率：位运算、
//				解压的时候不要遍历二叉树了，压缩把编号的码放到配置文件中，解压直接读配置文件


//LZ算法  http://blog.csdn.net/zhangskd/article/details/17009111
// http://book.2cto.com/201309/32715.html
