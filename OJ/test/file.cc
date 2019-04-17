#include "../util.hpp"

int main()
{
	std::string content;
	std::string fileInPath = "in.txt";
	std::string fileOutPath = "out.txt";

	fileUtil::readFile(fileInPath, content);
	fileUtil::writeFile(fileOutPath, content);
	return 0;
}
