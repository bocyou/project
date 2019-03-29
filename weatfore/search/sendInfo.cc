#include "util.hpp"

int main()
{
    char arg[1024] = {0};
    if(getenv("METHOD")){
        if(strcasecmp(getenv("METHOD"), "GET") == 0){
            strcpy(arg, getenv("QUERY_STRING"));
        }else{
            char c;
            int i = 0;
            int len = atoi(getenv("CONTENT_LENGTH"));
            for(; i < len; i++){
                read(0, &c, 1);
                arg[i] = c;
            }
            arg[i] = 0;
        }
    }
    
	// 给出查询页面
	std::string city = arg;
	city = util::urlDecode(city);
	city = city.substr(7);
	city += ".html";
	city = "/home/zmy/Linux/project/weatfore/output/wwwroot/html/" + city;


	int fd = open(city.c_str(), O_RDONLY);
	if(fd < 0) {
		std::cout << "search error" << std::endl;
	}
	char buf[1024] = {0};
	while(read(fd, buf, sizeof(buf)-1) > 0) {
		printf("%s", buf);
	}

	// 构造http请求, 获取实时数据
	

	close(fd);
}
