# FileCompress


文件压缩

内容：

实现一个程序，能够对指定的文件进行压缩以减小体积，还能对压缩后的文件进行解压恢复成原文件。

主要技术：
哈夫曼树与哈夫曼编码、 文件I/O、 位运算

实现原理：
· 以二进制格式打开待压缩的文件，统计文件中各字符出现的次数，将这些字符次数进行哈夫曼编码；
· 遍历文件中的这些字符，并把这些字符对应的哈夫曼编码以位的形式存入一个新的文件，作为压缩后的文件，即可实现压缩；
· 按位通过哈夫曼编码解析压缩文件存放的内容，并将解析后的字符存入一个新的文件，作为解压后的文件，即可实现解压缩。

改进：
每次压缩后写一个配置文件，保存各字符出现的次数和对应的哈夫曼编码，这样在解压的过程中就不用遍历哈夫曼树来获取字符对应的编码了，可以提高解压的速率。

开发工具：
Visual Studio 2015
