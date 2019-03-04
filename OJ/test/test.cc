#include <iostream>
#include <unistd.h>
#include <vector>

class myClass{
public:
	void func() {
		std::cout << "hello world" << std::endl;
	}
};

int main()
{
	std::vector<int> vec{1, 2, 3};
	myClass cl;
	cl.func();
	return 0;
}

