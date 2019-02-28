#include <iostream>
#include "mysql.h"

int main()
{
	std::cout << "mysql version is" << mysql_get_client_info() << std::endl;
	return 0;
}
