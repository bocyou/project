.PHONY:clean

bin=main
src=fileCompressHuff.cc

$(bin):$(src) main.cc
	g++ $^ -o $@ -g -std=c++11 -w

clean:
	rm $(bin) 
