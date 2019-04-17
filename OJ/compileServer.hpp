#include <iostream>
#include <string>
#include <atomic>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <jsoncpp/json/json.h>
#include "util.hpp"

class Compile{
	private:
		static std::string srcFilePath(std::string& name) {
			return "./tmp/" + name + ".cpp";
		}
		
		static std::string errFilePath(std::string& name) {
			return "./tmp/" + name + ".comerr";
		}

		static std::string exeFilePath(std::string& name) {
			return "./tmp/" + name + ".exe";
		}

		static std::string stdInPath(std::string& name) {
			return "./tmp/" + name + ".stdin";
		}

		static std::string stdOutPath(std::string& name) {
			return "./tmp/" + name + ".stdout";
		}

		static std::string stdErrPath(std::string& name) {
			return "./tmp/" + name + ".stderr";
		}

	public:
		static bool compileRun(const Json::Value& req, Json::Value& rsp) {
			if(req["code"].empty())	 {
				rsp["error"] = 3;
				rsp["reason"] = "input is null";
				LOG(INFO, "input is null");
				return false;
			}

			const std::string& code = req["code"].asString();
			const std::string& msgStdIn = req["stdin"].asString();

			// 在提交代码中屏蔽 system 和 fork, pthread_create, 
			if(!leageSource(code)) {
				rsp["error"] = 3;
				rsp["reason"] = "User submitted fields may compromise the server";
				LOG(WARNING, "maby have fork system pthread_create");
				return false;
			}

			std::string fileName = writeInfo(code, msgStdIn);
			std::string stdOutMsg;
			std::string stdErrMsg;
			std::string comErrMsg;

			bool ret = compile(fileName);
			if(!ret) {
				rsp["error"] = 1;
				std::string reason = "compile failed: ";
				fileUtil::readFile(errFilePath(fileName), comErrMsg);
				reason += comErrMsg;
				rsp["reason"] = reason;
				LOG(INFO, "compile error");
				return false;
			}
			LOG(INFO, "compile done");

			std::string timeAndSpace;
			int ret_code = runPro(fileName, timeAndSpace);
			if(ret_code != 0) {
				rsp["error"] = 2;
				if(ret_code == 11){
					rsp["reason"] = "Segmentation fault, 请检查函数逻辑,是否有越界访问或者栈溢出";
				} else {
					rsp["reason"] = "exec file error";
				}
				fileUtil::readFile(stdOutPath(fileName), stdOutMsg);
				rsp["stdout"] = stdOutMsg;
				fileUtil::readFile(stdErrPath(fileName), stdErrMsg);
				rsp["stderr"] = stdErrMsg;
				LOG(INFO, "exec file error");
				return false;
			}
			LOG(INFO, "run process done");

			rsp["error"] = 0;
			if(timeAndSpace.empty()) {
				rsp["reason"] = "";
			} else {
				rsp["reason"] = timeAndSpace;
			}
			fileUtil::readFile(stdOutPath(fileName), stdOutMsg);
			rsp["stdout"] = stdOutMsg;
			fileUtil::readFile(stdErrPath(fileName), stdErrMsg);
			rsp["stderr"] = stdErrMsg;
			LOG(INFO, "comlied " + fileName + " done");
			return true;
		}

	private:
		static std::string writeInfo(const std::string& code, const std::string& msgStdIn) {
			std::atomic_int id(0);
			++id;
			std::string fileName = "tmp_" + 
				std::to_string(timeUtil::getTimeStmp()) + 
				"_" + std::to_string(id);

			fileUtil::writeFile(srcFilePath(fileName), code);
			fileUtil::writeFile(stdInPath(fileName), msgStdIn);
			return fileName;
		}

		static bool leageSource(const std::string& source) {
			if(source.find("system") != std::string::npos) {
				return false;
			}
			if(source.find("fork") != std::string::npos) {
				return false;
			}
			if(source.find("pthread_create") != std::string::npos) {
				return false;
			}

			return true;
		}

		static bool compile(std::string fileName) {
			char *cmd[20] = {0};
			char space[20][50] = {{0}};
			for(int i = 0; i < 20; i++) {
				cmd[i] = space[i];
			}
			sprintf(cmd[0], "%s", "g++");
			sprintf(cmd[1], "%s", srcFilePath(fileName).c_str());
			sprintf(cmd[2], "%s", "-o");
			sprintf(cmd[3], "%s", exeFilePath(fileName).c_str());
			sprintf(cmd[4], "%s", "-std=c++11");
			cmd[5] = NULL;

			int err_fd = open(errFilePath(fileName).c_str(), O_CREAT | O_WRONLY, 0644);
			if(err_fd < 0) {
				LOG(ERROR, "open stderr file error");
				return false;
			}
			dup2(err_fd, 2);

			int id = fork();
			if(id < 0) {
				LOG(ERROR, "fork error");
				return false;
			} else if (id > 0) { //father
				waitpid(id, NULL, 0);
			} else {
				execvp(cmd[0], cmd);
				exit(0);
			}
			struct stat st;
			int st_fd = stat(exeFilePath(fileName).c_str(), &st);
			if(st_fd != 0) {
				LOG(INFO, "the executable does not exist");
				return false;
			}
			return true;
		}
	
		static int runPro(std::string fileName, std::string& timeAndSpace) {
			int status = 0;
	
			int id = fork();
	
			if(id < 0) {
				LOG(ERROR, "fork error");
				return -1;
			} else if (id > 0) {
				usleep(1000000);
				pid_t err = waitpid(id, &status, WNOHANG);
				if(err == 0) { // 设置WNOHANG属性，子进程没有结束，waitpid返回0
					timeAndSpace = "算法时间复杂度过大, 请检查函数逻辑";
					kill(id, 9);
				}
				return status & 0x7f;
			} else {
				system("ulimit -s 2000"); //设置栈空间大小
				int stdIn_fd = open(stdInPath(fileName).c_str(), O_RDONLY);
				dup2(stdIn_fd, 0);
				int stdOut_fd = open(stdOutPath(fileName).c_str(), O_CREAT | O_WRONLY, 0644);
				dup2(stdOut_fd, 1);
				int stdErr_fd = open(stdErrPath(fileName).c_str(), O_CREAT | O_WRONLY, 0644);
				dup2(stdErr_fd, 2);
				std::string exeName = fileName + ".exe";
				execl(exeFilePath(fileName).c_str(), exeName.c_str(), NULL);
				exit(0);
			}
		}
};
