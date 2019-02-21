#include "huffMan.hpp"

fileCompressHuff::fileCompressHuff() {
	_charInfo.resize(256);
	for(int i = 0; i < 256; i++) {
		_charInfo[i]._charCount = 0;
		_charInfo[i]._ch = i;
	}
}

void fileCompressHuff::compressHuff(const std::string filePath) {
	// 1、获取每个字符出现次数
	int rd_fd = open(filePath.c_str(), O_RDONLY);
	if(rd_fd < 0) {
		std::cout << "open file error" << std::endl;
		return;
	}

	char *readBuff = new char[1024];
	while(1) {
		size_t readSize = read(rd_fd, readBuff, 1024);

		if(readSize < 0) {
			std::cout << "read error" << std::endl;
		} else if (readSize == 0) {
			break;
		}

		for(size_t i = 0; i < readSize; i++) {
			_charInfo[readBuff[i]]._charCount++;
		}
	}
	lseek(rd_fd, 0, SEEK_SET);

	// 2、创建huffman树
	CharInfo invalid(0);
	huffManTree<CharInfo> huffman;
	huffman.createHuffmanTree(_charInfo, invalid);
	//huffman.displayCode(huffman.getRoot());

	// 3、获取编码
	
	huffman.getCode(huffman.getRoot());
	huffman.displayCode(huffman.getRoot());
	// 4、对源文件进行改写
	
	std::string fileName;
	if(filePath.rfind("/") < filePath.size()) {
		size_t pos = filePath.rfind(".") - filePath.rfind("/");
		fileName = filePath.substr(filePath.rfind("/")+1, pos-1);
	} else {
		fileName = filePath.substr(0, filePath.size() - filePath.rfind("."));
	}

	std::string suffix = filePath.substr(filePath.rfind("."));

	std::string huffName = fileName+".huff";
	int wr_fd = open(huffName.c_str(), O_WRONLY | O_CREAT, 0664);

	write(wr_fd, suffix.c_str(), suffix.size());
	write(wr_fd, "\n", 1);

	int bitCount = 0;
	char *flag = new char(0);
	while(1) {
		size_t readSize = read(rd_fd, readBuff, 1024);
		if(readSize < 0) {
			std::cout << "read error" << std::endl;
			break;
		} else if(readSize == 0) {
			break;
		}

		for(size_t i = 0; i < readSize; i++) {
			std::cout << readBuff[i] << " ";
		}
		std::cout << std::endl;

		for(size_t i = 0; i < readSize; i++) {
			std::string tmpCode = _charInfo[readBuff[i]]._charCode;
			for(size_t j = 0; j < tmpCode.size(); j++) {
				if(tmpCode[i] == '1') {
					(*flag) |= 1;
				}
				(*flag) <<= 1;
				bitCount++;
				if(bitCount == 8) {
					std::cout << *flag << std::endl;
					write(wr_fd, flag, 1);
					flag = 0, bitCount = 0;
				}
			}
		}
	}

	close(rd_fd);
	close(wr_fd);
}

void fileCompressHuff::getCharCount() {
	std::vector<CharInfo>::iterator it = _charInfo.begin();
	while(it != _charInfo.end()) {
		std::cout << it->_ch << ":" << it->_charCount << ":"  <<it->_charCode  << std::endl;
		it++;
	}
}
