#include "comm.h"

MYSQL *connectMysql() {
	MYSQL *my_fd = mysql_init(NULL);

	if(mysql_real_connect(my_fd, "127.0.0.1", "root", 
				"zmy19980520", "OJ", 3306, NULL, 0) == NULL) {
		std::cerr << "connect error" << std::endl;
	} else {
		std::cout << "connect success" << std::endl;
	}

	return my_fd;
}

void selectMysql(MYSQL *my_fd) {
	mysql_query(my_fd, "set names utf8");

	std::string sel_sql = "select * from oj_list";
	mysql_query(my_fd, sel_sql.c_str());

	MYSQL_RES *result = mysql_store_result(my_fd);

	int rows = mysql_num_rows(result);
	int cols = mysql_num_fields(result);

	MYSQL_FIELD	*feild = mysql_fetch_fields(result);

	for(int i = 0; i < cols; i++) {
		std::cout << feild[i].name << "|";
	}
	std::cout << std::endl;

	for(int i = 0; i < rows; i++) {
		MYSQL_ROW line = mysql_fetch_row(result);
		for(int j = 0; j < cols; j++) {
			std::cout << line[j] << "|";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	free(result);
}

void closeMysql(MYSQL *my_fd) {
	mysql_close(my_fd);
}
