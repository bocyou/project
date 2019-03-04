#include "ojModel.hpp"
#include "compileServer.hpp"
#include "ojView.hpp"
#include "httplib.h"
#include <jsoncpp/json/json.h>

int main()
{
	httplib::Server svr;
	ojModel oj;
	oj.loadQueston();
	svr.Get("/questionList", [&oj](const httplib::Request& req, httplib::Response& rsp) {
			(void)req;
			std::vector<Question> questionList;
			oj.getAllQuestions(questionList);

			//渲染页面
			std::string html;
			ojView::renderQuestionList(questionList, html);
			rsp.set_content(html, "text/html");
		});

	svr.Get(R"(/questionInfo/(\d+))", [&oj](const httplib::Request& req, httplib::Response& rsp) {
			Question que;
			oj.getInfoQuestion(req.matches[1].str(), que);
			std::string html;
			ojView::renderQuestionInfo(que, html);
			rsp.set_content(html, "text/html");
		});

	svr.Post(R"(/compile/(\d+))", [&oj](const httplib::Request& req, httplib::Response& rsp) {
			// 解析题目
			Question que;
			oj.getInfoQuestion(req.matches[1].str(), que);
			Json::Value req_json;
			Json::Value rsp_json;
			//解析用户输入
			std::string user_body = req.body;
			stringUtil::codeToKv(user_body, req_json);
			std::string user_code = req_json["code"].asString(); 

			// 加入测试用例提交运行
			req_json["code"] = user_code + que.tail;
			Compile::compileRun(req_json, rsp_json);

			//根据编译结果构造最终网页
			std::string html;
			ojView::renderResult(rsp_json["stdout"].asString(), rsp_json["reason"].asString(), html);
			rsp.set_content(html, "text/html");
		});

	svr.set_base_dir("./wwwroot");
	svr.listen("0.0.0.0", 20000);
	return 0;
}
