#pragma once
#include <iostream>
#include <stdint.h>
#include <cstddef>
#include <string>
#include <fstream>
#include <sys/time.h>

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
