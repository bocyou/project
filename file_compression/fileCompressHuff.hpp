#include "huffMan.hpp"

class fileCompressHuff{
	public:
		fileCompressHuff();

		void compressHuff(const std::string filePath);
		void uncompressHuff(const std::string filePath);

		void getCharCount();
		void writeHead(int wr_fd);

	private:
		void _getHuffCode(huffManNode *root);
	private:
		std::vector<CharInfo> _charInfo;
};
