#include <jsoncpp/json/json.h>
#include "httplib.h"
#include "util.hpp"

int main()
{
	httplib::Server svr;

	//回调函数
	//lamdba表达式 [](){}
	svr.Get("/", [](const httplib::Request& req, httplib::Response& rsp){
			(void)req;
			Json::Value req_json;
			Json::Value rsp_json;
			//反序列化发送给客户端  fastwrite
			rsp.set_content("<html>hello world </html>", "text/html");
			});

	svr.listen("0.0.0.0", 20000);
	return 0;
}
