#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

struct CharInfo {
	char _ch;
	long long _charCount;
	std::string _charCode;
};

class fileCompressHuff{
	public:
		fileCompressHuff();

		void compressHuff(const std::string filePath);
		void uncompressHuff(const std::string filePath);
		void getCharCount();

	private:
		std::vector<CharInfo> _charInfo;
};
