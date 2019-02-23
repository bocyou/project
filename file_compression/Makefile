.PHONY:clean

bin=main
src=fileCompressHuff.cc

$(bin):$(src) main.cc
	g++ $^ -o $@ -g -std=c++11

clean:
	rm $(bin) test.huff
