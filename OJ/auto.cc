#include <iostream>
#include <jsoncpp/json/json.h>
#include "httplib.h"
#include "util.hpp"

int main()
{
	httplib::Server sve;
	sve.Get("/", [](const httplib::Request& req, httplib::Response& rsp){
			(void)req;
			std::string str;
			fileUtil::readFile("./wwwroot/login.html", str);
			rsp.set_content(str.c_str(), "text/html");
		});

	sve.Post("/login", [](const httplib::Request& req, httplib::Response& rsp) {
			Json::Value user_pass;
			stringUtil::codeToKv(req.body, user_pass);
			std::cout << req.body << std::endl;
			std::string user = user_pass["username"].asString();
			std::string pass = user_pass["password"].asString();

			std::cout << user << " -> " << pass << std::endl;

			// set response
			std::string str;
			fileUtil::readFile("./wwwroot/register.html", str);
			rsp.set_content(str.c_str(), "text/html");
		});
	sve.set_base_dir("./wwwroot");
	sve.listen("0.0.0.0", 20000);
	return 0;
}
