#include <iostream>
#include <fstream>
#include <algorithm>
#include <jsoncpp/json/json.h>
#include "searCher.h"
#include "../common/util.hpp"

namespace searCher {

	const char* const DICT_PATH = "../lib/jieba_dict/jieba.dict.utf8";
	const char* const HMM_PATH = "../lib/jieba_dict/hmm_model.utf8";
	const char* const USER_DICT_PATH = "../lib/jieba_dict/user.dict.utf8";
	const char* const IDF_PATH = "../lib/jieba_dict/idf.utf8";
	const char* const STOP_WORD_PATH = "../lib/jieba_dict/stop_words.utf8";

	Index::Index() : _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH) {}

	bool Index::bulidIndex(const std::string& input_path) {
		std::cout << "index build start" << std::endl;
		// 1. 按行读取文件内容(每一个行对应一个文档)
		std::ifstream file(input_path.c_str());
		if(!file.is_open()) {
			std::cout << "open err" << std::endl;
			return false;
		}

		std::string line; // not include '\n'
		while(std::getline(file, line)) {
			// 2. 构造 docInfo 对象, 构造正排索引数据, 得到对象插入vector
			const docInfo* doc_info = buildForward(line);

			// 3. 更新倒排索引数据
			buildInverted(*doc_info);
			if(doc_info->doc_id % 100 == 0) {
				std::cout << "build doc_id = " << doc_info->doc_id << std::endl;
			}
		}

		std::cout << "index build finished" << std::endl;
		file.close();
		return true;
	}

	const docInfo* Index::getDocInfo(uint64_t doc_id) const {
		if(doc_id >= _forward_index.size()) {
			return NULL;
		}
		return &_forward_index[doc_id];
	}

	const invertedList* Index::getInvertedList(const std::string& key) const {
		auto pos = _inverted_index.find(key);
		if(pos == _inverted_index.end()) {
			return NULL;
		}
		return &pos->second;
	}

	
	const docInfo* Index::buildForward(const std::string& line) {
		// 1. 对行内容进行切分
		std::vector<std::string> tokens; // 存放切分结果
		stringUtil::spiltString(line, tokens, "\3");
		if(tokens.size() != 3) {
			std::cout << "tokens not ok" << std::endl;
			return NULL;
		}

		// 2. 构造一个 docInfo 对象
		docInfo doc_info;
		doc_info.doc_id = _forward_index.size();
		doc_info.title = tokens[0];
		doc_info.url = tokens[1];
		doc_info.content = tokens[2];

		// 3. 把对象插入到正排索引
		_forward_index.push_back(doc_info);
		return &_forward_index.back();
	}

	void Index::buildInverted(const docInfo& doc_info) {
		// 1. 先对doc_info进行分词, 对正文, 对标题分词
		std::vector<std::string> title_tokens;
		cutWord(doc_info.title, title_tokens);

		std::vector<std::string> content_tokens;
		cutWord(doc_info.content, content_tokens);

		// 2. 对 doc_info中的标题和正文进行词频统计
		struct wordCount {
			int title_count;
			int content_count;
		};
		std::unordered_map<std::string, wordCount> word_count;
		for(std::string word : title_tokens) {
			boost::to_lower(word);
			++word_count[word].title_count;
		}

		for(std::string word : content_tokens) {
			boost::to_lower(word);
			++word_count[word].content_count;
		}

		// 3. 遍历分词结果, 在倒排索引中查找
		for(const auto& word_pair : word_count) {
			Weight weight;
			weight.doc_id = doc_info.doc_id;
			weight.weight = 10 * word_pair.second.title_count + word_pair.second.content_count;
			weight.key = word_pair.first;
			// 4. 不存在构建新的键值对
			// 5. 存在找到对应值, 构建一个新的 Weight 对象插入到 vector 中
			invertedList& inverted_list = _inverted_index[word_pair.first];
			inverted_list.push_back(weight);
		}
	}

	void Index::cutWord(const std::string& input, std::vector<std::string>& output) {
		_jieba.CutForSearch(input, output);
	}

	bool Searcher::Init(const std::string& input_path) {
		return _index->bulidIndex(input_path);
	}

	bool Searcher::Search(const std::string& query, std::string& result) {
		// 分词: 对查询词进行分词
		std::vector<std::string> tokens;
		_index->cutWord(query, tokens);

		// 触发: 对分词结果进行倒排索引, 找到那些文档具有相关性
		std::vector<Weight> all_token_result;
		for(std::string word : tokens) {
			boost::to_lower(word);
			auto* inverted_list = _index->getInvertedList(word);
			if(inverted_list == NULL) {
				continue;
			}
			// 此处进一步的改进是考虑不同的分词结果对应相同的 id
			// 实现思路是合并有序链表
			all_token_result.insert(all_token_result.end(), 
					inverted_list->begin(), inverted_list->end());
		}

		// 排序: 对结果进行一定规则排序
		// sort 第三个参数可以使用 仿函数/函数指针/lamdba表达式
		std::sort(all_token_result.begin(), all_token_result.end(), 
				[](const Weight& w1, const Weight& w2){
					return w1.weight > w2.weight;
				});

		// 构造结果: 查正排索引, 找到每个结果的 标题, 正文, url
		//使用json构造结果	
		
		Json::Value all_results;
		for(const auto& weight : all_token_result) {
			const auto* doc_info = _index->getDocInfo(weight.doc_id);
			if(doc_info == NULL) {
				continue;
			}

			Json::Value result_info;
			result_info["title"] = doc_info->title;
			result_info["url"] = doc_info->url;
			result_info["desc"] = getDesc(doc_info->content, weight.key);
			all_results.append(result_info);	
		}

		Json::FastWriter writer;
		result = writer.write(all_results);
		return true;
	}

	std::string Searcher::getDesc(const std::string& content, const std::string& key) {
		size_t pos = content.find(key);
		if(pos == std::string::npos) {
			if(content.size() < 150) {
				return content;
			} else {
				return content.substr(0, 150);
			}
		}

		size_t begin = pos < 50 ? 0 : pos - 50;
		if(begin + 150 >= content.size()) {
			return content.substr(begin);
		} else {
			return content.substr(begin, 160) + "...";
		}
	}

} // end of search
