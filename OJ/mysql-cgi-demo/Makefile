HTML_DIR := /var/www/html
CGI_DIR := /var/www/cgi-bin

.PHONY: all install clean

all: register.cgi login.cgi

register.cgi: src/register.cc src/unencode.cc src/split.cc src/mysql_utils.cc
	g++ -std=c++11 -Iinclude -g $^ -o build/$@ `mysql_config --cflags --libs`

login.cgi: src/login.cc src/unencode.cc src/split.cc src/mysql_utils.cc
	g++ -std=c++11 -Iinclude -g $^ -o build/$@ `mysql_config --cflags --libs`

install: build/login.cgi build/register.cgi
	cp build/register.cgi ${CGI_DIR}/
	cp build/login.cgi ${CGI_DIR}/

	cp html/register.html ${HTML_DIR}/
	cp html/login.html ${HTML_DIR}/

clean:
	rm -rf build/*
