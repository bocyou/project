#pragma once
#include <hiredis/hiredis.h>
#include "util.hpp"

class Redis {
private:
	static redisContext* _con;
	static redisReply* _rep;
public:
	Redis() {}

	static bool connectRedis(std::string host, int port) {
		_con = redisConnect(host.c_str(), port);
		if(_con != NULL && _con->err) {
			LOG(ERROR, _con->errstr);
			return false;
		}
		return true;
	}

	static std::string command_String_GET(const std::string& key) {
		_rep = (redisReply*)redisCommand(_con, "GET %s", key.c_str());
		std::string str = _rep->str;
		freeReplyObject(_rep);
		return str;
	}

	static void command_String_SET(const std::string& key, const std::string& val) {
		redisCommand(_con, "SET %s %s", key.c_str(), val.c_str());
	}

	static void command_String_DEL(const std::string& key) {
		redisCommand(_con, "DEL %s", key.c_str());
	}

	~Redis() {
		_con = NULL;
		_rep = NULL;
	}
};

redisContext* Redis::_con = NULL;
redisReply* Redis::_rep = NULL;
