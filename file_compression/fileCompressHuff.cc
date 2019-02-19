#include "huffMan.hpp"

fileCompressHuff::fileCompressHuff() {
	_charInfo.resize(256);
	for(int i = 0; i < 256; i++) {
		_charInfo[i]._charCount = 0;
	}
}

void fileCompressHuff::compressHuff(const std::string filePath) {
	// 1、获取每个字符出现次数
	int fd = open(filePath.c_str(), O_RDONLY);
	if(fd < 0) {
		std::cout << "open file error" << std::endl;
		return;
	}

	char *readBuff = new char[1024];
	while(1) {
		size_t readSize = read(fd, readBuff, 1024);
		if(readSize < 0) {
			std::cout << "read error" << std::endl;
		} else if (readSize == 0) {
			break;
		}

		for(size_t i = 0; i < readSize; i++) {
			_charInfo[readBuff[i]]._charCount++;
			_charInfo[readBuff[i]]._ch = readBuff[i];
		}
	}

	// 2、创建huffman树
	// 3、获取编码
	// 4、对源文件进行改写
	

	close(fd);
}

void fileCompressHuff::getCharCount() {
	std::vector<CharInfo>::iterator it = _charInfo.begin();
	while(it != _charInfo.end()) {
		std::cout << it->_ch << ":" << it->_charCount << std::endl;
		it++;
	}
}
