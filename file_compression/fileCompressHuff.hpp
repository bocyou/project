#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <queue>
#include <ostream>

struct CharInfo {

	CharInfo(long long count = 0) : _ch(0), _charCount(count), _charCode("") {}

	char _ch;
	long long _charCount;
	std::string _charCode;

	bool operator!=(const CharInfo& ch) {
		return _charCount != ch._charCount;
	}

	bool operator==(const CharInfo& ch) {
		return _charCount == ch._charCount;
	}

	CharInfo operator+(const CharInfo& ch) {
		return CharInfo(_charCount + ch._charCount);
	}

	friend std::ostream& operator<<(std::ostream& out, const CharInfo& ch) {
		out << " " << ch._charCount << "\n";
		return out;
	}
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
