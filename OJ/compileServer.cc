#include "httplib.h"
#include "util.hpp"
#include "compileServer.hpp"

int main()
{
	httplib::Server svr;

	//回调函数
	//lamdba表达式 [](){}
	svr.Post("/compile", [](const httplib::Request& req, httplib::Response& rsp){
			(void)req;
			Json::Value req_json;
			Json::Value rsp_json;
			//解析用户输入
			std::string user_body = req.body;
			stringUtil::codeToKv(user_body, req_json);

			//反序列化发送给客户端  fastwrite
			Compile::compileRun(req_json, rsp_json);
			Json::FastWriter writer;
			writer.write(rsp_json);
			rsp.set_content(rsp_json.toStyledString(), "text/plain");
			});

	svr.set_base_dir("./wwwroot");
	svr.listen("0.0.0.0", 20000);
	return 0;
}
