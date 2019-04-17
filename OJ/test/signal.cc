#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>

int main()
{
	int id = fork();
	if(id == 0) {
		std::cout << "child " << getppid() << std::endl;
		while(1) {}
	} else {
		sleep(5);
		pid_t err = waitpid(id, NULL, WNOHANG);
		std::cout << "father " << getpid() << std::endl;
		std::cout << err << std::endl;
	}
	return 0;
}
