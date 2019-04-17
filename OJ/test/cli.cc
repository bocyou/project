#include <jsoncpp/json/json.h>
#include "../compileServer.hpp"


int main()
{
	Json::FastWriter writer;
	Json::Value res;
	Json::Value rsp;
	res["code"] = "#include <stdio.h> \nint main() { printf(\"hellow\"); return 0; }";
	res["stdin"] = "hello";
	//writer.write(res);

	Compile::compileRun(res, rsp);
	std::string res_str = writer.write(rsp);
	std::cout << res_str << std::endl;
	return 0;
}
