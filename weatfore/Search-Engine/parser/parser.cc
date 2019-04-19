// 数据处理模块
// 对html去标签,
// 进行文件合并, 把文档中涉及到的N和html文件内容合并成一个行文本文件
//				 生成的结果是一个大文件, 里面有很多行, 一行对应一个html
//				 这么做是让后面的索引处理更方便
// 对文档进行分析, 提取标题, 正文, 目标url

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "../common/util.hpp"

const std::string g_input_path = "../data/input";
const std::string g_output_path = "../data/tmp/raw_input";


// doc 是文档, 待搜索的html
struct docInfo {
	std::string title;
	std::string content;
	std::string url;
};

bool enumFile(const std::string& input_path, std::vector<std::string>& file_list) {
	namespace fs = boost::filesystem;
	fs::path root_path(input_path);
	if(!fs::exists(root_path)) {
		std::cout << "input_path err" << std::endl;
		return false;
	}

	// boost递归遍历目录, 借助迭代器
	fs::recursive_directory_iterator end_iter; // end
	for(fs::recursive_directory_iterator iter(root_path); iter != end_iter; ++iter) {
		// 只保留html, 不需要目录
		if(!fs::is_regular_file(*iter)) {
			continue;
		}
		if(iter->path().extension() != ".html") {
			continue;
		}
		file_list.push_back(iter->path().string());
	}
	return true;
}

// 从 title 标签提取
bool parseTitle(const std::string& html, std::string& title) {
	size_t beg = html.find("<title>");
	if(beg == std::string::npos) {
		std::cout << "<title> not found" << std::endl;
		return false;
	}

	size_t end = html.find("</title>");
	if(end == std::string::npos) {
		std::cout << "</title> not found" << std::endl;
		return false;
	}
	beg += std::string("<title>").size();
	if(beg > end) {
		std::cout << "beg end err" << std::endl;
		return false;
	}

	title = html.substr(beg, end-beg);
	return true;
}

// 没有标签的都是正文
bool parseContent(const std::string& html, std::string& content) {
	// 一个一个字符读取, < 认为标签开始, 接下来字符舍弃, 遇到 > 认为结束, 字符恢复
	bool is_content = true;
	for(auto ch : html) {
		if(is_content) {
			if(ch == '<') {
				is_content = false;
			} else {
				if(ch == '\n') {
					ch = ' ';
				}
				content.push_back(ch);
			}
		} else {
			if(ch == '>') {
				is_content = true;
			}
		}
	}

	return true;
}

// 前半部分 https://www.....
// 后半部分在path中, 截取后缀
bool parseUrl(const std::string& file_path, std::string& url) {
	std::string prefix = "https://www.boost.org/doc/libs/1_69_0/doc";
	std::string tail = file_path.substr(g_input_path.size());
	url = prefix + tail;
	return true;
}

bool parseFile(const std::string& file_path, struct docInfo& doc_info) {
	// 打开文件
	std::string html;
	bool ret = fileUtil::readFile(file_path, html);
	if(!ret) {
		std::cout << "read err" << std::endl;
		return false;
	}
	// 解析标题
	ret = parseTitle(html, doc_info.title);
	if(!ret) {
		std::cout << "parse title err " << file_path << std::endl;
		return false;
	}
	// 解析正文, 去标签
	
	ret = parseContent(html, doc_info.content);
	if(!ret) {
		std::cout << "parse content err" << std::endl;
		return false;
	}
	// 解析url
	ret = parseUrl(file_path, doc_info.url);
	if(!ret) {
		std::cout << "parse url err" << std::endl;
		return false;
	}
	return true;
}

// 最终的结果是行文本文件, 每一行一个doc_info
bool writeOutPut(const docInfo& doc_info, std::ofstream& file) {
	std::string line = doc_info.title + "\3" + doc_info.url + "\3" + doc_info.content + "\n";
	file.write(line.c_str(), line.size());
	return true;
}

int main()
{
	// 枚举输入路径中的所有html文档路径
	// /home/zmy/../../../data/input/html/xian.html
	std::vector<std::string> file_list;
	bool ret = enumFile(g_input_path, file_list);
	if(!ret) {
		std::cout << "enmuFile err" << std::endl;
		return 1;
	}

	/*
	for(auto e : file_list) {
		std::cout << e << std::endl;
	}
	std::cout << std::endl;	
	*/

	std::ofstream output_file(g_output_path.c_str());
	if(!output_file.is_open()) {
		std::cout << "open err. g_output_path :" << g_output_path << std::endl;
		return -1;
	}


	// 依次处理每个枚举的路径, 对文件进行分析, 分析文件标题, 正文, url, 去标签
	for(const auto& file_path : file_list) {
		docInfo info;
		ret = parseFile(file_path, info);
		if(!ret) {
			std::cout << "parse file err " << file_path << std::endl;
			continue;
		}
		// 把分析结果写入输入文件中
		writeOutPut(info, output_file);
	}

	output_file.close();
	return 0;
}
