#pragma once
#include <boost/algorithm/string.hpp>
#include "util.hpp"
#include "selectMysql.hpp"

class userManage {
public: 

	// 此处得到的用户和密码都是经过urldecode转换之后的
	
	static bool userRegister(Json::Value& user_pass, std::string& fail_reason, long& user_id) {
		std::string user = user_pass["username"].asString();
		std::string pass = user_pass["password"].asString();
		if(!check_user_pass(user, pass, fail_reason)) {
			fail_reason = "用户注册失败, " + fail_reason;
			return false;
		}

		Mysql::connectMysql();
		user_id = Mysql::insert_user(user, pass);
		fail_reason = "用户注册成功";
		Mysql::closeMysql();

		return true;
	}

	static bool userLogin(Json::Value& user_pass, std::string& fail_reason, long& user_id) {
		std::string user = user_pass["username"].asString();
		std::string pass = user_pass["password"].asString();
		if(!check_user_pass(user, pass, fail_reason)) {
			fail_reason = "用户登陆失败, " + fail_reason;
			return false;
		}

		Mysql::connectMysql();
		user_id = Mysql::login_user(user, pass);
		if(user_id == -1) {
			fail_reason = "用户不存在, 请先注册";
		} else if (user_id == -2) {
			fail_reason = "用户名或者密码不正确";
		} else {
			fail_reason = "用户登陆成功";
		}
		Mysql::closeMysql();

		return true;
	}

private:
	static bool check_user_pass(std::string user, std::string pass, std::string& fail_reason) {
		if(user.empty()) {
			fail_reason = "用户名不能为空";
			return false;
		} else if (pass.empty()) {
			fail_reason = "密码不能为空";
			return false;
		}

		boost::to_upper(user);
		boost::to_upper(pass);

		std::vector<std::string> sql_comm {"MYSQL", "SELECT", "CREATE", "DROP", 
			"SHOW", "USE", "DELETE", "INSERT", "UPDATA", "TABLE", "DATABASE"};

		for(auto sql : sql_comm) {
			if(user.find(sql) != std::string::npos) {
				fail_reason = "请勿使用SQL语句, 谢谢配合!";
				return false;
			}
		}

		for(auto sql : sql_comm) {
			if(pass.find(sql) != std::string::npos) {
				fail_reason = "请勿使用SQL语句, 谢谢配合!";
				return false;
			}
		}
		return true;
	}
};

