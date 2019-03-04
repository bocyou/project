#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <ostream>
#include "./lib/include/mysql.h"


struct Question {
	std::string id;
	std::string name;
	std::string diffcult;
	std::string description;
	std::string src;
	std::string tail;

#ifdef __DEBUG__
	friend std::ostream& operator<<(std::ostream& cout, Question &q);
#endif
};

#ifdef __DEBUG__
std::ostream& operator<<(std::ostream& cout, Question &q) {
	cout << q.id << " | " << q.name <<  " | " << q.diffcult << " | " << q.description << " | " << q.src << " | " << q.tail << std::endl;
	return cout;
}
#endif 

class Mysql{
public:
	static MYSQL *connectMysql() {
		MYSQL *my_fd = mysql_init(NULL);

		assert(mysql_real_connect(my_fd, "127.0.0.1", "username", 
					"passwd", "database", 3306, NULL, 0) != NULL); 

		return my_fd;
	}

	static void selectForLoad(MYSQL *my_fd, std::map<std::string, Question>& oj_model) {
		mysql_query(my_fd, "set names utf8"); //解决查询中文编码问题

		std::string sel_sql = "select * from oj_list";
		mysql_query(my_fd, sel_sql.c_str());  //给数据库发送指令

		MYSQL_RES *result = mysql_store_result(my_fd);

		int rows = mysql_num_rows(result);
		//int cols = mysql_num_fields(result);

		//MYSQL_FIELD	*feild = mysql_fetch_fields(result); //获取列信息

		//for(int i = 0; i < cols; i++) {
		//	std::cout << feild[i].name << " | ";
		//}
		//std::cout << std::endl;

		for(int i = 0; i < rows; i++) { 
			MYSQL_ROW line = mysql_fetch_row(result); //查询行
			oj_model[line[0]].id = line[0];
			oj_model[line[0]].name = line[1];
			oj_model[line[0]].diffcult = line[2];
			oj_model[line[0]].description = line[3];
			oj_model[line[0]].src = line[4];
			oj_model[line[0]].tail = line[5];
		}

		free(result);
	}

	static void closeMysql(MYSQL *my_fd) {
		mysql_close(my_fd);
	}
};
