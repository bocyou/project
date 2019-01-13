.PHONY:clean

all:math_cgi input 

math_cgi:math_cgi.c
	gcc $^ -o $@

input:input.cc
	g++ $^ -o $@

clean:
	rm math_cgi input
