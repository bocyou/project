#pragma once
// 构建索引和搜索

#include <string>
#include <vector>
#include <unordered_map>
#include "../lib/include/Jieba.hpp"

namespace searCher {

	struct docInfo {
		uint64_t doc_id;
		std::string title;
		std::string content;
		std::string url;
	};

	struct Weight {
		uint64_t doc_id;
		int weight; // 权重, 为排序做准备, 采用词频计算权重
		std::string key;
	};

	typedef std::vector<Weight> invertedList; // 创建一个 "倒排拉链" 类型

	class Index {
	private:
		std::vector<docInfo> _forward_index;  // 知道id获取对应的文档内容
		std::unordered_map<std::string, invertedList> _inverted_index; // 知道词获取id,倒排索引
		cppjieba::Jieba _jieba;

	public:
		Index();
		bool bulidIndex(const std::string& input_path); // form raw_input build to index
		const docInfo* getDocInfo(uint64_t doc_id) const ;  // serach forward
		const invertedList* getInvertedList(const std::string& key) const ; // search inverted
		void cutWord(const std::string& input, std::vector<std::string>& output);

	private:
		const docInfo* buildForward(const std::string& line);
		void buildInverted(const docInfo& doc_info);
	};

	class Searcher {
	private:
		Index* _index;
	public:
		Searcher() : _index(new Index()) {}

		~Searcher() {
			delete _index;
		}

		// 加载索引
		bool Init(const std::string& input_path);
		// 构造搜索结果
		bool Search(const std::string& query, std::string& result);
	private:
		std::string getDesc(const std::string& content, const std::string& key);
	};

} // end of searCher
