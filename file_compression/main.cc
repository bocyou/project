#include "fileCompressHuff.hpp"

int main()
{
	fileCompressHuff fc;
	//fc.compressHuff("/home/zmy/git-code/project/file_compression/test.txt");
	//fc.compressHuff("test.txt");
	fc.uncompressHuff("test.huff");
	return 0;
}
