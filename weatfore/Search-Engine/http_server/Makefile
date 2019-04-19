FLAG=-std=c++11 -lpthread -I ~/third_part/include

http_server:http_ser.cc ../searcher/libsearcher.so
	g++ $^ ../searcher/libsearcher.so -o $@ $(FLAG) 

.PHONY:clean

clean:
	rm http_server
