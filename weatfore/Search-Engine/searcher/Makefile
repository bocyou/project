

.PHONY:all

all: search libsearcher.so

search:searCher.cc test.cc
	g++ $^ -o $@ -std=c++11 -ljsoncpp

libsearcher.so:searCher.h searCher.cc
	g++ searCher.cc -shared -fPIC -o $@ -std=c++11 -ljsoncpp

.PHONY:clean

clean:
	rm search
