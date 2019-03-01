.PHONY:clean all
WORK_PATH=$(shell pwd)
SELECT=$(WORK_PATH)/mysql_conn
FLAG=-lpthread -ljsoncpp -std=c++11
SRC=compileServer.cc
BIN=compileServer
cc=g++

all:select $(BIN)

select:
	cd $(SELECT); make clean; make; cd -;

$(BIN):$(SRC)
	$(cc) $^ -o $@ $(FLAG)

clean:
	cd $(SELECT); make clean; cd -;
	rm $(BIN) ./tmp/*
