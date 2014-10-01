#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib
INC_DIR=../include

all: exemplo mandel teste_vetor teste_prio

exemplo: clean exemplo.c $(LIB_DIR)/libsthread.a
	$(CC) -o exemplo exemplo.c -L$(LIB_DIR) -lsthread -Wall

1: teste_vetor.c $(LIB_DIR)/libsthread.a
	$(CC) -o 1.ex teste_vetor.c -L$(LIB_DIR) -lsthread -Wall

2: teste_prio.c $(LIB_DIR)/libsthread.a
	$(CC) -o 2.ex teste_prio.c -L$(LIB_DIR) -lsthread -Wall

mandel: mandel.c $(LIB_DIR)/libsthread.a
	$(CC) -o mandel mandel.c -L$(LIB_DIR) -lsthread -lm -Wall

clean:
	rm -rf exemplo mandel teste_vetor teste_prio *~ *.ex
