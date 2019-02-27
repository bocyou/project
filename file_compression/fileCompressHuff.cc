#include "fileCompressHuff.hpp"

fileCompressHuff::fileCompressHuff() {
	_charInfo.resize(256);
	for(int i = 0; i < 256; i++) {
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

	UCH *readBuff = new UCH[1024];
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
	huffManTree huffman;
	huffman.createHuffmanTree(_charInfo, invalid);

	// 3、获取编码
	_getHuffCode(huffman.getRoot());

	// 4、对源文件进行改写
	
	std::string fileName;
	std::string suffix = filePath.substr(filePath.rfind("."));
	if(filePath.rfind("/") < filePath.size()) {
		size_t pos = filePath.rfind(".") - filePath.rfind("/");
		fileName = filePath.substr(filePath.rfind("/")+1, pos-1);
	} else {
		fileName = filePath.substr(0, filePath.size() - suffix.size());
	}


	std::string huffName = fileName+".huff";
	int wr_fd = open(huffName.c_str(), O_WRONLY | O_CREAT, 0664);

	write(wr_fd, suffix.c_str(), suffix.size());
	write(wr_fd, "\n", 1);
	writeHead(wr_fd);

	char bitCount = 0;
	char *flag = new char(0);
	//char flag = 0;
	while(1) {
		size_t readSize = read(rd_fd, readBuff, 1024);
		if(readSize < 0) {
			std::cout << "read error" << std::endl;
			break;
		} else if(readSize == 0) {
			break;
		}

		for(size_t i = 0; i < readSize; i++) {
			std::string tmpCode = _charInfo[readBuff[i]]._charCode;
			for(size_t j = 0; j < tmpCode.size(); j++) {
				(*flag) <<= 1;
				if(tmpCode[j] == '1') {
					(*flag) |= 1;
				} 

				bitCount++;
				if(bitCount == 8) {
					write(wr_fd, flag, 1);
					bitCount = 0;
				}
			}
		}
		if(bitCount > 0 && bitCount < 8) {
			(*flag) <<= (8-bitCount);
			write(wr_fd, flag, 1);
		}
	}

	delete[] readBuff;
	close(rd_fd);
	close(wr_fd);
}

void fileCompressHuff::uncompressHuff(const std::string filePath) {
	//从源文件获取后缀得到文件名
	std::string fileName;
	std::string suffix = filePath.substr(filePath.rfind("."));

	if(suffix != ".huff") {
		std::cout << "suffix error" << std::endl;
		return;
	}

	if(filePath.rfind("/") < filePath.size()) {
		size_t pos = filePath.rfind(".") - filePath.rfind("/");
		fileName = filePath.substr(filePath.rfind("/")+1, pos-1);
	} else {
		fileName = filePath.substr(0, filePath.size() - suffix.size());
	}

	int rd_fd = open(filePath.c_str(), O_RDONLY);
	if(rd_fd < 0) {
		std::cout << "read huff error" << std::endl;
		return;
	}
	std::string charNum;
	std::string lineStr;

	suffix.clear();
	getLine(rd_fd, suffix);
	fileName += suffix;

	getLine(rd_fd, charNum);
	size_t charLine = atoi(charNum.c_str());

	for(size_t i = 0; i < charLine; i++) {
		lineStr.clear();
		getLine(rd_fd, lineStr);
		if(lineStr.empty()) {
			lineStr += '\n';
			getLine(rd_fd, lineStr);
		}
		//string类中是char，插入到数组中需要强转为unsigned char
		_charInfo[(UCH)lineStr[0]]._charCount = atoi(lineStr.c_str() + 2);

	}

	//重构huffman树，
	huffManTree huff;
	CharInfo invalid(0);
	huff.createHuffmanTree(_charInfo, invalid);


	//开始解压缩
	int wr_fd = open(fileName.c_str(), O_CREAT | O_WRONLY, 0644);
	char pos = 7;
	huffManNode *cur = huff.getRoot();
	size_t fileSize = cur->_weight._charCount;
	char *readBuff = new char[1024];
	char *ch = new char(0x00);

	while(true) {
		size_t readSize = read(rd_fd, readBuff, 1024);
		if(readSize == 0) {
			break;
		}

		for(size_t i = 0; i < readSize; i++) {
			pos = 7;
			for(size_t j  = 0; j < 8; j++) {
				if(readBuff[i] & (1 << pos)) {
					cur = cur->_right;
				} else {
					cur = cur->_left;
				}

				if(cur->_left == NULL && cur->_right == NULL) {
					*ch = cur->_weight._ch;
					write(wr_fd, ch, 1);
					cur = huff.getRoot();

					fileSize--;
					if(fileSize == 0) {
						break;
					}
				}
				pos--;
			}
		}
	}

	delete[] readBuff;
	delete ch;
	close(rd_fd);
	close(wr_fd);
}

void fileCompressHuff::writeHead(int wr_fd) {
	size_t tmpNum = 0;
	std::string content;
	for(size_t i = 0; i < 256; i++) {
		if(_charInfo[i]._charCount != 0) {
			content += _charInfo[i]._ch;
			content.push_back(',');
			content += std::to_string(_charInfo[i]._charCount);
			content += '\n';
			tmpNum++;
		}
	}
	std::string codeNum = std::to_string(tmpNum);
	codeNum.push_back('\n');
	write(wr_fd, codeNum.c_str(), codeNum.size());
	write(wr_fd, content.c_str(), content.size());
}

void fileCompressHuff::getCharCount() {
	std::vector<CharInfo>::iterator it = _charInfo.begin();
	while(it != _charInfo.end()) {
		std::cout << it->_ch << ":" << it->_charCount << ":"  <<it->_charCode  << std::endl;
		it++;
	}
}

void fileCompressHuff::_getHuffCode(huffManNode *root) {
	if(root == NULL) {
		return;
	}

	_getHuffCode(root->_left);
	_getHuffCode(root->_right);

	if(root->_left == NULL && root->_right == NULL) {
		std::string code;
		huffManNode *parent = root->_parent;
		huffManNode *cur = root;
		while(parent) {
			if(parent->_left == cur) {
				code.insert(0, "0");
			} else  {
				code.insert(0, "1");
			}
			cur = parent;
			parent = cur->_parent;
		}
		_charInfo[root->_weight._ch]._charCode = code;
	}
}

void fileCompressHuff::getLine(int rd_fd, std::string &content) {
	char ch = '\0';
	read(rd_fd, &ch, 1);
	while(ch != '\n') {
		content.push_back(ch);
		read(rd_fd, &ch, 1);
	}
}
