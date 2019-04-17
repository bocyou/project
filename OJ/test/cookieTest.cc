#include "../util.hpp"
#include "../httplib.h"

int main()
{
	httplib::Server ser;
	ser.Get("/", [](const httplib::Request& req, httplib::Response& rsp) {
				// std::string cookie = std::to_string(timeUtil::getTimeStmp());
				std::cout << req.method << std::endl;
				std::cout << req.path << std::endl;
				std::cout << req.version << std::endl;
				
				//httplib::Headers head = req.headers;

				std::string user_cookie = req.get_header_value("Cookie");
				std::cout << "user_cookie : " << user_cookie << std::endl;

				std::string cookie = std::to_string(timeUtil::getTimeStmp());
				rsp.set_header("Set-Cookie", cookie.c_str());
				rsp.set_content("i got it", "text/plan");
			});


	ser.listen("0.0.0.0", 20000);
	return 0;
}
