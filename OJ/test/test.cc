#include <iostream>
#include <unistd.h>

int main()
{
	char  *list[] = {"g++", "file.cc", "-o", "file", "-std=c++11", NULL};
	execvp("g++", list);
	std::cout << "exec error" << std::endl;
	return 0;
}
