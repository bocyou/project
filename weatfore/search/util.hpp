#pragma once

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

class util {
private:
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

public:
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

	static int64_t getTimeStepSM() {
		struct timeval	tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
};
