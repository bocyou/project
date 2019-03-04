.PHONY:clean all
WORK_PATH=$(shell pwd)
FLAG=-I$(WORK_PATH)/lib/include -L$(WORK_PATH)/lib/lib \
	 -lpthread -ljsoncpp -lctemplate -lmysqlclient -std=c++11
SRC=ojServer.cc
BIN=ojServer
cc=g++

all:$(BIN)


$(BIN):$(SRC)
	$(cc) $^ -o $@ $(FLAG)

clean:
	rm $(BIN) ./tmp/*
