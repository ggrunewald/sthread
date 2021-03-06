CC=gcc
AR=ar
LIB_DIR=./lib/
INC_DIR=./include/
BIN_DIR=./bin/
SRC_DIR=./src/
CFLAG=-Wall

all: clean libsthread.a

libsthread.a: list.o sthread.o
	$(AR) crs $(LIB_DIR)libsthread.a $(BIN_DIR)sthread.o $(BIN_DIR)list.o

sthread.o:
	$(CC) -c $(SRC_DIR)sthread.c $(CFLAGS)
	mv sthread.o $(BIN_DIR)

list.o:
	$(CC) -c $(SRC_DIR)list.c $(CFLAGS)
	mv list.o $(BIN_DIR)

clean:
	rm -rf $(LIB_DIR)*.a $(BIN_DIR)*.o $(SRC_BIN)*~ $(INC_BIN)*~ *~
