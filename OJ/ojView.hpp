#pragma once
#include <sstream>
#include <ctemplate/template.h>
#include "ojModel.hpp"

static std::string leveToString(const std::string &str) {
	int falg;
	std::stringstream ss;
	ss << str;
	ss >> falg;
	switch(falg) {
		case 0:
			return "简单";
		case 1:
			return "中等";
		case 2:
			return "困难";
	}
	return "";
}

class ojView{
public:
	static void renderQuestionList(const std::vector<Question>& allQuestion, std::string& html) {
		ctemplate::TemplateDictionary dict("questionList");
		for(const auto& question : allQuestion) {
			 ctemplate::TemplateDictionary* table_dict
				 = dict.AddSectionDictionary("question");
			 table_dict->SetValue("id", question.id);
			 table_dict->SetValue("name", question.name);
			 table_dict->SetValue("diffcult", leveToString(question.diffcult));
		}
		ctemplate::Template* tpl;
		tpl = ctemplate::Template::GetTemplate("./template/questionList.html", ctemplate::DO_NOT_STRIP);
		tpl->Expand(&html, &dict);
		LOG(INFO, "render question list done");
	}

	static void renderQuestionInfo(const Question& que, std::string &html) {
		ctemplate::TemplateDictionary dict("questionInfo");
		dict.SetValue("id", que.id);
		dict.SetValue("name", que.name);
		dict.SetValue("diffcult", leveToString(que.diffcult));
		dict.SetValue("description", que.description);
		dict.SetValue("src", que.src);

		ctemplate::Template* tpl;
		tpl = ctemplate::Template::GetTemplate("./template/questionInfo.html", ctemplate::DO_NOT_STRIP);
		tpl->Expand(&html, &dict);
		LOG(INFO, "render question info done");
	}

	static void renderResult(const std::string& str_stdout, const std::string& reason, std::string& html) {
		ctemplate::TemplateDictionary dict("resultInfo");
		dict.SetValue("stdout", str_stdout);
		dict.SetValue("reason", reason);

		ctemplate::Template* tpl;
		tpl = ctemplate::Template::GetTemplate("./template/resultInfo.html", ctemplate::DO_NOT_STRIP);
		tpl->Expand(&html, &dict);
		LOG(INFO, "render result done");
	}
};

class userMangeView {
public:
	static void renderUser(std::string& html, std::string& inforMation, long user_id) {
		ctemplate::TemplateDictionary dict("userMange");
		dict.SetValue("info", inforMation);
		dict.SetValue("userid", std::to_string(user_id));

		ctemplate::Template* tpl;
		tpl = ctemplate::Template::GetTemplate("./template/userMange.html", ctemplate::DO_NOT_STRIP);
		tpl->Expand(&html, &dict);
		LOG(INFO, "render usermange done");
	}


};
