.PHONY:clean
src=cli.cc
bin=cli

$(bin):$(src)
	g++ $^ -o $@ -std=c++11 -lpthread -ljsoncpp -g

clean:
	rm $(bin) ./tmp/*
