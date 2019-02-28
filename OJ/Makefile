.PHONY:clean
src=compileServer.cc
bin=compileServer

$(bin):$(src)
	g++ $^ -o $@ -std=c++11 -lpthread -ljsoncpp -g

clean:
	rm $(bin) ./tmp/*
