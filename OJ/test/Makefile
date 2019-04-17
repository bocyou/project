src=cookieTest.cc
obj=bin

$(obj):$(src)
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean

clean:
	rm $(bin)
