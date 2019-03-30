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
	const char* host = "www.nmc.cn";
	char ip_addr[32] = {0};
	util::hostToAddr(host, ip_addr);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		LOG(ERROR, "create connect sock err");
		return -1;
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(80);
	local.sin_addr.s_addr = inet_addr(ip_addr);
	socklen_t len = sizeof(local);

	if(connect(sock, (struct sockaddr*)&local, len) < 0) {
		LOG(ERROR, "connect error");
		return -1;
	}


	char send_buf[1024] = "GET http://www.nmc.cn/f/rest/real/57036?_=1553922369335 HTTP/1.1\r\nHost: www.nmc.cn\r\nConnection: keep-alive\r\nAccept: application/json, text/javascript, */*; q=0.01\r\nX-Requested-With: XMLHttpRequest\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36\r\nReferer: http://www.nmc.cn/publish/forecast/ASN/xian.html\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: zh-CN,zh;q=0.9\r\nCookie: UM_distinctid=169b4e8b444f-02b902ccfd3819-5f1d3a17-144000-169b4e8b4454da; followcity=54511%2C58367%2C59493%2C57516%2C58321%2C57679%2C58847; CNZZDATA1254743953=1878924922-1553514088-https%253A%252F%252Fwww.baidu.com%252F%7C1553919665\r\n\r\n";

	send(sock, send_buf, strlen(send_buf), 0);
	memset(send_buf, 0x00, sizeof(send_buf));
	recv(sock, send_buf, sizeof(send_buf), 0);

	std::cout << send_buf << std::endl;

	close(sock);
	close(fd);
}
