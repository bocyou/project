#pragma once
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <ostream>
#include "./lib/include/mysql.h"
#include "util.hpp"


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
	static void connectMysql() {

		my_fd = mysql_init(NULL);
		assert(mysql_real_connect(my_fd, "127.0.0.1", "username", 
					"password", "database", 3306, NULL, 0) != NULL); 

		// 设置客户端字符编码为utf8
		assert(mysql_set_character_set(my_fd, "utf8") == 0);

		LOG(INFO, "connect sql success");
	}

	static void selectForLoad(std::map<std::string, Question>& oj_model) {
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
		LOG(INFO, "select from sql done");
	}

	static long insert_user(std::string& user, std::string& pass) {
		// insert into oj_users (username, password) values ("zmy", "****")
		// std::string insert_query = "INSERT INTO oj_users (username, password) VALUES (\"";
		// insert_query += user;
		// insert_query += "\", \"";
		// insert_query += pass;
		// insert_query += "\")";

		
		// 下面的是PASSWORD函数, 用于web服务加密密码
		std::string insert_query = "INSERT INTO oj_users (username, password) VALUES ('"
								 + user 
								 + "', PASSWORD('"
								 + pass 
								 + "'))";

		if(mysql_query(my_fd, insert_query.c_str())) {
			LOG(WARNING, "insert user err");
			return -1;
		}

		mysql_use_result(my_fd);

		return (long)mysql_insert_id(my_fd);
	}

	static long login_user(std::string& user, std::string& pass) {
		std::string login_query = "SELECT id FROM oj_users WHERE username = '"
								+ user 
								+ "' AND PASSWORD = PASSWORD('"
								+ pass 
								+ "')";

		if(mysql_query(my_fd, login_query.c_str())) {
			LOG(WARNING, "search login err");
			return -1;
		}

		MYSQL_RES* result = mysql_store_result(my_fd);
		if(result == nullptr) {
			LOG(INFO, "mysql result is nullptr");
			return -1;
		}

		auto nums = mysql_num_rows(result);
		if(nums == 0) {
			mysql_free_result(result);
			LOG(INFO, "The query result is empty");
			return -2;
		}

		MYSQL_ROW row;
		long id = -1;
		while((row = mysql_fetch_row(result))) {
			id = atol(row[0]);
		}

		mysql_free_result(result); // 释放结果集

		return id;
	}

	static void closeMysql() {
		mysql_close(my_fd);
		LOG(INFO, "close sql done");
	}
private:
	static MYSQL *my_fd;
};

MYSQL* Mysql::my_fd = mysql_init(NULL);
