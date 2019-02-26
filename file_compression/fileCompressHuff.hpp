#include "huffMan.hpp"

class fileCompressHuff{
	public:
		fileCompressHuff();

		void compressHuff(const std::string filePath);
		void uncompressHuff(const std::string filePath);

	private:
		void _getHuffCode(huffManNode *root);
		void getCharCount();
		void writeHead(int wr_fd);
		void getLine(int rd_fd, std::string &content);
	private:
		std::vector<CharInfo> _charInfo;
};
