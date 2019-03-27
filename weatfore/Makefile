WORK_DIR=$(shell pwd)
cc=g++
bin=httpd
src=httpd.cc
LDFLAGS=-lpthread

.PHONY:all
all:$(bin) cgi

$(bin):$(src)
	$(cc) -o $@ $^ $(LDFLAGS) -w

cgi:
	cd search; make clean; make; cd -
	cd sql_connect; make clean; make; cd -

.PHONY:clean
clean:
	rm -f $(bin)
	cd search; make clean; cd -
	cd sql_connect; make clean; cd -
	# rm -rf output

.PHONY:output
output:
	mkdir -p output/wwwroot
	mkdir -p output/wwwroot/cgi
	mkdir -p output/wwwroot/html
	cp httpd output
	cp start.sh output
	cp wwwroot/*.html output/wwwroot
	cp wwwroot/html/*.html output/wwwroot/html
	cp search/search output/wwwroot/cgi
	cp sql_connect/insert_cgi output/wwwroot/cgi
	cp sql_connect/select_cgi output/wwwroot/cgi

