//OJ的题目管理，将题目从数据库中加载到内存中，打开题目列表以及题目详情

#include "selectMysql.hpp"

class ojModel {
private:
	std::map<std::string, Question> oj_model;
public:

	void loadQueston() {
		Mysql::selectForLoad(Mysql::connectMysql(), oj_model);
		Mysql::closeMysql(Mysql::connectMysql());
	}

	void getAllQuestions() {}

	void getInfoQuestion() {}

	void printInfo() {
		for(auto e : oj_model) {
			std::cout << e.first << ":" << std::endl;
			std::cout << "--------------------------" << std::endl;
			std::cout << e.second << std::endl;
			std::cout << std::endl;
		}
	}
};

