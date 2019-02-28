#include "comm.h"

int main()
{
	MYSQL *my_fd = connectMysql();
	selectMysql(my_fd);
	closeMysql(my_fd);
	return 0;
}
