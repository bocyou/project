#include <iostream>
#include <string>
#include "../redisCache.hpp"

void redisTest() {
	redisContext* cli = redisConnect("127.0.0.1", 6379);
	if(cli->err) {
		redisFree(cli);
		std::cout << "connect to redisServer err" << std::endl;
		return;
	}
	std::cout << "conect to redis redisServer Success";

}

int main()
{
	Redis* con = new Redis();
	if(!con->connectRedis("127.0.0.1", 6379)) {
		std::cout << "connect err" << std::endl;
		return -1;
	}
	con->commandSET("name", "zmy");
	std::cout << "get name is: " << con->commandGET("name") << std::endl;
	delete con;
	return 0;
}
