#include "compileServer.hpp"
#include "ojView.hpp"
#include "ojModel.hpp"
#include "userManage.hpp"
#include "httplib.h"

int main()
{
	httplib::Server svr;
	ojModel oj;
	oj.loadQueston();

	// questionList
	svr.Get("/", [&oj](const httplib::Request& req, httplib::Response& rsp) {
			(void)req;
			std::vector<Question> questionList;
			oj.getAllQuestions(questionList);

			//渲染页面
			std::string html;
			ojView::renderQuestionList(questionList, html);
			rsp.set_content(html, "text/html");
		});

	// questionInfo
	svr.Get(R"(/questionInfo/(\d+))", [&oj](const httplib::Request& req, httplib::Response& rsp) {
			Question que;
			if(oj.getInfoQuestion(req.matches[1].str(), que)) {
				// id查找到再加载评论	
			}
			std::string html;
			ojView::renderQuestionInfo(que, html);
			rsp.set_content(html, "text/html");
		});
	svr.Post(R"(/questionInfo/(\d+)/comment)", [](const httplib::Request& req, httplib::Response& rsp){
			Json::Value user_comm;
			stringUtil::codeToKv(req.body, user_comm);

			std::cout << req.matches[1].str() << "-> " << user_comm["comment"].asString() << std::endl;
			rsp.set_content("i got it", "text/plan");
		});

	// userLogin
	svr.Post("/login", [](const httplib::Request& req, httplib::Response& rsp){
			Json::Value user_pass;
			std::string inforMation;
			stringUtil::codeToKv(req.body, user_pass);

			std::string html;
			long user_id = -1;

			userManage::userLogin(user_pass, inforMation, user_id);

			userMangeView::renderUser(html, inforMation, user_id);
			rsp.set_content(html, "text/html");
		});

	// userRegist
	svr.Post("/register", [](const httplib::Request& req, httplib::Response& rsp){
			Json::Value user_pass;
			std::string inforMation;
			stringUtil::codeToKv(req.body, user_pass);

			std::string html;
			long user_id = -1;

			userManage::userRegister(user_pass, inforMation, user_id);

			userMangeView::renderUser(html, inforMation, user_id);
			rsp.set_content(html, "text/html");
		});

	// compileRun
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
