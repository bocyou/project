#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

class fileUtil {
public:
	static bool readFile(const std::string& file_path, std::string& content) {
		std::ifstream file(file_path.c_str());
		if(!file.is_open()) {
			return false;
		}
		std::string line;
		while(std::getline(file, line)) {
			content += line + "\n";
		}

		file.close();
		return true;
	}

	static bool writeFile(const std::string& file_path, const std::string& content) {
		std::ofstream file(file_path.c_str());
		if(!file.is_open()) {
			return false;
		}
		file.write(content.c_str(), content.size());
		file.close();
		return true;
	}
};

class stringUtil {
public:
	static void spiltString(const std::string& input, std::vector<std::string>& output, const std::string& split_char) {
		boost::split(output, input, boost::is_any_of(split_char), boost::token_compress_off);
	}
};
