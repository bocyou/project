#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

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
    
	//std::string city = arg;
	//city = urlDecode(city);
	//city += ".html";
	//city = "~/Linux/project/weatfore/output/wwwroot/html/" + city;
	
	std::string city = "/home/zmy/Linux/project/weatfore/wwwroot/html/西安.html";

	int fd = open(city.c_str(), O_RDONLY);
	if(fd < 0) {
		std::cout << "open error" << std::endl;
	}
	char buf[1024] = {0};
	while(read(fd, buf, sizeof(buf)-1) > 0) {
		printf("%s", buf);
	}

	//while(read(fd, buf, sizeof(buf)-1) > 0) {
	//	printf("%s", buf);
	//}

	close(fd);
}
