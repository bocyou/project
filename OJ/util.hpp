#pragma once
#include <iostream>
#include <stdint.h>
#include <cstddef>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <sys/time.h>
#include <jsoncpp/json/json.h>
#include <boost/algorithm/string.hpp>

class timeUtil{
public:
	//获取当前时间戳
	static int64_t getTimeStmp() {
		struct timeval tv; 
		gettimeofday(&tv, NULL);
		return tv.tv_sec;
	}
	
	static int64_t getTimeStmpSM() {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
};

enum Level{
	INFO,
	WARNING,
	ERROR,
	FATAL,
};

std::string getLevel(Level lev) {
	switch(lev) {
		case INFO:
			return "INFO";
		case WARNING:
			return "WARNING";
		case ERROR:
			return "ERROR";
		case FATAL:
			return "FATAL";
		default:
			return "UNKNOW";
	}
}

void Log(Level lev, std::string msg, std::string fileName, int fileLine) {
	std::cout << " [ " << getLevel(lev) << " ] ";
	std::cout << " [ " << timeUtil::getTimeStmp() << " ] ";
	std::cout << " [ " << fileName << " ] ";
	std::cout << " [ " << fileLine << " ] ";
	std::cout << " [ " << msg << " ] " << std::endl;
}

#define LOG(lev, msg) Log(lev, msg, __FILE__, __LINE__)

class fileUtil{
public:
	static bool readFile(const std::string &filePath, std::string &content) {
		std::ifstream file(filePath.c_str());
		if(!file.is_open()) {
			return false;
		}
		std::string str;
		while(std::getline(file, str)) {
			content += str + '\n';
		}
		file.close();
		return true;
	}

	static bool writeFile(const std::string &filePath, const std::string &content) {
		std::ofstream file(filePath.c_str());
		if(!file.is_open()) {
			return false;
		}
		file.write(content.c_str(), content.size());
		file.close();
		return true;
	}
};

class boostUtil {
public:
	static void cutString(std::string& input_str, std::vector<std::string>& output_vec, std::string cut_str) {
		boost::split(output_vec, input_str, boost::is_any_of(cut_str), boost::token_compress_off);
	}
};

class urlCodeParse {
public:
	static unsigned char FromHex(unsigned char x)
	{
	    unsigned char y;
	    if(x >= 'A' && x <= 'Z'){
	        y = x - 'A' + 10;
	    }
	    else if(x >= 'a' && x <= 'z'){
	        y = x - 'a' + 10;
	    }
	    else if(x >= '0' && x <= '9'){
	        y = x - '0';
	    }
	    else
	        assert(0);
	
	    return y;
	}
	
	static std::string urlDecode(std::string& str)
	{
		std::string strtmp = "";
	    size_t length = str.length();
	    size_t i = 0;
	    for(; i < length; i++){
	        if(str[i] == '+')
	            strtmp += ' ';
	        else if(str[i] == '%'){
	            assert(i+2 < length);
	            unsigned char high = FromHex((unsigned char)str[++i]);
	            unsigned char low = FromHex((unsigned char)str[++i]);
	            strtmp += high*16 + low;
	        }
	        else{
	            strtmp += str[i];
	        }
	    }
	    return strtmp;
	}

};

class stringUtil {
public:
	static void codeToKv(std::string& user_body, Json::Value& req) {
		// 先对用户输入的body进行&切分
		std::vector<std::string> tmp_vec;
		std::unordered_map<std::string, std::string> code_kv;
		boostUtil::cutString(user_body, tmp_vec, ",");

		// 再进行urldecode解析，对等号进行切分
		for(auto k : tmp_vec) {
			std::vector<std::string> code_vec;
			boostUtil::cutString(k, code_vec, "=");
			code_kv[code_vec[0]] = urlCodeParse::urlDecode(code_vec[1]);
		}

		//将结果写入json
		for(auto v : code_kv) {
			req[v.first] = v.second;
		}
	}
};
