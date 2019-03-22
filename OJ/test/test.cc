static bool compileRun(const Json::Value& req, Json::Value& rsp) {
	if(req["code"].empty())	 {
		rsp["error"] = 3;
		rsp["reason"] = "input is null";
		LOG(INFO, "input is null");
		return false;

	}

	const std::string& code = req["code"].asString();
	const std::string& msgStdIn = req["stdin"].asString();
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

	int ret_code = runPro(fileName);
	if(ret_code != 0) {
		rsp["error"] = 2;
		rsp["reason"] = "exec file error";
		fileUtil::readFile(stdOutPath(fileName), stdOutMsg);
		rsp["stdout"] = stdOutMsg;
		fileUtil::readFile(stdErrPath(fileName), stdErrMsg);
		rsp["stderr"] = stdErrMsg;
		LOG(INFO, "exec file error");
		return false;

	}
	LOG(INFO, "run process done");

	rsp["error"] = 0;
	rsp["reason"] = "";
	fileUtil::readFile(stdOutPath(fileName), stdOutMsg);
	rsp["stdout"] = stdOutMsg;
	fileUtil::readFile(stdErrPath(fileName), stdErrMsg);
	rsp["stderr"] = stdErrMsg;
	return true;

}
