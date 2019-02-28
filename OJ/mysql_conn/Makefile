.PHONY:clean all
cc=g++
CURR_PATH=$(shell pwd)
LIB=$(CURR_PATH)/../lib
INCLUDE=-I$(LIB)/include
SQL_LIB=-L$(LIB)/lib -lmysqlclient

all:selectMysql

selectMysql:selectMysql.cc comm.cc
	$(cc) $^ -o $@ $(INCLUDE) $(SQL_LIB)

clean:
	rm selectMysql
