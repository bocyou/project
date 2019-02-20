.PHONY:clean

bin=main
src=fileCompressHuff.cc

$(bin):$(src) main.cc
	g++ $^ -o $@ -g

clean:
	rm $(bin)
