#include <iostream>
#include <unistd.h>

enum star {
	"简单",
	"中等",
	"困难",
};

int main()
{
	std::cout << star[0] << std::endl;
	return 0;
}
