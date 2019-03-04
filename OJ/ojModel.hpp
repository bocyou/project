//OJ的题目管理，将题目从数据库中加载到内存中，打开题目列表以及题目详情

#pragma	once
#include "selectMysql.hpp"
#include "util.hpp"

class ojModel {
private:
	std::map<std::string, Question> oj_model;
public:

	void loadQueston() {
		Mysql::selectForLoad(Mysql::connectMysql(), oj_model);
		Mysql::closeMysql(Mysql::connectMysql());
		LOG(INFO, "load " + std::to_string(oj_model.size()) + " questions");
	}

	bool getAllQuestions(std::vector<Question>& allQuestions) {
		allQuestions.clear(); //防止一直插入
		for(auto oj : oj_model) {
			allQuestions.push_back(oj.second);
		}
		return true;
	}

	bool getInfoQuestion(const std::string& id, Question &que) const {
		std::map<std::string, Question>::const_iterator pos = oj_model.find(id);
		if(pos == oj_model.end()) {
			//该id未查找到
			return false;
		}
		que = pos->second;
		return true;
	}

#ifdef __DEBUG__
	void printInfo() {
		for(auto e : oj_model) {
			std::cout << e.first << ":" << std::endl;
			std::cout << "--------------------------" << std::endl;
			std::cout << e.second << std::endl;
			std::cout << std::endl;
		}
	}
#endif
};

