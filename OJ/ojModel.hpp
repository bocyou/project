//OJ的题目管理，将题目从数据库中加载到内存中，打开题目列表以及题目详情

#pragma	once
#include "selectMysql.hpp"

class ojModel {
private:
	std::map<std::string, Question> oj_question;
public:

	void loadQueston() {
		Mysql::connectMysql();
		Mysql::selectForLoad(oj_question);
		Mysql::closeMysql();
		LOG(INFO, "load " + std::to_string(oj_question.size()) + " questions");
	}

	bool getAllQuestions(std::vector<Question>& allQuestions) {
		allQuestions.clear(); //防止一直插入
		for(auto oj : oj_question) {
			allQuestions.push_back(oj.second);
		}
		LOG(INFO, "put all questions");
		return true;
	}

	bool getInfoQuestion(const std::string& id, Question &que) const {
		std::map<std::string, Question>::const_iterator pos = oj_question.find(id);
		if(pos == oj_question.end()) {
			//该id未查找到
			return false;
		}
		que = pos->second;
		LOG(INFO, "put one question");
		return true;
	}

	bool getCommQuestion(const std::string& id, std::vector<Comment>& userComm) {
		Mysql::selectForComm(id, userComm);
		return true;
	}

#ifdef __DEBUG__
	void printInfo() {
		for(auto e : oj_question) {
			std::cout << e.first << ":" << std::endl;
			std::cout << "--------------------------" << std::endl;
			std::cout << e.second << std::endl;
			std::cout << std::endl;
		}
	}
#endif
};

